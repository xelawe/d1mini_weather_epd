

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

void callback_mqtt_3(char* topic, byte* payload, unsigned int length) {
  DebugPrintln("Callback 3");

  float lv_wind = payload_to_float( payload, length);

  if ( gv_wind != lv_wind ) {
    gv_wind = lv_wind;
    // gv_ticked = true;
  }
}

void callback_mqtt_4(char* topic, byte* payload, unsigned int length) {
  DebugPrintln("Callback 4");

  float lv_waterl = payload_to_float( payload, length);

  if ( gv_waterl != lv_waterl ) {
    gv_waterl = lv_waterl;
    // gv_ticked = true;
  }
}

void callback_mqtt_5(char* topic, byte* payload, unsigned int length) {
  DebugPrintln("Callback 5");

  // Copy the payload to the new buffer
  memcpy(gv_payload_txt1, payload, length);
  gv_payload_txt1_length = length;

  // Free the memory
  //free(gv_payload);
}

void callback_mqtt_6(char* topic, byte* payload, unsigned int length) {
  DebugPrintln("Callback 6");

  gv_timestamp_mqtt = payload_to_time_t( payload, length);
  gv_timestamp_mqtt_ok = true;

}


void init_mqtt_local() {
  mqtt_pubtopic_temp_i_s += gv_clientname;
  mqtt_pubtopic_temp_i_s += '/';
  mqtt_pubtopic_temp_i_s += mqtt_pubtopic_temp_i_suff;
  mqtt_pubtopic_temp_i = (char*) mqtt_pubtopic_temp_i_s.c_str();
  DebugPrintln(mqtt_pubtopic_temp_i);

  mqtt_pubtopic_brght_i_s += gv_clientname;
  mqtt_pubtopic_brght_i_s += '/';
  mqtt_pubtopic_brght_i_s += mqtt_pubtopic_brght_i_suff;
  mqtt_pubtopic_brght_i = (char*) mqtt_pubtopic_brght_i_s.c_str();
  DebugPrintln(mqtt_pubtopic_brght_i);

  mqtt_pubtopic_btn1_s += gv_clientname;
  mqtt_pubtopic_btn1_s += '/';
  mqtt_pubtopic_btn1_s += mqtt_pubtopic_btn1_suff;
  mqtt_pubtopic_btn1 = (char*) mqtt_pubtopic_btn1_s.c_str();
  DebugPrintln(mqtt_pubtopic_btn1);

  mqtt_pubtopic_pir1_s += gv_clientname;
  mqtt_pubtopic_pir1_s += '/';
  mqtt_pubtopic_pir1_s += mqtt_pubtopic_pir1_suff;
  mqtt_pubtopic_pir1 = (char*) mqtt_pubtopic_pir1_s.c_str();
  DebugPrintln(mqtt_pubtopic_pir1);

  mqtt_subtopic_txt1_s += gv_clientname;
  mqtt_subtopic_txt1_s += '/';
  mqtt_subtopic_txt1_s += mqtt_subtopic_txt1_suff;
  mqtt_subtopic_txt1 = (char*) mqtt_subtopic_txt1_s.c_str();
  DebugPrintln(mqtt_subtopic_txt1);

  // Allocate the correct amount of memory for the payload copy
  gv_payload_txt1 = (byte*)malloc(MQTT_MAX_PACKET_SIZE);
  gv_payload_txt1_length = 0;

  init_mqtt(gv_clientname);

  add_subtopic(mqtt_subtopic_temp_a, callback_mqtt_1);
  add_subtopic(mqtt_subtopic_rain_h24, callback_mqtt_2);
  add_subtopic(mqtt_subtopic_wind, callback_mqtt_3);
  add_subtopic(mqtt_subtopic_waterl, callback_mqtt_4);
  add_subtopic(mqtt_subtopic_txt1, callback_mqtt_5);
  add_subtopic(mqtt_subtopic_timestamp, callback_mqtt_6);

}

void pub_vals() {

  if (gv_bme280_ok == true) {
    char buffer[10];
    dtostrf(gv_temp_bme280, 0, 1, buffer);
    client.publish(mqtt_pubtopic_temp_i, buffer, true);
  }

  if (gv_bh1750_ok == true) {
    char buffer[10];
    dtostrf(gv_lux, 0, 1, buffer);
    client.publish(mqtt_pubtopic_brght_i, buffer, true);
  }
}


void pub_btn1(boolean state) {
  int state_int;
  if (state == true) {
    client.publish(mqtt_pubtopic_btn1, "OFF", false);
  } else {
    client.publish(mqtt_pubtopic_btn1, "ON", false);
  }
}

void pub_pir1(boolean state) {
  int state_int;
  if (state == true) {
    client.publish(mqtt_pubtopic_pir1, "ON", false);
  } else {
    client.publish(mqtt_pubtopic_pir1, "OFF", false);
  }
}


