#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Wi-Fi credentials
const char* ssid = "Pablo";          // Replace with your Wi-Fi network name
const char* password = "pablo123";  // Replace with your Wi-Fi password

// MQTT broker details
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char* topic = "eie/ucon/proy/luz";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.begin(9600);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (Serial.available()) {  // Read message from Arduino
    String message = Serial.readStringUntil('\n');
    message.trim();  // Remove any extra whitespace or newline characters

    Serial.println("Received from Arduino: " + message);  // Debugging
    client.publish(topic, message.c_str());  // Publish to MQTT
    Serial.println("Published to MQTT: " + message);
  }
}
