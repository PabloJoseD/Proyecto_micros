void setup() {
  Serial.begin(9600);   // Initialize Serial Monitor for debugging
  Serial1.begin(9600);  // Initialize Serial1 for communication
  while (!Serial);      // Wait for Serial Monitor to be ready
}

void loop() {
  for (int i = 1; i <= 4; i++) {
    Serial1.println(i);  // Send number to ESP8266
    Serial.println("Sent: " + String(i)); // Print sent number to Serial Monitor
    delay(1000);          // Wait for 1 second before sending the next number
  }
}
