#include <Time.h> //http://www.arduino.cc/playground/Code/Time
#include <TimeLib.h> // https://github.com/PaulStoffregen/Time
#include <TimeAlarms.h>
#include <Timezone.h>    //https://github.com/JChristensen/Timezone

#include <WiFiUdp.h>


/* Don't hardwire the IP address or we won't get the benefits of the pool.
    Lookup the IP address for the host name instead */
IPAddress timeServerIP; // time.nist.gov NTP server address
//const char* ntpServerName = "time.nist.gov";
const char* ntpServerName = "at.pool.ntp.org";

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
unsigned int localPort = 2390;      // local port to listen for UDP packets
const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

int SyncInt = 300;
const int timeZone = 0;     // UTC

// Central European Time Zone (Vienna)
TimeChangeRule myDST = {"CEDT", Last, Sun, Mar, 2, 120};    //Daylight time = UTC - 4 hours
TimeChangeRule mySTD = {"CET", Last, Sun, Oct, 3,  60};     //Standard time = UTC - 5 hours
Timezone myTZ(myDST, mySTD);

TimeChangeRule *tcr;        //pointer to the time change rule, use to get TZ abbrev


