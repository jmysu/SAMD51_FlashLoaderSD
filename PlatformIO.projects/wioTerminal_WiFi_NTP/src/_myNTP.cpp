/*
 * NTP client
 * 
 * Update MbedTLS platform_tim.h 
 * //
 * //The time_t datatype
 * //
 *   #if defined(MBEDTLS_PLATFORM_TIME_TYPE_MACRO)
 *    typedef MBEDTLS_PLATFORM_TIME_TYPE_MACRO mbedtls_time_t;
 *   #else
 *    //* For time_t *
 *    //#include <time.h>
 *    //typedef time_t mbedtls_time_t;
 *    #include <TimeLib.h>
 *    #define mbedtls_time_t   time_t 
 *    #endif /* MBEDTLS_PLATFORM_TIME_TYPE_MACRO *
 */
#include <Arduino.h>
#include <TimeLib.h>
#include <rpcWiFi.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
// NTP Servers:
//IPAddress timeServer(132, 163, 4, 101); // time-a.timefreq.bldrdoc.gov
IPAddress timeServer(132, 163, 96, 3);  // extenral NTP server e.g. time.nist.gov
// IPAddress timeServer(132, 163, 4, 102); // time-b.timefreq.bldrdoc.gov
// IPAddress timeServer(132, 163, 4, 103); // time-c.timefreq.bldrdoc.gov

//const int timeZone = 1;     // Central European Time
//const int timeZone = -5;  // Eastern Standard Time (USA)
//const int timeZone = -4;  // Eastern Daylight Time (USA)
//const int timeZone = -8;  // Pacific Standard Time (USA)
//const int timeZone = -7;  // Pacific Daylight Time (USA)
const int timeZone = 8;  // Taiwan

// define WiFI client
WiFiClient client;
//The udp library class
WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

void connectToWiFi(const char* ssid, const char* pwd) {
    Serial.println("Connecting to WiFi network: " + String(ssid));

    // delete old config
    WiFi.disconnect(true);
    Serial.println("Waiting for WIFI connection...");

    //Initiate connection
    WiFi.begin(ssid, pwd);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    Serial.println("Connected.");
    //
    // print the SSID of the network you're attached to:
    Serial.println("");
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
    Serial.println("");
}

void printDigits(int digits) {
    // utility for digital clock display: prints preceding colon and leading 0
    Serial.print(":");
    if (digits < 10)
        Serial.print('0');
    Serial.print(digits);
}

extern void drawClockHands();
extern void drawClockDigits();
void digitalClockDisplay() {
    // digital clock display of the time
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.print(" ");

    Serial.print(month());
    Serial.print("-");
    Serial.print(day());
    Serial.print("-");
    Serial.print(year());
    Serial.println();

    drawClockDigits();
    drawClockHands();
}

/*-------- NTP code ----------*/
const int NTP_PACKET_SIZE = 48;      // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE];  //buffer to hold incoming & outgoing packets
// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress& address) {
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011;  // LI, Version, Mode
    packetBuffer[1] = 0;           // Stratum, or type of clock
    packetBuffer[2] = 6;           // Polling Interval
    packetBuffer[3] = 0xEC;        // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;
    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    Udp.beginPacket(address, 123);  //NTP requests are to port 123
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
}

time_t getNtpTime() {

    while (Udp.parsePacket() > 0)
        ;  // discard any previously received packets
    Serial.println("Transmit NTP Request");
    sendNTPpacket(timeServer);
    uint32_t beginWait = millis();
    while (millis() - beginWait < 3000) {
        int size = Udp.parsePacket();
        if (size >= NTP_PACKET_SIZE) {
            digitalClockDisplay();
            Serial.println("Receive NTP Response");
            Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
            unsigned long secsSince1900;
            // convert four bytes starting at location 40 to a long integer
            secsSince1900 = (unsigned long)packetBuffer[40] << 24;
            secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
            secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
            secsSince1900 |= (unsigned long)packetBuffer[43];

            setSyncInterval(60*60); //sync 1hour later
            Serial.println("resync at 60min later!");
            return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
        }
    }
    Serial.println("No NTP Response :-(");
    setSyncInterval(10); //resync 10seconds later
    Serial.println("resync at 10sec later!");
    return 0;  // return 0 if unable to get the time
}

extern void checkSdLaunch();
extern void setupTFT();
void setup() {
    checkSdLaunch();

    Serial.begin(115200);
    //while (!Serial) ; // Needed for Leonardo only
    //
    setupTFT();

    Serial.println("TimeNTP Example");
    connectToWiFi("BreezeHillXXX", "xxxxxxxxxx");

    digitalClockDisplay();
    Serial.println("waiting for sync");
    setSyncProvider(getNtpTime);
}

time_t timePrevious = 0;  // when the digital clock was displayed
timeStatus_t timeStatusOld;
extern bool isInitClock;
void loop() {
    if (timeStatusOld != timeStatus()) {
        timeStatusOld = timeStatus();
        isInitClock = 1;
    }
    if (timeStatus() == timeSet) {
        if (now() != timePrevious) {  //update the display only if time has changed
            timePrevious = now();
            digitalClockDisplay();
            isInitClock = 0;
        }
    }
}
