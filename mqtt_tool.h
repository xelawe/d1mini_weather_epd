
#include "cy_mqtt.h"

const char* mqtt_subtopic_temp_a   = "ATSH28/AUSSEN/TEMP/1/value";
const char* mqtt_subtopic_rain_h24 = "ATSH28/AUSSEN/RAIN24H/1/value";
const char* mqtt_subtopic_wind     = "ATSH28/AUSSEN/WIND/1/value";
const char* mqtt_subtopic_waterl   = "ATSH28/AUSSEN/WATERLEVEL/1/value";
const char* mqtt_subtopic_date     = "ATSH28/DATE/full";
const char* mqtt_subtopic_timestamp = "ATSH28/DATE/timestamp";

const char* mqtt_pubtopic_temp_i_suff = "tele/temp";
String mqtt_pubtopic_temp_i_s;
const char* mqtt_pubtopic_temp_i;

//const char* mqtt_pubtopic_lux_suff = "tele/lux";
//String mqtt_pubtopic_lux_s;
//const char* mqtt_pubtopic_lux;

void init_mqtt_local() {
  mqtt_pubtopic_temp_i_s += gv_clientname;
  mqtt_pubtopic_temp_i_s += '/';
  mqtt_pubtopic_temp_i_s += mqtt_pubtopic_temp_i_suff;
  mqtt_pubtopic_temp_i = (char*) mqtt_pubtopic_temp_i_s.c_str();
  DebugPrintln(mqtt_pubtopic_temp_i);

  init_mqtt(gv_clientname);
  
}

void pub_vals() {

  char buffer[10];
  dtostrf(gv_temp_bme280, 0, 1, buffer);
  client.publish(mqtt_pubtopic_temp_i, buffer, true);
  
}
