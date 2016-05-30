#include "gasscale_service.h"
#include "properties.h"
#include "json_util.h"
#include "rest_util.h"
#include "loadcell_util.h"



unsigned long riskMillis = 0;        // last time risk signal sent
unsigned long remainMillis = 0;      //last time remain signal sent
unsigned long readMillis = 0;

//Main function to process gas_service
int Main(void){
   
 if(!configuration.isUpdated){
     saveSettingInfoToMem();
     if(!configuration.isUpdated) {
       Serial.println("Setting info is not availble");
       return 0;
      }
  } 
 long start = millis();
 double gasWeight = readGasWeight();
 updateGasinfo(gasWeight);
 printGasinfo();
 long endTime = millis() - start;
 Serial.print("time to read: ");
 Serial.println(formatTime(endTime/1000));
 Serial.println("*** Sending to rest service ?");         
 if(gasData.curGasWgt>=0 && gasData.isUsing){
     sendGasInfoToRestService();
 }    
  return 1;
}


 //send the current gas information to rest server
void  sendGasinfo(){
    char buffer[GAS_BUFFER_SIZE];
    buildGasInfoJsonDataStr(buffer);
    buffer[GAS_BUFFER_SIZE - 1] = '\0';
    Serial.print("Gas Info: Update Gas Info to Rest service " );
    Serial.println(buffer);
    int n=0;
    char  url[100];

    //URL = /gassistant/rest/scales/$id_parameter/gas
     n=sprintf (url, "/gassistant/rest/scales/%s/gas",configuration.deviceId);
    RestResponse restResponse;
    //restResponse = sendRestService(PUT_HTTP_VERB, url,buffer);
    restResponse = sendRestService(PUT_CMD, url,buffer);
     if(restResponse.statusCode==STATUS_CODE_OK){
          Serial.println("Update gas info sucessfully" );
     }
     //Setting has been changed
     else if (restResponse.statusCode==STATUS_CODE_CONFLICT){
         Serial.println("Setting info has been changed");
         //Update setting
         saveSettingInfoToMem();
         //resend GasInfo 
         sendGasinfo();
     }
     else{
        Serial.print("FAIL to connect to rest service: " );
        Serial.println(restResponse.data);
     }
  }

    


void  notifyOutOfGas(){
    int n=0;
    char  url[100];
  
   //URL = /gassistant/rest/scales/&id_paramer/notify/outofgas
    n=sprintf (url, "/gassistant/rest/scales/%s/notify/outofgas",configuration.deviceId );
    char data[10];
    dtostrf(gasData.curGasWgt,5,2,data);
    Serial.print("Notify Out Of Gas: Number of Gas " );
    Serial.print(data);
    Serial.println(" gram");
    sendRestService(PUT_CMD, url,data);
    
  
}

void  notifyLeakingRisk(){

    int n=0;
    char  url[100];
    //URL = /gassistant/rest/scales/$id_parameter/notify/leakingrisk
     n=sprintf (url,"/gassistant/rest/scales/%s/notify/leakingrisk",configuration.deviceId); 
    char data[10];
    n= sprintf (data,"%d",gasData.usedPeriod );
    Serial.print("Notify Leaking Risk: Possible Gas Leaking in " );
    Serial.print(formatTime(gasData.usedPeriod));
    Serial.println(" minutes");
    sendRestService(PUT_CMD, url,data);

  
  
}

 void notifyScaleSettingsUpdated(){
    int n=0;
    char  url[100];
      
    //URL = /gassistant/rest/scales/$id_parameter/settings/updatedonscale
    n=sprintf (url, "/gassistant/rest/scales/%s/settings/updatedonscale",configuration.deviceId );  
    Serial.print("Notify updated the settings of the scale " );
    Serial.println(configuration.deviceId);
    sendRestService(PUT_CMD, url,"");
  
  }

const char * readSettingInfo(){
  int n=0;
  char  url[50];

  //URL = /gassistant/rest/scales/$id_parameter/settings
  n=sprintf (url, "/gassistant/rest/scales/%s/settings",configuration.deviceId); 
  Serial.print("Get setting info" );
  RestResponse restResponse;
  
  restResponse= sendRestService(GET_CMD,url,"");

 
  if(restResponse.statusCode==STATUS_CODE_OK){
    return restResponse.data.c_str();
  }
  else{
    Serial.print("FAIL to connect to rest service: " );
    Serial.print(restResponse.data);
    return "";
  }
  
  
}

void sendGasInfoToRestService(){
  // if(gasData.curGrossWgt < configuration.tankWgt){
  //  return;
  //}
  sendGasinfo();
  if(gasData.usedPeriod >= configuration.riskDuration
      /*&& millis() - riskMillis > RISK_INTERVAL*/ ){
    notifyLeakingRisk();
    riskMillis = millis();
  }
  if((millis() - remainMillis > REMAIN_INTERVAL) && (gasData.curGasWgt <= configuration.alertRemainGram)
      /*|| 100*gasData.curGasWgt/(gasData.curGrossWgt-configuration.tankWgt<=configuration.alertRemainPcent))*/
    ){
    notifyOutOfGas();
    remainMillis = millis();
  }

}

void saveSettingInfoToMem(){
  const char * retStr ;
  retStr  = readSettingInfo();
  if(retStr==""){
    configuration.isUpdated = false;  
    return;
  }
  else{
     parseConfig(retStr);
  }
 
  notifyScaleSettingsUpdated();
}

