#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

float gv_temp_bme280;
float gv_humi_bme280;

boolean gv_bme280_ok;

