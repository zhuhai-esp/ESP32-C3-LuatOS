#include <Arduino.h>
#include <WiFi.h>

#define PIN_LED1 12
#define PIN_LED2 13

void setup() {
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);
  Serial.begin(115200);
}

void loop() {
  int n = WiFi.scanNetworks();
  Serial.printf("Scan Result: [%d] APs\n", n);
  if (n > 0) {
    Serial.println("---------START----------");
    for (int i = 0; i < n; i++) {
      Serial.printf("%02d: %s(%02d)\n", i, WiFi.SSID(i).c_str(),
                    WiFi.channel(i));
    }
    Serial.println("----------END-----------");
  }
  digitalWrite(PIN_LED1, HIGH);
  delay(20);
  digitalWrite(PIN_LED1, LOW);
  delay(1000);
}