void setup() {
  Serial.begin(9600);  // USB Serial communication
  while (!Serial);     // Wait for the serial connection to establish

  // Initialize Serial1 for ESP8266 communication
  Serial1.begin(9600); // Match the baud rate with the ESP8266
  Serial.println("Arduino ready");
}

int value = 0;

void loop() {
  Serial1.println(value);  // Send value to the PC
  value = (value + 1) % 4;  // Cycle through 0, 1, 2, 3
  delay(3000);  // Wait for 1 second
}


