#include <sensor_reporter.h>

#include <lwip/apps/mqtt.h>
#include <lwip/debug.h>

static mqtt_client_t *client;
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);
static void publish_cb(void *arg, err_t err);
void mqtt_connect();

#define BUFF_LEN 100
static char buff[BUFF_LEN] = {0};

void report_sensor_values(sensor_values_t *values)
{
    if (!mqtt_client_is_connected(client))
    {
        printf("Skipping publishing data. No Connection\n");
        return;
    }
    int length;
    err_t err;

    length = snprintf(buff, BUFF_LEN, "%.2f C", values->temperature_celcius);
    err = mqtt_publish(client, "pico/temperature", buff, length, 0, 1, publish_cb, NULL);

    length = snprintf(buff, BUFF_LEN, "%.2f kPa", values->pressure_kPa);
    err = mqtt_publish(client, "pico/pressure", buff, length, 0, 1, publish_cb, NULL);

    length = snprintf(buff, BUFF_LEN, "%.2f %%", values->humidity_rel);
    err = mqtt_publish(client, "pico/humidity", buff, length, 0, 1, publish_cb, NULL);

    if (err != ERR_OK)
    {
        printf("mqtt_subscribe return: %hhx\n", err);
    }
}

void init_sensor_reporter()
{
    client = mqtt_client_new();
    mqtt_connect();
}

void mqtt_connect() {
    ip_addr_t ip_addr; // = IPADDR4_INIT_BYTES(MQTT_SERVER_IP);
    IP4_ADDR(&ip_addr, 192, 168, 178, 27);

    struct mqtt_connect_client_info_t ci = {0};

    ci.client_id = "lwip_test";
    ci.keep_alive = 60;

    err_t err = mqtt_client_connect(client, &ip_addr, MQTT_PORT, mqtt_connection_cb, 0, &ci);

    if (err != ERR_OK)
    {
        printf("mqtt_connect return %hhx\n", err);
    }
}

void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    err_t err;
    if (status == MQTT_CONNECT_ACCEPTED)
    {
        printf("mqtt_connection_cb: Successfully connected\n");

        if (err != ERR_OK)
        {
            printf("mqtt_subscribe return: %hhx\n", err);
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
    if(err == ERR_OK) {
        return;
    }
    printf("Publish result: %d\n", err);
}
