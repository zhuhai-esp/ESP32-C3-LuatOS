#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft;
TFT_eSprite drawBuf(&tft);

void setup() {
  Serial.begin(115200);
  Serial.println("Hello ESP32C3!!");
  tft.begin();
  drawBuf.createSprite(TFT_WIDTH, TFT_HEIGHT);
  drawBuf.setTextFont(2);
  drawBuf.fillScreen(TFT_BLACK);
  drawBuf.drawString("Hello ESP32C3!!", 0, 0);
  drawBuf.drawRect(2, 20, 100, 20, TFT_RED);
  drawBuf.drawRect(2, 60, 100, 20, TFT_GREEN);
  drawBuf.drawRect(2, 100, 100, 20, TFT_BLUE);
  drawBuf.pushSprite(0, 0);
}

void loop() {}