#include <Arduino.h>
#include <WiFi.h>

#define PIN_LED1 12
#define PIN_LED2 13

void setup() {
  Serial.begin(115200);
  Serial.println("Hello ESP32C3!!");
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
}

void loop() {
  static u32_t round = 0;
  digitalWrite(PIN_LED1, HIGH);
  Serial.printf("[%d] network scan start!\n", round);
  int16_t count = WiFi.scanNetworks();
  digitalWrite(PIN_LED1, LOW);
  Serial.printf("[%d] %d networks Found\n", ++round, count);
  for (int i = 0; i < count; ++i) {
    Serial.printf("%2d. [%24s] [%4d] [%2d]\n", i + 1, WiFi.SSID(i).c_str(),
                  WiFi.RSSI(i), WiFi.channel(i));
  }
  Serial.println();
  delay(5000);
}