#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi settings
const char *ssid = "Pablo";             // Replace with your WiFi name
const char *password = "pablo123";   // Replace with your WiFi password

// MQTT Broker settings
const char *mqtt_broker = "broker.emqx.io";  // EMQX broker endpoint
const char *mqtt_topic = "eie/ucon/proy/luz";     // MQTT topic
const char *mqtt_username = "emqx";  // MQTT username for authentication
const char *mqtt_password = "public";  // MQTT password for authentication
const int mqtt_port = 1883;  // MQTT port (TCP)

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

// Pin de los LEDs
const int Abanico = D8; 
const int Bombillo = D7;  

void connectToWiFi();
void connectToMQTTBroker();
void mqttCallback(char *topic, byte *payload, unsigned int length);

void setup() {
    Serial.begin(115200);
    

     // Configurar pines de los LEDS como salida
    pinMode(Abanico, OUTPUT);
    pinMode(Bombillo, OUTPUT);

    // Iniciar con los LEDS apagados
    digitalWrite(Abanico, LOW);
    digitalWrite(Bombillo, LOW);

    connectToWiFi();
    mqtt_client.setServer(mqtt_broker, mqtt_port);
    mqtt_client.setCallback(mqttCallback);
    connectToMQTTBroker();
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
            mqtt_client.subscribe(mqtt_topic);
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

void mqttCallback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message received on topic: ");
    Serial.println(topic);
    Serial.print("Message:");

    String message = "";
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    message.trim(); // Eliminar espacios y caracteres extra al inicio o final
    Serial.println(message);
    Serial.println("-----------------------");

    // Toggle logic for each command
    if (message == "0") {
        digitalWrite(Abanico, !digitalRead(Abanico)); // Toggle Abanico
        Serial.println(digitalRead(Abanico) ? "Abanico ON" : "Abanico OFF");
    } else if (message == "1") {
        digitalWrite(Bombillo, !digitalRead(Bombillo)); // Toggle Bombillo
        Serial.println(digitalRead(Bombillo) ? "Bombillo ON" : "Bombillo OFF");
    } else if (message == "2") {
      digitalWrite(Abanico, LOW);
      digitalWrite(Bombillo, LOW);
    } else {
        Serial.println("Unknown command");
    }
}


void loop() {
    if (!mqtt_client.connected()) {
        connectToMQTTBroker();
    }
    mqtt_client.loop();

}