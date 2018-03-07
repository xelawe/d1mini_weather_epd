/************************************************************************************
   GxEPD_SPI_TestExample : test example for e-Paper displays from Dalian Good Display Co., Ltd.: www.good-display.com

   based on Demo Example from Good Display, now available on http://www.good-display.com/download_list/downloadcategoryid=34&isMode=false.html

   Author : J-M Zingg

   Version : 2.0

   Support: limited, provided as example, no claim to be fit for serious use

   connection to the e-Paper display is through DESTM32-S2 connection board, available from Good Display

*/


#include <cy_serdebug.h>
#include <cy_serial.h>

const char *gc_hostname = "d1minwepd";
#include "cy_wifi.h"
#include "cy_ota.h"
#include "cy_mqtt.h"
#include "ntp_tool.h"
#include <Ticker.h>

// Supporting Arduino Forum Topics:
// Waveshare e-paper displays with SPI: http://forum.arduino.cc/index.php?topic=487007.0
// Good Dispay ePaper for Arduino : https://forum.arduino.cc/index.php?topic=436411.0

// mapping suggestion from Waveshare SPI e-Paper to Wemos D1 mini
// BUSY -> D2, RST -> D4, DC -> D3, CS -> D8, CLK -> D5, DIN -> D7, GND -> GND, 3.3V -> 3.3V

// include library, include base class, make path known
#include <GxEPD.h>

// select the display class to use, only one
#include <GxGDEW042T2/GxGDEW042T2.cpp>      // 4.2" b/w
//#include <GxGDEW042Z15/GxGDEW042Z15.cpp>    // 4.2" b/w/r

#include GxEPD_BitmapExamples

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>


#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

#if defined(ESP8266)

// generic/common.h
//static const uint8_t SS    = 15;
//static const uint8_t MOSI  = 13;
//static const uint8_t MISO  = 12;
//static const uint8_t SCK   = 14;
// pins_arduino.h
//static const uint8_t D8   = 15;
//static const uint8_t D7   = 13;
//static const uint8_t D6   = 12;
//static const uint8_t D5   = 14;

// GxIO_SPI(SPIClass& spi, int8_t cs, int8_t dc, int8_t rst = -1, int8_t bl = -1);
GxIO_Class io(SPI, SS, 0, 2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
// GxGDEP015OC1(GxIO& io, uint8_t rst = 2, uint8_t busy = 4);
GxEPD_Class display(io); // default selection of D4(=2), D2(=4)
#endif


const char* mqtt_subtopic_temp_a = "ATSH28/AUSSEN/TEMP/1/value";
float gv_temp = 0;
Ticker ticker;
boolean gv_ticked = false;


void tick()
{
  gv_ticked = true;
}

void callback_mqtt1(char* topic, byte* payload, unsigned int length) {
  DebugPrintln("Callback 1");

  float lv_temp = 0;
  float lv_mul = 1;
  float lv_fact = 0;

  for (int i = 0; i < length; i++) {
    DebugPrint((char)payload[i]);
  }

  for (int i = 0; i < length; i++) {
    if ( payload[i] != 46 ) { // "."
      lv_temp = (lv_temp * lv_mul) + payload[i] - 48;
      lv_mul = lv_mul * 10;
      lv_fact = lv_fact * 10;
    } else {
      lv_fact = 1;
    }
  }

  if (lv_fact > 0) {
    lv_temp = lv_temp / lv_fact;
  }

  DebugPrintln("");
  DebugPrintln(lv_temp);

  if ( lv_temp != gv_temp ) {
    gv_temp = lv_temp;
    gv_ticked = true;
  }

}

void setup()
{
  cy_serial::start(__FILE__);

  display.init();

  wifi_init(gc_hostname);

  init_ota(gv_clientname);

  init_time();

  init_mqtt(gv_clientname);
  add_subtopic("ATSH28/AUSSEN/TEMP/1/value", callback_mqtt1);

  ticker.attach(60, tick);
  gv_ticked = true;
}

void loop()
{

  check_ota();

  check_mqtt();

  check_time();

  if (gv_ticked == true ) {

    //showBitmapExample();
    //delay(2000);

    //drawCornerTest();
    //showFont("FreeMonoBold9pt7b", &FreeMonoBold9pt7b);
    showFont("FreeMonoBold12pt7b", &FreeMonoBold12pt7b);
    //showFont("FreeMonoBold18pt7b", &FreeMonoBold18pt7b);
    //showFont("FreeMonoBold24pt7b", &FreeMonoBold24pt7b);
    gv_ticked = false;

  }
  //delay(10000);
}


#if defined(_GxGDEW042T2_H_) || defined(_GxGDEW042T2_FPU_H_)
void showBitmapExample()
{

  //display.drawExampleBitmap(BitmapExample1, sizeof(BitmapExample1));
  //delay(2000);
  display.drawExampleBitmap(BitmapExample2, sizeof(BitmapExample2));
  delay(5000);
  //display.fillScreen(GxEPD_WHITE);
  //display.drawExampleBitmap(BitmapExample1, 0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_BLACK);
  display.update();

}
#endif


// utility function for digital clock display: prints leading 0
String twoDigits(int digits) {
  if (digits < 10) {
    String i = '0' + String(digits);
    return i;
  }
  else {
    return String(digits);
  }
}

void showFont(const char name[], const GFXfont* f)
{
  display.fillScreen(GxEPD_WHITE);
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

  display.print("Aussentemperatur: ");
  display.print(gv_temp);
  display.print(" ");
  display.print("\xB0");
  display.println("C");

  display.update();

}
