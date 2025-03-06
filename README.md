# IOT-project - Smart Mailbox

For our Internet of Things (IoT) project, we designed a smart mailbox system that sends notifications when mail has arrived. The system is activated when the lid of the mailbox is opened. This action triggers a limit switch, which in turn activates an LED to visually indicate the status of the mailbox. When the limit switch is released, the LED turns off, signaling that the mailbox has been closed.
<br>
<br>
Once the limit switch is triggered, the microcontroller code begins executing. First, the Wi-Fi module is activated and connects to a predefined Wi-Fi network using the credentials stored in the system. Once the Wi-Fi connection is established, the system sends an MQTT message to the broker at test.mosquitto.org under the topic "MQTTnoti".
<br>
<br>
In parallel, on the server side, we have set up a flow in Node-RED, which listens to the test.mosquitto.org broker for messages under the topic "MQTTnoti". When Node-RED receives a message, it triggers an email notification to a predefined email address, alerting the user that mail has arrived in the mailbox.
<br>
<br>
This system demonstrates a simple yet effective application of IoT technologies to automate notifications and enhance convenience.
<br>
<br>
<br>
We used this GitHub repository to send MQTT messages to Node-RED: https://github.com/ARMmbed/mbed-mqtt.
With a few modifications, we managed to build the mbed project. The code did not build without errors initially.

In the folder located at: /home/studio/workspace/"PROJECT"/mbed-mqtt/src/MQTTClientMbedOs.h, we had to add a couple of lines:

- MQTT::Client<MQTTNetworkMbedOs, Countdown, MBED_CONF_MBED_MQTT_MAX_PACKET_SIZE, MBED_CONF_MBED_MQTT_MAX_CONNECTIONS> *client;<br>
- MQTTSN::Client<MQTTNetworkMbedOs, Countdown, MBED_CONF_MBED_MQTT_MAX_PACKET_SIZE, MBED_CONF_MBED_MQTT_MAX_CONNECTIONS> *clientSN;
<br>
  
Additionally, in the folder called: ***
We made other improvements.

This version makes the text clearer and more concise.
