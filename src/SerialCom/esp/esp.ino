#include <SoftwareSerial.h>


// Define GPIO pins directly
#define RX_PIN 4 // D2 (GPIO4)
#define TX_PIN 0 // D3 (GPIO0)

SoftwareSerial espSerial(RX_PIN, TX_PIN);
void setup() {
  Serial.begin(9600);          // Debugging
  espSerial.begin(9600);       // Communication with Arduino
  Serial.println("ESP8266 ready");
}

void loop() {
  if (espSerial.available()) {
    Serial.println("ESP8266 loop ready");
    String data = espSerial.readStringUntil('\n');
    Serial.println("Received: " + data);
  }
}
