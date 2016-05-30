#include "properties.h"
#include "rest_util.h"
#include "wifi_util.h"
#include "init_util.h"
#include <ESP8266WiFi.h>
#include <TimeLib.h>
#include <stdlib.h> 

WiFiClient client;
// Variable Setup
long lastConnectionTime = 0; 
boolean lastConnected = false;
int failedCounter = 0;

RestResponse sendRestService(int httpVerb, const char * url ,const char * data){
  RestResponse restResponse;
  restResponse = updateRestService(httpVerb, url,data);
  // Check if Arduino Ethernet needs to be restarted
  if (failedCounter > 3 ) {initWifi();}
  return restResponse;
  
}



RestResponse updateRestService(int httpVerb, const char * url, const char * data)
{

  if(!client.connect(WEBSITE, 80))
  {
    failedCounter++;
    Serial.println("Connection to REST service Failed ("+String(failedCounter, DEC)+")");   
    Serial.println();
    lastConnectionTime = millis(); 
    RestResponse result;
    result.statusCode= -1 ;
    result.data="Could not connect to rest service";
    return result;
  } 
  
  if(httpVerb== PUT_CMD){
  //if(httpVerb.equals(PUT_HTTP_VERB)){
          int len;
          len = strlen(data);
          client.print(PUT_HTTP_VERB);
          client.print(" ");
          client.print(url);
          client.print(" HTTP/1.1");
          client.println();
          client.print("Host: ");
          client.print(WEBSITE);
          client.println();
          client.println("Content-Type: application/json");
          client.println("Content-Length: " +String(len) );
          client.println("Connection: close");
          client.println();
          client.println(data);
          
     }
     else if(httpVerb== GET_CMD){
     //else if (httpVerb.equals(GET_HTTP_VERB)){
          client.print(GET_HTTP_VERB);
          client.print(" ");
          client.print(url);
          client.print(" HTTP/1.1");
          client.println();
          client.print("Host: ");
          client.print(WEBSITE);
          client.println();
          client.println("Content-Type: application/json");
          client.println();
          client.println();
      } 

    lastConnectionTime = millis();
    failedCounter = 0;  
   delay(1000);

  String response="";

   // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    response.concat(line);
    Serial.println(line);  // uncomment to debug return from service
  } 
  Serial.println();
  Serial.println("closing connection");
  
  
  return getResponseData(response); 
}

RestResponse getResponseData(String response)
{
 
 //Get status code
  RestResponse restResponseData;
  String statusCode;
  int spaceIndex = response.indexOf(" ");
  statusCode = response.substring(spaceIndex+1);
 
  spaceIndex = statusCode.indexOf(" "); 
  statusCode = statusCode.substring(0,spaceIndex);
  Serial.println("Status Code: " + statusCode);
  restResponseData.statusCode = atoi (statusCode.c_str());
  
  //Get response darta
  int blnkLineIndex = response.lastIndexOf('\n',response.length()-2) ;
  String data = response.substring(blnkLineIndex);
  Serial.println("Response Data: " + data);
  restResponseData.data = data;
  
  return restResponseData;
}

void syncTime()
{ 
  //call sysn time service from rest server
  RestResponse restResponse;
 //char  url[50];
  restResponse = sendRestService(GET_CMD,"/gassistant/rest/scales/synctime",""); 
  
  if(restResponse.statusCode==STATUS_CODE_OK){
    String millisStr = restResponse.data; 
    millisStr = millisStr.substring(0,millisStr.length() -3);
  
    //unsigned long receivedTime = strtoul(millisStr.c_str(),NULL,10) ;
    unsigned long receivedTime = atoi(millisStr.c_str()) ;    // Fix for MAC
    if(receivedTime > 0){
      setTime(receivedTime);
    }
    Serial.print("setTime: ");
    Serial.println(receivedTime);
    printDateTime();
  }
  else{
    Serial.print("FAIL to connect to rest service: " );
    Serial.println(restResponse.data);
  }
}


