#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "properties.h"
#include "wifi_util.h"
#include <ESP8266WiFi.h>
const char* ssid     = "IoT";
const char* password = "IoT@AAVN";

void initWifi(void)
{
  startSmartConfig();
  Serial.println("");
  Serial.print("WiFi connected to ");  Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");Serial.println(WiFi.macAddress()); 
  delay(500); 
   
}
void startSmartConfig(){
 int cnt = 0;
  // Connect with mode STA
  WiFi.mode(WIFI_STA); delay(1000);
  //configure pin0
  // GPIO0 developing set to GND, this mean isSmartConfig = 0
  // When run stand alone, remove GPIO0 out of GND, this mean isSmartConfig = 1
  // We will set GPIO0 to switch, switch connect to GND when we want run Smart Config, and not connect to GND when run alone.
  
  //pinMode(0, INPUT_PULLUP);
   // deplay for 15 sec for smartConfig
  /*Serial.println("15 sec before clear SmartConfig");
  Serial.println("If you want config with new wifi");
  Serial.println("Please connect GPIO0 to GND");
  delay(15000);   // Debug set 5s
  
  // read pullup
  int isSmartConfig = digitalRead(0);
  // When developing, set condition == 1 to skip check wait Smart Config, set after config success, if run stand alone set isSmartConfig == 0
  if (isSmartConfig == 1) {   
    // bink for clear config
    blinkClearConfig();
    Serial.println("clear config");
    // reset default config
    WiFi.disconnect();
  }
*/
  // if wifi cannot connect start smartconfig
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if(cnt++ >= 15){
       WiFi.beginSmartConfig();
       while(1){
           delay(500);
           if(WiFi.smartConfigDone()){
             Serial.println();
             Serial.println("SmartConfig Success");
             blinkSmartConfig();
             break;
           }
       }
    }
  }

  Serial.println("");
  Serial.println("");
  
  WiFi.printDiag(Serial);

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void blinkSmartConfig() {
    digitalWrite(STATUS_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);              // wait for a second 
    digitalWrite(STATUS_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(500);
}

void blinkClearConfig() {
  int i=0;
  while(i<=3) {
    digitalWrite(STATUS_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second 
    digitalWrite(STATUS_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);
    i++;
  }
}

