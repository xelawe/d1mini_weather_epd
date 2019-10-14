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

#include "ntp_tool.h"
#include <Ticker.h>
#include "tools.h"
#include "epd_tool.h"
#include "http_tool.h"
#include "bme280_tool.h"

#include "mqtt_tool.h"


int gv_min = 0;



Ticker ticker1;
boolean gv_ticked1 = false;

void tick1()
{
  gv_ticked1 = true;
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
  display.println( WiFi.localIP() );
  //display.println("IP " + WiFi.localIP() );

  init_ota(gv_clientname);

  display.println("NTP init ...");
  display.update();
  delay(500);
  init_time();
  display.println("NTP OK");


  //init_mqtt(gv_clientname);
  init_mqtt_local();


  display.println("BME280 init ...");
  display.update();
  delay(500);
  init_bme280();
  display.println("BME280 OK");
  display.update();
  delay(500);

  ticker1.attach(600, tick1);
  gv_ticked1 = true;

}

void loop()
{

  check_ota();

  check_mqtt();
  if ( gv_mqtt_conn_ok != true ) {
    ESP.restart();
    delay(2000);
  }

  check_time();

  if (gv_min != minute() ) {
    gv_min = minute();

    get_bme280();
    pub_vals();


    if (gv_ticked1 == true ) {

      get_draw_pic();

      gv_ticked1 = false;

    }

    print_vals();
    //print_vals("FreeMonoBold12pt7b", &FreeMonoBold12pt7b);
    //showFont("FreeMonoBold9pt7b", &FreeMonoBold9pt7b);
    //showFont("FreeMonoBold18pt7b", &FreeMonoBold18pt7b);
    //showFont("FreeMonoBold24pt7b", &FreeMonoBold24pt7b);
    display.update();
  }

}

void print_vals()
{
  //display.fillScreen(GxEPD_WHITE);
  display.fillRect(0, 0, 400, 180, GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  //display.setFont(f);
  display.setFont(&FreeMonoBold12pt7b);
  display.setCursor(0, 0);
  display.println();

  time_t lv_now_utc = now();
  time_t lv_now =  myTZ.toLocal(lv_now_utc, &tcr);


  String timenow = String(hour(lv_now)) + ":" + twoDigits(minute(lv_now));// + ":" + twoDigits(second(lv_now));
  display.print(timenow);

  display.print(" ");
  display.print(dayStr(weekday(lv_now)));
  display.print(" ");
  display.print(day(lv_now));
  display.print(".");
  //display.print(monthStr(month()));
  display.print(month(lv_now));
  display.print(".");
  display.print(year(lv_now));

  display.setFont(&FreeMonoBold9pt7b);
  display.print(" ");
  if ( gv_mqtt_conn_ok == true ) {
    display.print("=");
  } else {
    display.print("!");
  }

  if ( gv_timestamp_mqtt_ok == true ) {
    time_t lv_time_diff = lv_now_utc - gv_timestamp_mqtt;
    display.print( lv_time_diff);
    if ( ( lv_time_diff > 600 ) || (lv_time_diff < -600 ) ) {
      display.print("!");
    } else {
      display.print("=");
    }
  }
  display.println();

  display.drawFastHLine(0, 30, 400, GxEPD_BLACK);

  display.setFont(&FreeMonoBold9pt7b);

  //display.println();

  display.print("aussen: ");
  //  for (int i = 0; i < gv_payload_date_length; i++) {
  //    display.print( (char)gv_payload_date[i] );
  //  }
  display.println();

  display.setFont(&FreeMonoBold12pt7b);

  display.print("Temp: ");
  display.print(gv_temp, 1);
  display.print(" *C");
  //display.println();

  display.print(" NS: ");
  display.print(gv_rain_h24, 0);
  display.print(" mm/d");
  display.println();

  display.print("Wind: ");
  display.print(gv_wind, 0);
  display.print(" km/h");
  //display.println();

  display.print(" WS: ");
  display.print(gv_waterl, 0);
  display.print(" mm");
  display.println();

  display.setFont(&FreeMonoBold9pt7b);

  display.println();
  display.print("innen: ");
  //  display.print( lv_now_utc );
  //  display.print( " - " );
  //  display.print( gv_timestamp_mqtt );
  display.println();


  display.setFont(&FreeMonoBold12pt7b);

  display.print("Temp: ");
  display.print(gv_temp_bme280, 1);
  display.print(" *C");
  //display.println();

  display.print(" LF: ");
  display.print(gv_humi_bme280, 1);
  display.print(" %");
  display.println();

  if (gv_payload_txt1_length > 0 ) {
    for (int i = 0; i < gv_payload_txt1_length; i++) {
      display.print((char)gv_payload_txt1[i]);
    }
    display.println();
  }

  //display.updateWindow(0, 0, 400, 180, true);

}
