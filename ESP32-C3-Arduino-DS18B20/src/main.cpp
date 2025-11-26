#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define PIN_LED1 12
#define PIN_LED2 13

#define ONE_WIRE_BUS 9

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress addr;

void setup() {
  Serial.begin(115200);
  Serial.println("Hello ESP32C3!!");
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);

  sensors.begin();
}

void printAddress(DeviceAddress deviceAddress) {
  Serial.print("Addr: 0X");
  for (uint8_t i = 0; i < 8; i++) {
    Serial.printf("%X", deviceAddress[i]);
  }
  Serial.println();
}

inline void readSensorData() {
  sensors.requestTemperatures();
  auto count = sensors.getDeviceCount();
  for (uint8_t i = 0; i < count; i++) {
    float tempC = sensors.getTempCByIndex(i);
    sensors.getAddress(addr, i);
    if (tempC != DEVICE_DISCONNECTED_C) {
      printAddress(addr);
      Serial.printf("[%u]TEMP: [%.2f]\n", i, tempC);
    }
  }
}

void loop() {
  digitalWrite(PIN_LED1, HIGH);
  digitalWrite(PIN_LED2, LOW);
  delay(200);
  digitalWrite(PIN_LED2, HIGH);
  digitalWrite(PIN_LED1, LOW);
  delay(200);
  readSensorData();
}