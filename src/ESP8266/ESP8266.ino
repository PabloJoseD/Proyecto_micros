#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

#define RX_PIN 4 // D2 (GPIO4)
#define TX_PIN 0 // D3 (GPIO0)


// Wi-Fi credentials
const char* ssid = "FGV";          // Replace with your Wi-Fi network name
const char* password = "segav2298";  // Replace with your Wi-Fi password

// MQTT broker details
const char *mqtt_broker = "broker.emqx.io";  // EMQX broker endpoint
const char *mqtt_topic = "eie/ucon/proy/luz";     // MQTT topic
const char *mqtt_username = "emqx";  // MQTT username for authentication
const char *mqtt_password = "public";  // MQTT password for authentication
const int mqtt_port = 1883;  // MQTT port (TCP)

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

SoftwareSerial espSerial(RX_PIN, TX_PIN);

// Function declarations
void connectToWiFi();
void connectToMQTTBroker();

void setup() {
    Serial.begin(9600);
    espSerial.begin(9600);       // Communication with Arduino
    connectToWiFi();
    mqtt_client.setServer(mqtt_broker, mqtt_port);
    connectToMQTTBroker();
    Serial.println("ESP8266 ready");
}

void connectToWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to the WiFi network");
}

void connectToMQTTBroker() {
    while (!mqtt_client.connected()) {
        String client_id = "esp8266-client-" + String(WiFi.macAddress());
        Serial.printf("Connecting to MQTT Broker as %s.....\n", client_id.c_str());
        if (mqtt_client.connect(client_id.c_str())) {
            Serial.println("Connected to MQTT broker");
            //mqtt_client.subscribe(mqtt_topic);
            // Publish message upon successful connection
            mqtt_client.publish(mqtt_topic, "Hi EMQX I'm ESP8266 ^^");
        } else {
            Serial.print("Failed to connect to MQTT broker, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}


void loop() {
    if (!mqtt_client.connected()) {
        connectToMQTTBroker();
    }
    mqtt_client.loop();
    
    // Check for data from Arduino on TX/RX interface
    if (espSerial.available()) {
        Serial.println("ESP8266 loop ready");
        String data = espSerial.readStringUntil('\n');
        Serial.println("Received from Arduino: " + data);  // Debugging
        mqtt_client.publish(mqtt_topic, data.c_str());  // Publish to MQTT
        Serial.println("Published to MQTT: " + data);
    }
}

