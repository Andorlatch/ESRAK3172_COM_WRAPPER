#include "beacon_defs.hpp"
SoftwareSerial mySerial(16, 17);
PortHandle hport(mySerial);
int flag = 0;
void setup() {

}

void loop() {
  hport.set_conf(1);
  delay(1000);
  
  if(flag == 0){
    hport.Send(34);
    flag = 1;
  }
 
   String b = hport.CheckDownlink();

   Serial.println("Downlink data -> "+b);
  





}
