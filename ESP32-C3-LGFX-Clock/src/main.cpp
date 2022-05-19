#include "lgfx.h"
#include <ArduinoOTA.h>

#define PIN_LED1 12
#define PIN_LED2 13

#define KEY_LEFT 9
#define KEY_RIGHT 13
#define KEY_UP 8
#define KEY_DOWN 5
#define KEY_CENTER 4

static LGFX tft;
static LGFX_Sprite canvas(&tft);
char buf[256];
long check300ms = 0;
long check5ms = 0;

void inline blinkLEDs() {
  digitalWrite(PIN_LED1, HIGH);
  digitalWrite(PIN_LED2, LOW);
  delay(1000);
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, HIGH);
  delay(1000);
}

void inline startWifiConfig() {
  delay(500);
  WiFi.mode(WIFI_STA);
  WiFi.begin();
  tft.clear();
  tft.drawCenterString("Connecting WiFi...", 80, 30);
  for (int i = 0; i < 10; i++) {
    if (WiFi.status() == WL_CONNECTED) {
      break;
    }
    delay(500);
  }
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.beginSmartConfig();
    tft.clear();
    tft.drawCenterString("Config With ESPTouch", 80, 30);
    while (!WiFi.smartConfigDone()) {
      delay(500);
    }
  }
  while (!WiFi.localIP()) {
    delay(200);
  }
}

void inline startConfigTime() {
  tft.clear();
  tft.drawCenterString("Configing Time", 80, 30);
  const int timeZone = 8 * 3600;
  configTime(timeZone, 0, "ntp6.aliyun.com", "cn.ntp.org.cn", "ntp.ntsc.ac.cn");
  while (time(nullptr) < 8 * 3600 * 2) {
    delay(500);
  }
  delay(500);
}

void inline setupOTAConfig() {
  ArduinoOTA.onStart([] {});
  ArduinoOTA.onProgress(
      [](u32_t pro, u32_t total) { int per = pro * 100 / total; });
  ArduinoOTA.onEnd([] {});
  ArduinoOTA.begin();
}

inline void showCurrentTime() {
  struct tm info;
  getLocalTime(&info);
  canvas.fillScreen(TFT_BLACK);
  strftime(buf, 32, "%F", &info);
  canvas.setTextColor(TFT_PINK);
  canvas.drawCenterString(buf, 80, 4, &AsciiFont8x16);
  strftime(buf, 32, "%T", &info);
  canvas.setTextColor(TFT_GREEN);
  canvas.drawCenterString(buf, 80, 26, &FreeSansBold18pt7b);
  sprintf(buf, "IP: %s", WiFi.localIP().toString().c_str());
  canvas.setTextColor(TFT_YELLOW);
  canvas.drawCenterString(buf, 80, 64, &AsciiFont8x16);
  canvas.pushRotateZoomWithAA(0, 1, 1);
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  tft.init();
  tft.setBrightness(10);
  tft.fillScreen(TFT_BLACK);
  tft.setTextFont(2);
  canvas.setColorDepth(16);
  canvas.createSprite(tft.width(), tft.height());
  canvas.setTextFont(2);
  startWifiConfig();
  startConfigTime();
  setupOTAConfig();
}

void loop() {
  auto ms = millis();
  if (ms - check300ms > 300) {
    check300ms = ms;
    ArduinoOTA.handle();
    showCurrentTime();
  }
  if (ms - check5ms > 5) {
    check5ms = ms;
  }
}