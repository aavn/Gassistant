#include <ArduinoJson.h>
#include "properties.h"
#include "json_util.h"
#include "init_util.h"
#include <TimeLib.h>



uint32_t parseConfig(const char * configStr){
  uint32_t ok = 1;
  char buffer[300];
  StaticJsonBuffer<350> jsonBuffer;

  strncpy(buffer, configStr, 300);
  buffer[300 - 1] = '\0';
  JsonObject& root = jsonBuffer.parseObject(buffer);

  StaticJsonBuffer<200> returnJsonBuffer;
  Serial.println();
  Serial.print(ARDUINO_ID_KEY);
  Serial.print(": ");
  Serial.println(configuration.deviceId);
  Serial.println();
  Serial.print(CONFIG_STATUS_KEY);
  Serial.print(": ");
  
  if (!root.success()) {
    Serial.println("FAIL");
    ok = 0;
  }else{
    //Tank type value
    
    configuration.tankTypeName = root[TANK_TYPE_KEY][TANK_TYPE_NAME_KEY];
    configuration.tankWgt = (double)(root[TANK_TYPE_KEY][TANK_TYPE_TANK_WEIGHT_KEY]) ; //in gram
    configuration.totalWgt = (double)(root[TANK_TYPE_KEY][TANK_TYPE_NET_WEIGHT_KEY]); //in gram
    
    //Notification value
    configuration.riskDuration = (unsigned int)(root[NOTIFIED_LEAKING_RISK_KEY]) *60; //IN second
    configuration.alertRemainGram = (double)(root[NOTIFIED_REMANING_WEIGHT_KEY]);
    configuration.isUpdated= true;
    Serial.println("OK");
  }
  Serial.println();
  printConfig();
  return ok;
}
void printConfig(void){
  Serial.print("Tank ID: ");
  Serial.print(configuration.deviceId);
  Serial.print("\nTank name: ");
  Serial.print(configuration.deviceNm);
  Serial.print("\nTank type: ");
  Serial.print(configuration.tankTypeName);
  Serial.print("\nTank weight in gram: ");
  Serial.print(configuration.tankWgt);
  Serial.print("\nTotal weight(tank + max gas): ");
  Serial.print(configuration.totalWgt);
  Serial.print("\nWhen to alert user: " );
  Serial.print(configuration.alertRemainGram);
  Serial.print(" gram ");
  Serial.print("\nAlert possible leaking when gas decreasing continuously ");
  Serial.print(configuration.riskDuration/60); 
  Serial.print(" mins\n");
}

//get current gas information in Json format
void buildGasInfoJsonDataStr( char * dataStr)
{
  char dateStr[30];
  unsigned long sampleTime = now();
  /*int n= sprintf (dateStr,"%d-%d-%dT%d:%d:%d.000+07"
                    ,year(gasData.beginUsedTimestamp)
                    ,month(gasData.beginUsedTimestamp)
                    ,day(gasData.beginUsedTimestamp)
                    ,hour(gasData.beginUsedTimestamp)
                    ,minute(gasData.beginUsedTimestamp)
                    ,second(gasData.beginUsedTimestamp));*/
  int n= sprintf (dateStr,"%d-%02d-%02dT%02d:%02d:%02d"
                    ,year(sampleTime)
                    ,month(sampleTime)
                    ,day(sampleTime)
                    ,hour(sampleTime)
                    ,minute(sampleTime)
                    ,second(sampleTime));
  Serial.print("\nBegin used timestamp:" );
  Serial.println(gasData.beginUsedTimestamp);
  Serial.print("\nDate String: ");
  Serial.println(dateStr);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root[CURRENT_GAS_WEIGHT_KEY] = gasData.curGasWgt;
  root[USED_DURATION_KEY]= gasData.usedPeriod;
  root[START_DATE_TIME_KEY]=dateStr;//"2015-12-29T10:41:25.000+07";
  Serial.print("Built data string: ");
  root.prettyPrintTo(Serial);
  root.printTo(dataStr,GAS_BUFFER_SIZE);
}



