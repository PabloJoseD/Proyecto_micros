void setup() {
  Serial.begin(9600);    // Serial Monitor
  Serial1.begin(9600);   // Communication with ESP8266 on Serial1 (TX1/RX1)
  randomSeed(analogRead(0));  // Seed the random number generator
}

void loop() {
  int randomValue = random(0, 4);  // Generate a random value between 0 and 3
  Serial1.println(randomValue);   // Send to ESP8266
  Serial.println("Sent to ESP: " + String(randomValue));  // Debugging
  delay(5000);  // Send data every 5 seconds
}
