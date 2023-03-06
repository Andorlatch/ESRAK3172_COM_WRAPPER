#include "beacon_defs.hpp"
SoftwareSerial mySerial(16, 17);
PortHandle hport(mySerial);

void setup() {

}

void loop() {
  hport.set_conf();
  delay(1000);
  int c = hport.Send(344);
  delay(1000);
  if (!c)
    Serial.println("OKEYIZ");
  else Serial.println("FAILED");





}
