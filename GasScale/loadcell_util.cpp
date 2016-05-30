#include "properties.h"
#include "loadcell_util.h"

HX711 scale1(SCALE_DT_PIN1, SCALE_SCK_PIN1); 
HX711 scale2(SCALE_DT_PIN2, SCALE_SCK_PIN2); 
HX711 scale3(SCALE_DT_PIN3, SCALE_SCK_PIN3);

void initLoadCellsConf(void) {
   Serial.println("scale: initializing...");
  /*
  Serial.println("Before setting up the scale:");
  Serial.print("read: scale 1 \t\t");
  Serial.println(scale1.read());     // print a raw reading from the ADC

  Serial.print("read: scale 2 \t\t");
  Serial.println(scale2.read());     // print a raw reading from the ADC

  Serial.print("read: scale 3 \t\t");
  Serial.println(scale3.read());     // print a raw reading from the ADC

  Serial.print("read average scale 1: \t\t");
  Serial.println(scale1.read_average(20));   // print the average of 20 readings from the ADC

  Serial.print("read average scale 2: \t\t");
  Serial.println(scale2.read_average(20));   // print the average of 20 readings from the ADC

  Serial.print("read average scale 3: \t\t");
  Serial.println(scale3.read_average(20));   // print the average of 20 readings from the ADC

  Serial.print("get value scale 1: \t\t");
  Serial.println(scale1.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get value scale 2: \t\t");
  Serial.println(scale2.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get value scale 3: \t\t");
  Serial.println(scale3.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units scale 1: \t\t");
  Serial.println(scale1.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided 

  Serial.print("get units scale 2: \t\t");
  Serial.println(scale2.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided 

  Serial.print("get units scale 3: \t\t");
  Serial.println(scale3.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided 
*/
  scale1.set_scale(SCALE_PARAM1);        //103.33              // this value is obtained by calibrating the scale with known weights; see the README for details
  delay(100);
  scale1.set_offset(OFFSET1*SCALE_PARAM1);
  delay(100);
  scale2.set_scale(SCALE_PARAM2);        //105.9              // this value is obtained by calibrating the scale with known weights; see the README for details
  delay(100);
  scale2.set_offset(OFFSET2*SCALE_PARAM2);
  delay(100);
  scale3.set_scale(SCALE_PARAM3);        //108.75              // this value is obtained by calibrating the scale with known weights; see the README for details
  delay(100);
  scale3.set_offset(OFFSET3*SCALE_PARAM3);
  delay(100);
  Serial.println("scale: initialized!");  /*
  Serial.println("After setting up the scale:");

  Serial.print("read scale 1: \t\t");
  Serial.println(scale1.read());                 // print a raw reading from the ADC

  Serial.print("read scale 2: \t\t");
  Serial.println(scale2.read());                 // print a raw reading from the ADC

   Serial.print("read scale 3: \t\t");
  Serial.println(scale3.read());                 // print a raw reading from the ADC

  Serial.print("read average scale 1: \t\t");
  Serial.println(scale1.read_average(20));       // print the average of 20 readings from the ADC

   Serial.print("read average scale 2: \t\t");
  Serial.println(scale2.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("read average scale 3: \t\t");
  Serial.println(scale3.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value scale 1: \t\t");
  Serial.println(scale1.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

 Serial.print("get value scale 2: \t\t");
 Serial.println(scale2.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get value scale 3: \t\t");
 Serial.println(scale3.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units scale 1: \t\t");
  Serial.println(scale1.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided 

 Serial.print("get units scale 2: \t\t");
 Serial.println(scale2.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided 

 Serial.print("get units scale 3: \t\t");
 Serial.println(scale3.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided 
 */
}


double readGasWeight() {
 double value1 = scale1.get_units(10);
 Serial.print("one reading scale 1:\t");
 Serial.print(value1, 1);
 delay(1000);
 double value2 = scale2.get_units(10);
 Serial.print(" one reading scale 2:\t");
 Serial.print(value2, 1);
 delay(1000);
 double value3 = scale3.get_units(10);
 Serial.print(" one reading scale 3:\t");
 Serial.println(value3, 1);
 delay(1000);
 double weight = (value1 + value2 + value3) ; //IN GRAM
 scale1.power_down();             // put the ADC in sleep mode
 delay(100);
 scale2.power_down();             // put the ADC in sleep mode
 delay(100);
 scale3.power_down();             // put the ADC in sleep mode
 delay(100);
 scale1.power_up();
 delay(100);
 scale2.power_up();
 delay(100);
 scale3.power_up();
 delay(100);
 return weight;
}

void updateGasinfo(double gasWeight) {
  //Update previous data
  gasData.prevGasWgt = gasData.curGasWgt;
  gasData.prevReadTimestamp = gasData.curReadTimestamp;

  //update current data
  gasData.curGrossWgt = gasWeight;
  gasData.curGasWgt = gasData.curGrossWgt - configuration.tankWgt;
  gasData.curReadTimestamp = millis();
  if(abs(gasData.prevGasWgt-gasData.curGasWgt)<epsilon){
    gasData.isUsing = false;
    gasData.beginUsedTimestamp = 0;
    gasData.usedPeriod = 0;
  }else{
    gasData.isUsing = true;
  }
  if(gasData.isUsing == true){
    if( gasData.beginUsedTimestamp == 0){
      gasData.beginUsedTimestamp = gasData.curReadTimestamp;
    }
    gasData.usedPeriod = (gasData.curReadTimestamp - gasData.beginUsedTimestamp)/1000;
  }


/*

  
  gasData.curReadTimestamp = millis();
  if (gasData.prevGasWgt == 0) {
    gasData.prevGasWgt = gasData.curGasWgt;
  }
  // change object to weight
  if (abs(gasData.curGasWgt - gasData.prevGasWgt) > epsilon) {
    gasData.prevGasWgt = gasData.curGasWgt;
  }

  //in use
  if ((gasData.prevGasWgt - gasData.curGasWgt) >= epsilon) {
      gasData.isUsing = true;
      if( gasData.beginUsedTimestamp == 0){
        gasData.beginUsedTimestamp = gasData.curReadTimestamp;
    }
    gasData.usedPeriod = (gasData.curReadTimestamp - gasData.beginUsedTimestamp)/1000;
    gasData.prevGasWgt = gasData.curGasWgt;
  } 
  // not in use
  else {
      gasData.isUsing = false;
      gasData.beginUsedTimestamp = 0;
      gasData.usedPeriod = 0;
  }
  */
}



void printGasinfo(void) {
  Serial.print("Gas info:\t");
  Serial.print("curGasWgt (gram):\t");
  Serial.print((gasData.curGasWgt));
  Serial.print("\t prevGasWgt (gram):\t");
  Serial.print((gasData.prevGasWgt));
  Serial.print("\t curReadTimestamp:\t");
  Serial.print(gasData.curReadTimestamp);
  Serial.print("\t beginUsedTimestamp:\t");
  Serial.print(gasData.beginUsedTimestamp);
  Serial.print("\t usedPeriod:\t");
  Serial.print(formatTime(gasData.usedPeriod));
  Serial.print("\t isUsing:\t");
  Serial.println(gasData.isUsing);
}

// t is time in seconds = millis()/1000;
char * formatTime(unsigned long t)
{
 static char str[12];
 long h = t / 3600;
 t = t % 3600;
 int m = t / 60;
 int s = t % 60;
 sprintf(str, "%04ld:%02d:%02d", h, m, s);
 return str;
}



