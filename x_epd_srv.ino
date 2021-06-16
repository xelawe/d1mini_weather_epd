void print_vals()
{
  //display.fillScreen(GxEPD_WHITE);
  display.fillRect(0, 0, 400, 180, GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  //display.setFont(f);
  //display.setFont(&FreeMonoBold12pt7b);
  display.setFont(&FreeSansBold12pt7b);
  display.setCursor(0, 22);
  // display.println();

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

  //display.setFont(&FreeMonoBold9pt7b);
  display.setFont(&FreeSansBold9pt7b);
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

  //display.setFont(&FreeMonoBold9pt7b);
  display.setFont(&FreeSansBold9pt7b);

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


  //display.setFont(&FreeMonoBold9pt7b);
  display.setFont(&FreeSansBold9pt7b);
  display.println();

  display.println();
  display.print("innen: ");
  //  display.print( lv_now_utc );
  //  display.print( " - " );
  //  display.print( gv_timestamp_mqtt );

  if ( gv_bh1750_ok == true ) {
    display.print("Brght: ");
    display.print(gv_lux, 1);
    display.print(" lx");
  }

  display.println();


  display.setFont(&FreeMonoBold12pt7b);

  if ( gv_bme280_ok == true ) {
    display.print("Temp: ");
    display.print(gv_temp_bme280, 1);
    display.setFont(&FreeSansBold9pt7b);
    display.print(" *C");

    display.setFont(&FreeMonoBold12pt7b);

    //display.println();

    display.print(" LF: ");
    display.print(gv_humi_bme280, 1);
    display.setFont(&FreeSansBold9pt7b);
    display.print(" %");
    display.println();
  }

  display.setFont(&FreeSansBold9pt7b);
  if (gv_payload_txt1_length > 0 ) {
    for (int i = 0; i < gv_payload_txt1_length; i++) {
      display.print((char)gv_payload_txt1[i]);
    }
    display.println();
  }

  //display.updateWindow(0, 0, 400, 180, true);

}



//void showBitmapExample()
//{
//
//  //display.drawExampleBitmap(BitmapExample1, sizeof(BitmapExample1));
//  //delay(2000);
//  display.drawExampleBitmap(BitmapExample2, sizeof(BitmapExample2));
//  delay(5000);
//  //display.fillScreen(GxEPD_WHITE);
//  //display.drawExampleBitmap(BitmapExample1, 0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_BLACK);
//  display.update();
//
//}


/** Draw the pixels to the screen

    @param  char *data    A char array
    @param  size_t len    Length of the char array
    @param  boolean start True if the begin of a new screen

*/
void drawPixels(char *data, size_t len, boolean start) {
  static int x;
  static int y;
  if (start) {
    x = 0;
    y = 0;
    // Not required
    //display.eraseDisplay(true);
  }

  Serial.println(String("Printing ") + len + " Bytes to the screen");
  for (size_t i = 0; i < len; i++) {

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

        if  (x == GxEPD_WIDTH) {
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
        if  (x == GxEPD_WIDTH) {
          y++;
          x = 0;
        }
      }
    }
  }
}
