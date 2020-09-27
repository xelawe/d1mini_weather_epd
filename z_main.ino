void setup()
{
  cy_serial::start(__FILE__);

  display.init();
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeSansBold9pt7b);
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

  init_bme280();

  delay(500);


  init_bh1750( );

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
    check_bh1750( );
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
