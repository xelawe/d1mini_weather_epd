/*
   based on Demo Example from Good Display, now available on http://www.good-display.com/download_list/downloadcategoryid=34&isMode=false.html
   Author : J-M Zingg
   Version : 2.0
   Support: limited, provided as example, no claim to be fit for serious use
   connection to the e-Paper display is through DESTM32-S2 connection board, available from Good Display

  // Supporting Arduino Forum Topics:
  // Waveshare e-paper displays with SPI: http://forum.arduino.cc/index.php?topic=487007.0
  // Good Dispay ePaper for Arduino : https://forum.arduino.cc/index.php?topic=436411.0


*/
//#ifndef EPD_TOOL_H
//#define EPD_TOOL_H

// include library, include base class, make path known
#include <GxEPD.h>

// select the display class to use, only one
#include <GxGDEW042T2/GxGDEW042T2.h>      // 4.2" b/w
//#include <GxGDEW042Z15/GxGDEW042Z15.cpp>    // 4.2" b/w/r

#include GxEPD_BitmapExamples

// FreeFonts from Adafruit_GFX
//#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
//#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
//#include <Fonts/FreeMonoBold18pt7b.h>
//#include <Fonts/FreeMonoBold24pt7b.h>


//#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
//#include <GxIO/GxIO.cpp>
#include <GxIO/GxIO.h>

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

  // mapping suggestion from Waveshare SPI e-Paper to Wemos D1 mini
  // BUSY -> D6 (GPIO12) <<- MISO (war vorher D2 (GPIO4) <<-SDA )
  // RST  -> D4 (GPIO2) <<- LED
  // DC   -> D3 (GPIO0)
  // CS   -> D8 (GPIO15, SS)
  // CLK  -> D5 
  // DIN  -> D7 
  // GND  -> GND 
  // 3.3V -> 3.3V
  
// GxIO_SPI(SPIClass& spi, int8_t cs, int8_t dc, int8_t rst = -1, int8_t bl = -1);
GxIO_Class io(SPI, SS, 0, 2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
// GxGDEP015OC1(GxIO& io, uint8_t rst = 2, uint8_t busy = 4);
GxEPD_Class display(io, 2, 12); // default selection of D4(=2), D2(=4) --> D4(=2), D6 (=12)
#endif
bool hasRed = false;

//#endif
