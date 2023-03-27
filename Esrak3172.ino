#include "beacon_defs.hpp"
#include "lib_ics.hpp"
SoftwareSerial mySerial(16, 17);
PortHandle hport(mySerial);
unsigned long oldTimeLaps = 0;
unsigned long newTimeLaps;
int tempTime = 10;
int mode_flag = 0, p2p_rec_flag = 0;
String p2p_temp_check = "";

void setup() {
  hport.set_conf(1);
  hport.set_keys();
}

void loop() {

  if (!mode_flag) {  //Checking for Mode P2P.
    hport.set_conf(0);
    Serial.println("Mode changed to P2P. Starting listening.");
    mode_flag = 1;
  }

  if (!p2p_rec_flag) {
    hport.openP2pListen();
    p2p_rec_flag = 1;
  }

  p2p_temp_check = hport.listenRes();

  while (p2p_temp_check.indexOf("RXP2P") == -1)
    p2p_temp_check = hport.listenRes();

  if (p2p_temp_check.indexOf("RXP2P") != -1) {
    Serial.println("Recieved Message = " + p2p_temp_check);
    p2p_rec_flag = 0;
  }



  newTimeLaps = millis();


  if (newTimeLaps - oldTimeLaps > BEACON_1_MIN * tempTime) {
    hport.set_conf(1);
    hport.set_keys();
    mode_flag = 0;
    String tempDL = hport.Send(3344);
    hport.dl_ins_return(MOVE(tempDL), tempTime, "34");

    Serial.println("tempTime is == " + String{ tempTime });
    //Serial.println("downlink is == "+hport.CheckDownlink());
    // if (tempDL.indexOf("ab") != -1) {
    //   tempTime = 10000;
    //   Serial.println("Downlink has (ab) ->" + tempDL);

    // } else if (tempDL.indexOf("00") != -1)
    //   Serial.println("Downlink has (00) ->" + tempDL);
    // else {
    //   Serial.println("Downlink has (FF) ->" + tempDL);
    // }
    oldTimeLaps = newTimeLaps;
  }

  //Serial.println(yeniZaman);
}
