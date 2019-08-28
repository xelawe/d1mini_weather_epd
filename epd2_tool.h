// Display Library example for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: these e-papers require 3.3V supply AND data lines!
//
// Display Library based on Demo Example from Good Display: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

// Supporting Arduino Forum Topics:
// Waveshare e-paper displays with SPI: http://forum.arduino.cc/index.php?topic=487007.0
// Good Dispay ePaper for Arduino: https://forum.arduino.cc/index.php?topic=436411.0

// mapping suggestion from Waveshare SPI e-Paper to Wemos D1 mini
// BUSY -> D2, RST -> D4, DC -> D3, CS -> D8, CLK -> D5, DIN -> D7, GND -> GND, 3.3V -> 3.3V
// NOTE: connect 4.7k pull-down from D8 to GND if your board or shield has level converters
// NOTE for ESP8266: using SS (GPIO15) for CS may cause boot mode problems, use different pin in case, or 4k7 pull-down

#ifndef EPD2_TOOL_H
#define EPD2_TOOL_H

// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>




#define EPD_CS SS

#if defined (ESP8266)
// select one and adapt to your mapping, can use full buffer size (full HEIGHT)
GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> display(GxEPD2_420(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));

// 3-color e-papers
// can use only half buffer size
//GxEPD2_3C<GxEPD2_420c, GxEPD2_420c::HEIGHT / 2> display(GxEPD2_420c(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));

#endif

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
//#include <Fonts/FreeMonoBold18pt7b.h>
//#include <Fonts/FreeMonoBold24pt7b.h>

bool hasRed = false;



/** Draw the pixels to the screen
 *  
 *  @param  char *data    A char array
 *  @param  size_t len    Length of the char array
 *  @param  boolean start True if the begin of a new screen
 * 
 */
void drawPixels(char *data, size_t len, boolean start){
  static int x;
  static int y;
  if (start){
    x = 0;
    y = 0;
    // Not required
    //display.eraseDisplay(true);
  }

  Serial.println(String("Printing ") + len + " Bytes to the screen");
  for (size_t i=0; i<len; i++){

    if (hasRed == true) {
      for (int b = 7; b >= 0; b -= 2) {
        int bit = bitRead(data[i], b);
        int bit2 = bitRead(data[i], b - 1);

        if ((bit == 1) && (bit2 == 1)) {
          display.drawPixel(x, y, GxEPD_BLACK);
        } else {
          if ((bit == 0) && (bit2 == 1)) {
            display.drawPixel(x, y, GxEPD_RED);
          } else {
            display.drawPixel(x, y, GxEPD_WHITE);
          }
        }
        x++;

        if  (x == WIDTH) {
          y++;
          x = 0;
        }
      }
    } else {  // hasRead
      for (int b = 7; b >= 0; b--) {
        int bit = bitRead(data[i], b);
        if (bit == 1) {
          display.drawPixel(x, y, GxEPD_BLACK);
        } else {
          display.drawPixel(x, y, GxEPD_WHITE);
        }
        x++;
        if  (x == WIDTH) {
          y++;
          x = 0;
        }
      }
    }
  }
}
#endif
