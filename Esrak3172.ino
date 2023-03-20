#include "beacon_defs.hpp"
#include "lib_ics.hpp"
SoftwareSerial mySerial(16, 17);
PortHandle hport(mySerial);
unsigned long eskiZaman = 0;
unsigned long yeniZaman;
int tempTime = 1;
int flag = 0;
void setup() {
  hport.set_conf(1);
  hport.set_keys();
}

void loop() {

  yeniZaman = millis();
  /* bir önceki turdan itibaren 1000 milisaniye geçmiş mi
  yani yeniZaman ile eskiZaman farkı 1000den büyük mü */
  if (yeniZaman - eskiZaman > BEACON_1_MIN * tempTime) {
    String tempDL = hport.Send(3344);
     dl_ins_return(MOVE(tempDL),tempTime);

    Serial.println("tempTime is == "+String{tempTime});
    // if (tempDL.indexOf("ab") != -1) {
    //   tempTime = 10000;
    //   Serial.println("Downlink has (ab) ->" + tempDL);

    // } else if (tempDL.indexOf("00") != -1)
    //   Serial.println("Downlink has (00) ->" + tempDL);
    // else {
    //   Serial.println("Downlink has (FF) ->" + tempDL);
    // }
    eskiZaman = yeniZaman;
  }
  /* Eski zaman değeri yeni zaman değeri ile güncelleniyor */
  //Serial.println(yeniZaman);
}
