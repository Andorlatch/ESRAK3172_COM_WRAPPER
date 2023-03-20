#include "stdint.h"
#ifndef LIB_ICS_H
#define LIB_ICS_H
#define BEACON_60_SECS "ab"
#define BEACON_SECS 1000
#define BEACON_1_MIN BEACON_SECS * 60

void dl_ins_return(String&& coktempStr,int& atime){
  Serial.println("DL entered to dl_ins_return = "+coktempStr);
  if(coktempStr.indexOf("fab") != -1){
    coktempStr.trim();
    coktempStr = coktempStr.substring(coktempStr.indexOf("fab")+3,coktempStr.length());
    Serial.println("(dl_ins_return) returned mins of => "+coktempStr);
    atime = coktempStr.toInt() > 100 ? 60 : coktempStr.toInt() <= 0 ? 1 : coktempStr.toInt();
    if(atime)
    Serial.println("(dl_ins_return) changed the given time to => "+String{atime}+" minutes");
    else Serial.println("(dl_ins_return) not changed the given time (data is not reasonable)");    
  }
  else if (coktempStr.indexOf("fff") != -1) {
  Serial.println("(dl_ins_return) returned a reset code... Restarting in 2 seconds.");
  delay(2000);
  ESP.restart();
  
  }
  else{
    Serial.println("not a reasonable DL");
  }

  
}


#endif
