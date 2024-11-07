#include <SoftwareSerial.h>

SoftwareSerial espSerial(10, 11);  // RX, TX for ESP8266

void setup() {
  Serial.begin(9600);    // Serial Monitor
  espSerial.begin(9600); // Communication with ESP8266
  randomSeed(analogRead(0));  // Seed the random number generator
}

void loop() {
  int randomValue = random(0, 2);  // Generate 0 or 1
  espSerial.println(randomValue);  // Send to ESP8266
  Serial.println("Sent to ESP: " + String(randomValue));  // Debugging
  delay(2000);  // Send data every 2 seconds
}
