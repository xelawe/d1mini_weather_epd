/*-------- NTP code ----------*/
// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

time_t getNtpTime()
{

  //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP);

  //DebugPrintln("Starting UDP");
  //Udp.begin(localPort);
  //Serial.print("Local port: ");
  //Serial.println(Udp.localPort());

  DebugPrintln("waiting for UDP sync");

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  DebugPrintln("Transmit NTP Request");

  sendNTPpacket(timeServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      DebugPrintln("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      //return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
      // calculate UTC
      time_t lv_time_utc = secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;

      // return CET / CEST
      //return  myTZ.toLocal(lv_time_utc, &tcr);

      // return UTC
      return lv_time_utc;
    }
  }
  DebugPrintln("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

void check_time() {
  if ( timeStatus() != timeSet ) {
    if (SyncInt != 5) {
      SyncInt = 5;
      setSyncInterval(SyncInt);
    }
  } else {
    if (SyncInt != 600) {
      SyncInt = 600;
      setSyncInterval(SyncInt);
    }
  }
}

void init_time() {


  DebugPrintln("Starting UDP");
  Udp.begin(localPort);

  setSyncProvider(getNtpTime);
  check_time();
  while (timeStatus() == timeNotSet) { // wait until the time is set by the sync provider
    delay(2000);
  }

  //reset_drift();

  DebugPrintln("done NTP");
}

