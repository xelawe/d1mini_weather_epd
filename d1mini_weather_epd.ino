/************************************************************************************
   Display Weather/Sensor Data on waveshare epaper Display with D1 Mini
   based on:

   GxEPD_SPI_TestExample : test example for e-Paper displays from Dalian Good Display Co., Ltd.: www.good-display.com

*/
#define USE_SERIAL Serial
#include <cy_serdebug.h>
#include <cy_serial.h>

const char *gc_hostname = "d1minwepd";
#include "cy_wifi.h"
#include "cy_ota.h"
#include "cy_mqtt.h"
#include "ntp_tool.h"
#include <Ticker.h>
#include "tools.h"
#include "epd_tool.h"
#include "http_tool.h"
#include "bme280_tool.h"


const char* mqtt_subtopic_temp_a   = "ATSH28/AUSSEN/TEMP/1/value";
const char* mqtt_subtopic_rain_h24 = "ATSH28/AUSSEN/RAIN24H/1/value";
float gv_temp = 0;
float gv_rain_h24 = 0;

Ticker ticker;
boolean gv_ticked = false;
Ticker ticker1;
boolean gv_ticked1 = false;

void tick()
{
  gv_ticked = true;
}

void tick1()
{
  gv_ticked1 = true;
}


void callback_mqtt_1(char* topic, byte* payload, unsigned int length) {
  DebugPrintln("Callback 1");

  float lv_temp = payload_to_float( payload, length);

  if ( lv_temp != gv_temp ) {
    gv_temp = lv_temp;
    // gv_ticked = true;
  }

}

void callback_mqtt_2(char* topic, byte* payload, unsigned int length) {
  DebugPrintln("Callback 2");

  float lv_rain_h24 = payload_to_float( payload, length);

  if ( gv_rain_h24 != lv_rain_h24 ) {
    gv_rain_h24 = lv_rain_h24;
    // gv_ticked = true;
  }

}

void setup()
{
  cy_serial::start(__FILE__);

  display.init();
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeMonoBold12pt7b);
  display.setCursor(0, 0);
  display.println();

  display.println("Wifi init ...");
  display.update();
  delay(500);
  wifi_init(gc_hostname);
 // display.println("Wifi OK");
  display.println("connected to " + WiFi.SSID() + " ...yeey");


  init_ota(gv_clientname);

  display.println("NTP init ...");
  display.update();
  delay(500);
  init_time();
  display.println("NTP OK");


  init_mqtt(gv_clientname);
  add_subtopic(mqtt_subtopic_temp_a, callback_mqtt_1);
  add_subtopic(mqtt_subtopic_rain_h24, callback_mqtt_2);

  display.println("BME280 init ...");
  display.update();
  delay(500);
  init_bme280();
  display.println("BME280 OK");
  display.update();
  delay(500);

  ticker.attach(60, tick);
  ticker1.attach(600, tick1);
  gv_ticked = true;
  gv_ticked1 = true;

}

void loop()
{

  check_ota();

  check_mqtt();

  check_time();

  if (gv_ticked == true ) {
    get_bme280();

    if (gv_ticked1 == true ) {

      get_draw_pic();

      gv_ticked1 = false;
      display.update();

    }

    print_vals("FreeMonoBold12pt7b", &FreeMonoBold12pt7b);
    //showFont("FreeMonoBold9pt7b", &FreeMonoBold9pt7b);
    //showFont("FreeMonoBold18pt7b", &FreeMonoBold18pt7b);
    //showFont("FreeMonoBold24pt7b", &FreeMonoBold24pt7b);

    gv_ticked = false;

  }

}

void print_vals(const char name[], const GFXfont* f)
{
  //display.fillScreen(GxEPD_WHITE);
  display.fillRect(0, 0, 400, 180, GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(f);
  display.setCursor(0, 0);
  display.println();

  String timenow = String(hour()) + ":" + twoDigits(minute());// + ":" + twoDigits(second());
  display.print(timenow);

  display.print(" ");
  display.print(dayStr(weekday()));
  display.print(" ");
  display.print(day());
  display.print(".");
  //display.print(monthStr(month()));
  display.print(month());
  display.print(".");
  display.print(year());
  display.println();

  display.drawFastHLine(0, 30, 400, GxEPD_BLACK);
  display.println();

  display.print("Temperatur a: ");
  display.print(gv_temp, 1);
  display.print(" *C");
  display.println();

  display.print("Niederschlag: ");
  display.print(gv_rain_h24, 0);
  display.print(" mm/d");
  display.println();

  display.print("Temperatur i: ");
  display.print(gv_temp_bme280, 1);
  display.print(" *C");
  display.println();

  display.print("Lufteuchtigk: ");
  display.print(gv_humi_bme280, 1);
  display.print(" %");
  display.println();



  display.updateWindow(0, 0, 400, 180, true);

}
