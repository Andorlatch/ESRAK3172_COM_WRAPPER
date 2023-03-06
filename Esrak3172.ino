#define DEBUG
#include "beacon_defs.hpp"
SoftwareSerial mySerial(16, 17);
PortHandle hport(mySerial);

void setup() {

}

void loop() {
  hport.autoRegister();
  delay(1000);
  
  
  

}
