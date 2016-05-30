#ifndef _PROPERTIES_H
#define _PROPERTIES_H

//#include <string.h>
#include <ArduinoJson.h>
#include "HX711.h"
#include <stdio.h>
#include <ESP8266WiFi.h>
#define epsilon ((double)2.5)
#define GAS_BUFFER_SIZE 256
#define GAS_ID_SIZE 15



#define ARDUINO_ID_KEY "arduinoId"

//KEY FOR JSON OBJECT
#define CURRENT_GAS_WEIGHT_KEY "currentGasWeight"
#define USED_DURATION_KEY   "usedDuration"
#define START_DATE_TIME_KEY "startDatetime"

//KEY FOR SETTING INFO
#define TANK_TYPE_KEY "tankType"
#define TANK_TYPE_NAME_KEY "name"
#define TANK_TYPE_TANK_WEIGHT_KEY "tankWeight"
#define TANK_TYPE_NET_WEIGHT_KEY "netWeight"

#define NOTIFIED_LEAKING_RISK_KEY "notifiedLeakingRisk"
#define NOTIFIED_REMANING_WEIGHT_KEY "notifiedRemainingWeight"

//#define CONT_USED_MINS_KEY "usedPeriod"
//#define MSG_KEY "message"

#define CONFIG_STATUS_KEY "configStatus"

#define SCALE_IDLE_TIMEOUT 10000 //60s

// constants won't change :
#define RISK_INTERVAL  60 * 1000    
#define REMAIN_INTERVAL  60 * 1000


#define DEFAULT_TANK_WGT 0
#define DEFAULT_GROSS_WGT 10000 //10 kg
#define DEFAULT_RISK_DURATION 1
#define DEFAULT_ALERT_REMAIN_PCENT 5
#define DEFAULT_ALERT_REMAIN_GRAM 1000 // 1kg

#define CONNECT_ATTEMPTS   10

//REST SERVICE CONNECTION
#define STRING_SIZE 80
#define updateInterval  3 * 1000      // Time interval in milliseconds to update REST service (number of seconds * 1000 = interval)


#define PUT_HTTP_VERB "PUT"
#define GET_HTTP_VERB "GET"

#define WEBSITE      "iot.axonactive.vn"
#define STATUS_CODE_OK 200
#define STATUS_CODE_CONFLICT 409


// The struct of the configuration.
struct Configuration {
  char deviceId[15];
  char deviceNm[15];
  const char* tankTypeName;
  double tankWgt;
  double totalWgt;
  double alertRemainGram; //in gram 
  unsigned int riskDuration;// in second
  bool isUpdated;
};
struct GasData{
  double curGrossWgt;
  double curGasWgt;
  double prevGasWgt;
  
  unsigned long prevReadTimestamp;
  unsigned long curReadTimestamp;
  unsigned long beginUsedTimestamp;
  unsigned int usedPeriod;

  boolean isUsing = false;
};

struct RestResponse{
   int statusCode;
   String data;
};
extern long lastConnectionTime ; 
extern boolean lastConnected;
extern int failedCounter ;
extern Configuration configuration;
extern GasData gasData;
extern HX711 scale1;    
extern HX711 scale2;  
extern HX711 scale3;  

#define SCALE_DT_PIN1   12     // Load cell 1 DT pin connect to ESP
#define SCALE_SCK_PIN1  14     // Load cell 1 SCK pin connect to ESP
#define SCALE_DT_PIN2   13     // Load cell 2 DT pin connect to ESP
#define SCALE_SCK_PIN2  5     //GPIO4 Load cell 2 SCK pin connect to ESP
#define SCALE_DT_PIN3   4      //GPIO5 Load cell 3 DT pin connect to ESP
#define SCALE_SCK_PIN3  16      // Load cell 3 SCK pin connect to ESP

#define SCALE_PARAM1 103.33f
#define SCALE_PARAM2 105.9f
#define SCALE_PARAM3 108.75f
#define OFFSET1 82023
#define OFFSET2 78785
#define OFFSET3 77136

#define STATUS_PIN  2             // Status pin
 
#endif

