// 

#include "mbed.h"
#include "ESP8266Interface.h"
#include "MQTTClientMbedOs.h"
#include "MQTTmbed.h"
#include "MQTTClient.h"
#include <string.h>

// Defines the SSID and password from the JSON file
#define WIFI_SSID MBED_CONF_APP_WIFI_SSID
#define WIFI_PASSWORD MBED_CONF_APP_WIFI_PASSWORD

// Defines the switch and LED pins
DigitalIn limitSwitch(A3);  // Switch
DigitalOut led(A6);         // LED

// Creates the ESP8266 WiFi interface
ESP8266Interface esp(D5, D4);

// Custom strlen since string.h does not work
size_t my_strlen(const char* str) {
    size_t length = 0;
    while (str[length] != '\0') {
        ++length;
    }
    return length;
}

// Function to send MQTT message
void send_mqtt_message() {
    printf("Connecting to MQTT broker...\r\n");
    
    // Define the MQTT broker address
    SocketAddress MQTTBroker;
    if (esp.gethostbyname("test.mosquitto.org", &MQTTBroker, NSAPI_IPv4, NULL) != 0) {
        printf("DNS connection failed\n");
        return;
    }
    MQTTBroker.set_port(1883);  // Standard MQTT TCP port

    // Open and connect socket
    TCPSocket socket;
    socket.open(&esp);
    if (socket.connect(MQTTBroker) != 0) {
        printf("Socket connection failed\n");
        return;
    }

    // Create MQTT client
    MQTTClient client(&socket);

    // MQTT connection settings
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = (char*)"MQTTnoti";

    // Connect to the MQTT broker
    if (client.connect(data) != 0) {
        printf("MQTT connection failed!\n");
        socket.close();
        return;
    }

    // Define message with topic "MQTTnoti"
    const char* topic = "MQTTnoti";
    char buffer[] = "Mail has arrived!";
    MQTT::Message msg;
    msg.qos = MQTT::QOS0;
    msg.retained = false;
    msg.dup = false;
    msg.payload = (void*)buffer;
    msg.payloadlen = my_strlen(buffer);

    // Send the message
    if (client.publish(topic, msg) == 0) {
        printf("Message sent: %s\n", topic);
    } else {
        printf("Message sending failed!\n");
    }

    // Disconnect from the MQTT broker
    client.disconnect();
    socket.close();
}

// Function to connect to Wi-Fi
nsapi_error_t connect_wifi() {
    printf("\nConnecting to Wi-Fi...\r\n");

    nsapi_error_t status = esp.connect(WIFI_SSID, WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    if (status != 0) {
        printf("WiFi connection failed! Error: %d\r\n", status);
        return status;
    }

    // Print network settings
    SocketAddress ip;
    esp.get_ip_address(&ip);
    printf("Wi-Fi connection successful! IP: %s\r\n", ip.get_ip_address());

    return 0;
}

// Function to disconnect from Wi-Fi
void disconnect_wifi() {
    esp.disconnect();
    printf("Wi-Fi connection disconnected.\r\n");
}

int main() {
    led = 1;  // Turn off LED initially
    bool notificationSent = false;

    while (true) {
        if (limitSwitch.read() == 0) {  // If the switch is pressed
            led = 0;  // Turn on the LED

            if (!notificationSent) {  // Check if the notification has already been sent
                printf("Connecting to Wi-Fi...\r\n");

                if (connect_wifi() == 0) {  // If Wi-Fi connection is successful
                    printf("Waiting for 3 seconds...\r\n");
                    ThisThread::sleep_for(3s);

                    send_mqtt_message();  // Send MQTT message

                    disconnect_wifi();  // Disconnect from Wi-Fi
                    notificationSent = true;
                } else {
                    printf("Wi-Fi connection failed! Message will not be sent.\r\n");
                }
            }
        } else {  // If the switch is not pressed
            led = 1;  // Turn off the LED
            notificationSent = false;  // Reset notification status
        }

        ThisThread::sleep_for(5s);  // Delay to slow down the loop
    }
}
