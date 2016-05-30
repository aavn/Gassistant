#include <ESP8266WiFi.h>
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starting \n");
  int chipId = 0;
  chipId = ESP.getChipId();
  Serial.print("Chip ID: " );
  Serial.println(chipId);
}

void loop() {
  // put your main code here, to run repeatedly:

}
