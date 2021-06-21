
void init_bme280() {
  bool status;

  gv_bme280_ok = false;

  display.println("BME280 init ...");
  display.update();
  delay(500);

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  
  Wire.begin(4, 5); // SDA, SCL - vorher: Wire.begin(12, 5);

  status = bme.begin(0x76, &Wire);
  if (!status) {
    display.println("Could not find a valid BME280 sensor!");
    display.update();
    //while (1) {
    // delay(100);
    // }
  } else {
    gv_bme280_ok = true;
    display.println("BME280 OK");
  }
}

void get_bme280() {
  if (!gv_bme280_ok) {
    return;
  }
  gv_temp_bme280 = bme.readTemperature();
  gv_humi_bme280 = bme.readHumidity();
}

