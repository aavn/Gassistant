#include "properties.h"
#include "init_util.h"
#include <TimeLib.h>
#include <ESP8266WiFi.h>

void computeWifiScale(void){
   int chipId = 0;
   int n=0;
   chipId = ESP.getChipId();
   n=sprintf (configuration.deviceId, "%d",chipId);
   n=sprintf (configuration.deviceNm, "%d",chipId);
   Serial.print("\nGenerated ID: " );
   Serial.println(configuration.deviceId);
}

void useDefaultSetting(void){
    configuration.deviceId[0] = '\0';
    configuration.deviceNm[0] = '\0';

    configuration.tankWgt =  DEFAULT_TANK_WGT;
    configuration.totalWgt = DEFAULT_GROSS_WGT;
    configuration.riskDuration = DEFAULT_RISK_DURATION;
    configuration.alertRemainGram = DEFAULT_ALERT_REMAIN_GRAM;

}

void printDateTime(){
  /*Serial.print(F("Current UNIX time: "));
  Serial.print(now());
  Serial.println(F(" (seconds since 1/1/1970 UTC)"));
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println();*/
}
void printDigits(int digits){
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
