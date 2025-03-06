# IOT-project - Smart Mailbox

For our Internet of Things (IoT) project, we chose to receive notifications when mail has arrived.



We used this GitHub repository to send MQTT messages to Node-RED: https://github.com/ARMmbed/mbed-mqtt.
With a few modifications, we managed to build the mbed project. The code did not build without errors initially.

In the folder located at: /home/studio/workspace/"PROJECT"/mbed-mqtt/src/MQTTClientMbedOs.h, we had to add a couple of lines:

MQTT::Client<MQTTNetworkMbedOs, Countdown, MBED_CONF_MBED_MQTT_MAX_PACKET_SIZE, MBED_CONF_MBED_MQTT_MAX_CONNECTIONS> *client;
MQTTSN::Client<MQTTNetworkMbedOs, Countdown, MBED_CONF_MBED_MQTT_MAX_PACKET_SIZE, MBED_CONF_MBED_MQTT_MAX_CONNECTIONS> *clientSN;
  
Additionally, in the folder called: ***
We made other improvements.

This version makes the text clearer and more concise.
