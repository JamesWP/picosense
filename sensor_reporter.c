#include <sensor_reporter.h>

#include <lwip/apps/mqtt.h>
#include <lwip/debug.h>
#include <pico/platform.h>
#include <cyw43.h>

static mqtt_client_t *client;
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);
static void publish_cb(void *arg, err_t err);
void mqtt_connect();

#define BUFF_LEN 100
static char buff[BUFF_LEN] = {0};

static int datapoint_TEMP=0x10000000;
static int datapoint_HUM=0x20000000;
static int datapoint_PRES=0x30000000;
static int datapoint_NUM_MASK=0x0FFFFFFF;

static bool has_failed = false;

void* cookie(int datapoint_number, int datapoint_type) {
    int a = datapoint_number & datapoint_NUM_MASK;
    return (void*)(a | datapoint_type);
}
void report_sensor_values_impl(sensor_values_t *values, reporting_status_t *status);
void report_sensor_values(sensor_values_t *values, reporting_status_t *status) 
{
    cyw43_arch_lwip_begin();
    report_sensor_values_impl(values, status);
    cyw43_arch_lwip_end();
}

void report_sensor_values_impl(sensor_values_t *values, reporting_status_t *status)
{

    int link_status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);

    switch (link_status) {
        case CYW43_LINK_DOWN:
            sprintf(status->wifi_state, "DW");
            status->is_wifi_connected = false;
            break;
        case CYW43_LINK_JOIN:
            sprintf(status->wifi_state, "JO");
            status->is_wifi_connected = true;
            break;
        case CYW43_LINK_NOIP:
            sprintf(status->wifi_state, "NI");
            status->is_wifi_connected = true;
            break;
        case CYW43_LINK_NONET:
            sprintf(status->wifi_state, "NN");
            status->is_wifi_connected = false;
            break;
        case CYW43_LINK_BADAUTH:
            sprintf(status->wifi_state, "BA");
            status->is_wifi_connected = false;
            break;
        case CYW43_LINK_UP:
            sprintf(status->wifi_state, "UP");
            status->is_wifi_connected = true;
            break;
        default:
            sprintf(status->wifi_state, "--");
            status->is_wifi_connected = false;
    }

    status->wifi_state[2] = '\0';

    if (!mqtt_client_is_connected(client))
    {
        printf("Skipping publishing data. No Connection\n");
        status->is_mqtt_connected = false;
        status->is_publish_success = false;
        return;
    } else {
        status->is_mqtt_connected = true;
    }

    int length;
    err_t err;

    static int datapoint = 1;

    printf("Publishing datapoint #%d\n", datapoint++);

    length = snprintf(buff, BUFF_LEN, "%.2f", values->temperature_celcius);
    err = mqtt_publish(client, "pico/temperature", buff, length, 0, 1, publish_cb, cookie(datapoint, datapoint_TEMP));

    if (err != ERR_OK)
    {
        printf("mqtt_publish return: %hhx\n", err);
        status->is_publish_success = false;
        return;
    }

    length = snprintf(buff, BUFF_LEN, "%.2f", values->pressure_kPa);
    err = mqtt_publish(client, "pico/pressure", buff, length, 0, 1, publish_cb, cookie(datapoint, datapoint_PRES));

    if (err != ERR_OK)
    {
        printf("mqtt_publish return: %hhx\n", err);
        status->is_publish_success = false;
        return;
    }

    length = snprintf(buff, BUFF_LEN, "%.2f", values->humidity_rel);
    err = mqtt_publish(client, "pico/humidity", buff, length, 0, 1, publish_cb, cookie(datapoint, datapoint_HUM));

    if (err != ERR_OK)
    {
        printf("mqtt_publish return: %hhx\n", err);
        status->is_publish_success = false;
        return;
    }

    status->is_publish_success = !has_failed;

    has_failed = false;
}

void init_sensor_reporter()
{
    client = mqtt_client_new();
    mqtt_connect();
}

void mqtt_connect() {
    cyw43_arch_lwip_begin();
    ip_addr_t ip_addr = IPADDR4_INIT(0x00000000);
    if (1 != ip4addr_aton(MQTT_SERVER_IP, &ip_addr)) {
        printf("Invalid ipv4 address given in MQTT_SERVER_IP: %s\n", MQTT_SERVER_IP);
        panic("Bad ip");
    } 

    struct mqtt_connect_client_info_t ci = {0};

    ci.client_id = "lwip_test";
    ci.keep_alive = 15;

    err_t err = mqtt_client_connect(client, &ip_addr, MQTT_PORT, mqtt_connection_cb, 0, &ci);

    if (err != ERR_OK)
    {
        printf("mqtt_connect return %hhx\n", err);
    }
    cyw43_arch_lwip_end();
}

void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    err_t err;
    if (status == MQTT_CONNECT_ACCEPTED)
    {
        printf("mqtt_connection_cb: Successfully connected\n");

        if (err != ERR_OK)
        {
            printf("mqtt_connection_cb error return: %hhx\n", err);
        }
    }
    else
    {
        printf("mqtt_connection_cb: Disconnected, reason: %d\n", status);
	    mqtt_connect();
    }
}

void publish_cb(void *arg, err_t err)
{
    int datapoint = (int)arg;

    if(err == ERR_OK) {
        printf("Publish datapoint 0x%08x success\n", datapoint);
        return;
    }

    has_failed = true;
    printf("Publish datapoint 0x%08x result: %d\n", datapoint, err);
}
