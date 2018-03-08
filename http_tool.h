#include <ESP8266HTTPClient.h>

void get_draw_pic() {

  boolean lv_start = true;

  HTTPClient http;

  USE_SERIAL.print("[HTTP] begin...\n");

  // configure server and url
  http.begin("http://93764278.net/ct_epd/?debug=false&display=4.2&content=rep_day_1");


  USE_SERIAL.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK) {

      // get lenght of document (is -1 when Server sends no Content-Length header)
      int len = http.getSize();

      // create buffer for read
      uint8_t buff[128] = { 0 };

      // get tcp stream
      WiFiClient * stream = http.getStreamPtr();

      // read all data from server
      while (http.connected() && (len > 0 || len == -1)) {
        // get available data size
        size_t size = stream->available();

        if (size) {
          // read up to 128 byte
          int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

          // write it to Serial
          //USE_SERIAL.write(buff, c);

          drawPixels((char *)buff, c, lv_start);
          lv_start = false;

          if (len > 0) {
            len -= c;
          }
        }
        delay(1);
      }

      USE_SERIAL.println();
      USE_SERIAL.print("[HTTP] connection closed or file end.\n");

    }
  } else {
    USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void get_draw_pic_old() {
  static int x;
  static int y;
  x = 0;
  y = 0;

  display.fillScreen(GxEPD_WHITE);

  HTTPClient http;

  USE_SERIAL.print("[HTTP] begin...\n");

  // configure server and url
  http.begin("http://93764278.net/ct_epd/?debug=false&display=4.2&content=rep_day_1");


  USE_SERIAL.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK) {

      // get lenght of document (is -1 when Server sends no Content-Length header)
      int len = http.getSize();

      // create buffer for read
      uint8_t buff[128] = { 0 };

      // get tcp stream
      WiFiClient * stream = http.getStreamPtr();

      // read all data from server
      while (http.connected() && (len > 0 || len == -1)) {
        // get available data size
        size_t size = stream->available();

        if (size) {
          // read up to 128 byte
          int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

          // write it to Serial
          //USE_SERIAL.write(buff, c);


          for (int j = 0; j < c; j++) {


            for (int b = 7; b >= 0; b--) {
              int bit = bitRead(buff[j], b);
              if (bit == 1) {
                display.drawPixel(x, y, GxEPD_BLACK);
                //Serial.print("X");
              } else {
                display.drawPixel(x, y, GxEPD_WHITE);
                //Serial.print(" ");
              }
              x++;
              if  (x == GxEPD_WIDTH) {
                y++;
                x = 0;
                //Serial.println();
              }

            }
          }

          if (len > 0) {
            len -= c;
          }
        }
        delay(1);
      }

      USE_SERIAL.println();
      USE_SERIAL.print("[HTTP] connection closed or file end.\n");

    }
  } else {
    USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}
