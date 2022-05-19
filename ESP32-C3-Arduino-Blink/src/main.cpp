#include <Arduino.h>

#define PIN_LED1 12
#define PIN_LED2 13

void setup() {
  Serial.begin(115200);
  Serial.println("Hello ESP32C3!!");
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
}

void loop() {
  digitalWrite(PIN_LED1, HIGH);
  digitalWrite(PIN_LED2, LOW);
  delay(200);
  digitalWrite(PIN_LED2, HIGH);
  digitalWrite(PIN_LED1, LOW);
  delay(200);
}