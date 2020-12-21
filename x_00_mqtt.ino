#include "cy_mqtt_v1.h"


float gv_temp = 0;
float gv_rain_h24 = 0;
float gv_wind = 0;
float gv_waterl = 0;

time_t gv_timestamp_mqtt;
boolean gv_timestamp_mqtt_ok = false;

byte* gv_payload_txt1;
unsigned int gv_payload_txt1_length;

const char* mqtt_subtopic_temp_a   = "ATSH28/AUSSEN/TEMP/1/value";
const char* mqtt_subtopic_rain_h24 = "ATSH28/AUSSEN/RAIN24H/1/value";
const char* mqtt_subtopic_wind     = "ATSH28/AUSSEN/WIND/1/value";
const char* mqtt_subtopic_waterl   = "ATSH28/AUSSEN/WATERLEVEL/1/value";
const char* mqtt_subtopic_date     = "ATSH28/DATE/full";
const char* mqtt_subtopic_timestamp = "ATSH28/DATE/timestamp";

const char* mqtt_pubtopic_temp_i_suff = "tele/temp";
String mqtt_pubtopic_temp_i_s;
const char* mqtt_pubtopic_temp_i;

const char* mqtt_subtopic_txt1_suff = "cmnd/txt1";
String mqtt_subtopic_txt1_s;
const char* mqtt_subtopic_txt1;

const char* mqtt_pubtopic_brght_i_suff = "tele/brightness";
String mqtt_pubtopic_brght_i_s;
const char* mqtt_pubtopic_brght_i;

const char* mqtt_pubtopic_btn1_suff = "tele/Button1";
String mqtt_pubtopic_btn1_s;
const char* mqtt_pubtopic_btn1;

const char* mqtt_pubtopic_pir1_suff = "tele/PIR1";
String mqtt_pubtopic_pir1_s;
const char* mqtt_pubtopic_pir1;
