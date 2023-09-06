#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#define PIN_LED1 12
#define PIN_LED2 13

#define PIN_PIXS 8
#define PIX_NUM 64

Adafruit_NeoPixel pixels(PIX_NUM, PIN_PIXS, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);

  pixels.begin();
  pixels.setBrightness(5);
}

void loop() {
  for (uint16_t i = 0; i < 65536; i += 30) {
    pixels.rainbow(i, -1, 255, 255);
    pixels.show();
  }
}
