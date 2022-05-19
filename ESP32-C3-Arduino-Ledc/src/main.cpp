#include <Arduino.h>
#include <math.h>

#define PIN_LED1 12
#define PIN_LED2 13

void setup() {
  Serial.begin(115200);
  Serial.println("Hello ESP32C3!!");
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);

  ledcSetup(0, 800, 10);
  ledcAttachPin(PIN_LED1, 0);
  ledcSetup(1, 800, 10);
  ledcAttachPin(PIN_LED2, 1);
}

void loop() {
  static int duty = 0;
  static int step = 1;
  ledcWrite(0, duty);
  ledcWrite(1, 40 - duty);
  delay(30);
  duty += step;
  if (abs(duty - 20) >= 20) {
    step = -step;
  }
}