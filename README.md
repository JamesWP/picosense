# Raspberry Pi Pico - MQTT temperature / humidity / pressure publisher

# Build instructions

```
$ mkdir build
$ cd build
$ cmake .. \
        -DCMAKE_BUILD_TYPE=Debug \
        -DPICO_BOARD=pico_w \ 
        -DWIFI_SSID=<YOUR_SSID> \ 
        -DWIFI_PASSWORD=<YOUR_PASSWORD> \ 
        -DMQTT_SERVER_IP=<MQTT_BROKER_IPv4> \
        -DPICO_SDK_PATH=<PATH_TO_pico-sdk>
$ cmake --build .
```
n.b. `MQTT_BROKER_IPv4` should be in `,` separated format: `192,168,1,1`
