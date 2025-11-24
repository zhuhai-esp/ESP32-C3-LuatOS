#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <Arduino.h>

#define PIN_LED1 12
#define PIN_LED2 13

// static const uint8_t SDA = 4;
// static const uint8_t SCL = 5;
Adafruit_BMP280 bmp;
Adafruit_AHTX0 aht;

inline void initBMP280() {
  unsigned status;
  status = bmp.begin();
  if (!status) {
    Serial.println("=============================");
    Serial.println(F("No Device BMP280 found!"));
    Serial.print("SensorID was: 0x");
    Serial.println(bmp.sensorID(), 16);
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    Serial.println("=============================");
    while (1)
      delay(10);
  }
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

inline void printBMP280() {
  Serial.println("=============================");
  Serial.print(F("BMP280 Temperature = "));
  Serial.print(bmp.readTemperature());
  Serial.println(" ℃");

  Serial.print(F("BMP280 Pressure = "));
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  Serial.print(F("BMP280 Approx altitude = "));
  /* Adjusted to local forecast! */
  Serial.print(bmp.readAltitude(1013.25));
  Serial.println(" m");
  Serial.println("=============================");
}

inline void initAHT20() {
  if (!aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1)
      delay(10);
  }
}

inline void printAHT20() {
  Serial.println("=============================");
  sensors_event_t humidity, temp;
  // populate temp and humidity objects with fresh data
  aht.getEvent(&humidity, &temp);
  Serial.print("AHT20 Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" ℃");
  Serial.print("AHT20 Humidity: ");
  Serial.print(humidity.relative_humidity);
  Serial.println("% rH");
  Serial.println("=============================");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Hello ESP32C3!!");
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);
  initBMP280();
  initAHT20();
}

void loop() {
  printBMP280();
  printAHT20();
  delay(5000);
}