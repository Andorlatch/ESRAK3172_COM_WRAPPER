#define DEBUG
#include "new_beacon_definitions.hpp"
SoftwareSerial mySerial(16, 17);
PortHandle hport(mySerial);

void setup() {

}

void loop() {
  hport.autoRegister();
  delay(1000);
  
  
  

}
