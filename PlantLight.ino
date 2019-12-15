/**
   LED wifi controller for pixie lights on a house plant (mini Christmas tree!)
   4 strands, each with PWM control using a 3v<==>5v line driver module (4 mosfets)
   see 'led' module for LED blink sequence 'program' configuration.

   BOARD:  NodeMCU 0.9 (ESP-12 Module)
   Flash Size: 4M (FS: none)

   TODO: add interactive buttons/settings in web interface.
   TODO: add A0 input with CdS LDR sensor to measure room lighting and auto-adjust brightness of lights.
   DONE: add MQTT interface api
*/

// for web
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// for mqtt
//#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>

#include <ArduinoOTA.h>

#include "password.h" // const char* ssid, password (for local Wifi)

const char * hostName = "plantlight";
const char * mqttTopic = "local";
const char * compiledDate = __DATE__;
const char * compiledTime = __TIME__;

//#define BUILTIN_LED  2  // ESP12 led on gpio2, aka TX?

long now, secs;
long curr_ticks, prev_ticks;
long flash_ticks;

// single LED channel
typedef struct ULedChan {
  Ticker ledTicker;
  int ledPin;
  int ledIndex, ledProgram;
  //int bright_pct;
} chan_t;

// 4 led channels in this build.
#define LED_CHAN_MAX  4 // number of pins for LEDs
chan_t chans[LED_CHAN_MAX];


void setup() {
  delay(200);
  WiFi.disconnect();
  delay(1000);

  //  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output

  //Serial.begin(115200);
  Serial.begin(74880);
  delay(100);
  //Serial.setDebugOutput(true);

  //  espid = String(ESP.getChipId());
  Serial.print("espid: ");
  Serial.println(ESP.getChipId(), HEX);

  wifi_setup();
  //  mqtt_setup();
  web_setup();
  led_setup();

  //  fixarp_setup();  // YUCK! HACK -- send gratuitous ARP periodically

  ArduinoOTA.setHostname(hostName);
  delay(100);
  ArduinoOTA.begin();

  Serial.println("setup done");
}

void loop() {
  now = millis();

  curr_ticks = now / 100;  // use 100 msec interval
  if (prev_ticks != curr_ticks) {
    if ( (curr_ticks - flash_ticks) > 20) {
      Serial.println( now ); // each 2 secs
      flash_ticks = curr_ticks;
    }
    secs = curr_ticks / 10;
    prev_ticks = curr_ticks;
  }

  // support OTA first 20 mins
  if (now < (20 * 60 * 1000) ) {
    ArduinoOTA.handle();
  }

  MDNS.update();

  //WiFi.update(true);
  //WiFi.handle();

  //  led_update();
}
