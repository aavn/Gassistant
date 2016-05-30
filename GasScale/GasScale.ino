
#include <ArduinoJson.h>
#include "HX711.h"
#include <stdint.h>
#include "properties.h"
#include "json_util.h"
#include "loadcell_util.h"
#include "wifi_util.h"
#include "init_util.h"
#include "gasscale_service.h"
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include "rest_util.h"
Configuration configuration;
GasData gasData;


// the setup routine runs once when you press reset:
void setup() {
    Serial.begin(115200);
    Serial.println("Starting \n");
    Serial.println("setup...\n");
    //setup wifi
    initWifi();
    if(configuration.deviceId[0] == '\0'){
      computeWifiScale();
    }
    printConfig();
    initLoadCellsConf();
    syncTime();
    //TODO check the case setting is not avaible
    saveSettingInfoToMem();
    printDateTime();
}

// the loop routine runs over and over again forever:
void loop() {
  Main();
  delay (SCALE_IDLE_TIMEOUT);
}

