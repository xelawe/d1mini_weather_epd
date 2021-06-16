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

#include <Ticker.h>

int gv_min = 0;

Ticker ticker1;
volatile boolean gv_ticked1 = false;

void ICACHE_RAM_ATTR tick1()
{
  gv_ticked1 = true;
}


void init_ser(){
    cy_serial::start(__FILE__);
}
