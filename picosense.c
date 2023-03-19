#include <string.h>
#include <time.h>
#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>
#include <sensor_reporter.h>
#include <sensor_reader.h>

int main(int argc, const char *argv[])
{
  stdio_init_all();
  if (cyw43_arch_init())
  {
    printf("WiFi init failed");
    return -1;
  }

  cyw43_arch_enable_sta_mode();

  printf("Connecting to WiFi...\n");
  if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000))
  {
    printf("failed to connect.\n");
    return 1;
  }
  else
  {
    printf("Connected to wifi.\n");
  }

  init_sensor_reporter();
  init_sensor_reader();

  const int update_frequency_ms = 10 * 1000;
  absolute_time_t next_update = make_timeout_time_ms(update_frequency_ms);

  // TODO add watchdog

  while (true)
  {
  #if 1
    if (time_reached(next_update))
    {
      sensor_values_t values = {0};

      if (!read_sensor_values(&values))
      {
        continue;
      }

      printf("Temp     %.2f C\n", values.temperature_celcius);
      printf("Pressure %.2f kPa\n", values.pressure_kPa);
      printf("Humidity %.2f %%\n", values.humidity_rel);

      report_sensor_values(&values);

      next_update = make_timeout_time_ms(update_frequency_ms);
    }
  #endif

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(1000);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    sleep_ms(1000);
  }

  // cyw43_arch_deinit();
}
