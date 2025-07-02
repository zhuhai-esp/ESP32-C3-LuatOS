#include <Arduino.h>
#include <OneButton.h>
#include <math.h>

#define PIN_LED1 GPIO_NUM_12
#define PIN_LED2 GPIO_NUM_13
#define PIN_BOOT GPIO_NUM_9
#define PIN_WAKEUP GPIO_NUM_0

OneButton btnBoot(PIN_BOOT, true);
uint64_t ms100ms = 0, ms10ms = 0, ms1s = 0;

inline void setupDevice() {
  Serial.begin(115200);
  Serial.println("Hello ESP32C3!!");
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  digitalWrite(PIN_LED1, HIGH);
  digitalWrite(PIN_LED2, HIGH);
  delay(200);
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);
}

inline void enterDeepSleep() {
  const gpio_config_t config = {
      .pin_bit_mask = BIT(PIN_WAKEUP),
      .mode = GPIO_MODE_INPUT,
  };
  gpio_config(&config);
  esp_deep_sleep_enable_gpio_wakeup(BIT(PIN_WAKEUP), ESP_GPIO_WAKEUP_GPIO_LOW);
  esp_deep_sleep_start();
}

inline void enterLightSleep() {
  // esp_sleep_enable_timer_wakeup(10 * 1000000);
  gpio_wakeup_enable(PIN_BOOT, GPIO_INTR_LOW_LEVEL);
  esp_sleep_enable_gpio_wakeup();
  Serial.println("Light Sleep Start!!");
  {
    digitalWrite(PIN_LED1, HIGH);
    delay(400);
    digitalWrite(PIN_LED1, LOW);
  }
  esp_light_sleep_start();
  Serial.println("Light Sleep Ended!!");
  {
    digitalWrite(PIN_LED2, HIGH);
    delay(400);
    digitalWrite(PIN_LED2, LOW);
  }
}

void setup() {
  setupDevice();
  btnBoot.attachClick([]() {
    // enterDeepSleep();
    enterLightSleep();
  });
}

void loop() {
  auto cur = millis();
  if (cur - ms10ms > 10) {
    ms10ms = cur;
    btnBoot.tick();
  }
  if (cur - ms100ms > 100) {
    ms100ms = cur;
  }
  if (cur - ms1s > 1000) {
    ms1s = cur;
    static u64_t rounds = 0;
    Serial.printf("App Running [%6d]!!\n", rounds++);
  }
}