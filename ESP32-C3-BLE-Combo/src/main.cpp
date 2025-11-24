#include <Arduino.h>
#include <BleCombo.h>
#include <OneButton.h>
#include <WiFi.h>

#define PIN_LED1 12
#define PIN_LED2 13
#define PIN_BOOT 9

char buf[128] = {0};
OneButton btnBoot(PIN_BOOT, true);
uint64_t check5ms = 0, ms100ms = 0, ms300ms = 0, check1s = 0;

inline void startBLEDevice() {
  uint8_t mac[6];
  esp_read_mac(mac, ESP_MAC_WIFI_SOFTAP);
  sprintf(buf, "ESPCombo-%02X%02X", mac[4], mac[5]);
  Keyboard.deviceName = buf;
  Keyboard.begin();
  Mouse.begin();
  Keyboard.setBatteryLevel(98);
}

inline void initButtons() {
  btnBoot.attachClick([]() {
    Serial.println("Boot Key Pressed!!!");
    if (Keyboard.isConnected()) {
      Keyboard.press(KEY_BACKSPACE);
      delay(200);
      Keyboard.releaseAll();
    }
  });
}

void setup() {
  Serial.begin(115200);
  Serial.println("Hello ESP32C3!!");
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  digitalWrite(PIN_LED1, HIGH);
  startBLEDevice();
  initButtons();
  digitalWrite(PIN_LED1, LOW);
}

void loop() {
  auto cur = millis();
  if (cur - check5ms > 5) {
    check5ms = cur;
    btnBoot.tick();
  }
  if (cur - ms100ms > 100) {
    ms100ms = cur;
  }
  if (cur - ms300ms > 300) {
    ms300ms = cur;
  }
  if (cur - check1s > 1000) {
    check1s = cur;
  }
}