#include <Arduino.h>
#include <math.h>

#define PIN_LED1 12
#define PIN_LED2 13

void setup() {
  Serial.begin(115200);
  Serial.println("Hello ESP32C3!!");
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);

  ledcSetup(0, 1000, 10);
  ledcAttachPin(PIN_LED1, 0);
  ledcSetup(1, 1000, 10);
  ledcAttachPin(PIN_LED2, 1);
}

void loop() {
  static int duty = 0;
  static int step = 10;
  ledcWrite(0, duty);
  ledcWrite(1, 1000 - duty);
  delay(20);
  duty += step;
  if (abs(duty-500) >= 500) {
    step = -step;
  }
}