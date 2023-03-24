#include "WString.h"
#ifndef BEACON_DEFS_H
#define BEACON_DEFS_H

#define SMOVE(x) (static_cast<SoftwareSerial &&>(x))
#define MOVE(x) (static_cast<String &&>(x))
#define getName(VariableName) #VariableName

#include <SoftwareSerial.h>
#include <Arduino.h>
#include "beacon_confs.hpp"
#include "lib_ics.hpp"
////////////////////////////////
/*
Sonradan eklenen bir IITR interrupt    
*/
std::atomic<bool> rxPending(false);

void IRAM_ATTR receiveHandler() {
  rxPending.store(true);
}
/*    */
String conokReturn(String &&coktempStr) {
  Serial.println("conokreturn = " + coktempStr);
  if (coktempStr.indexOf("+EVT:1:") != -1) {
    coktempStr.trim();
    coktempStr = coktempStr.substring(coktempStr.indexOf("+EVT:UNICAST") + 21, coktempStr.length());
    Serial.println("Confirmed OK return an DL with data of => " + coktempStr + " and length of " + String{ coktempStr.length() });
    return String{ coktempStr };
  } else {
    Serial.println("Confirmed OK not returned a DL");
    return String{ "No_DL_data" };
  }
}


///////////////////////////////////////
class PortHandle {

public:

  PortHandle(SoftwareSerial &portNew)
    : MySerial(&portNew) {
    Serial.begin(115200);
    MySerial->begin(9600);
    MySerial->setTimeout(1000);  //1000
    MySerial->onReceive(receiveHandler);
    Serial.println("PortHandle started with the reference");
  };
  PortHandle(SoftwareSerial *portNew)
    : MySerial(portNew) {
    Serial.begin(9600);
    MySerial->begin(9600);
    MySerial->setTimeout(1500);
    Serial.println("PortHandle started with the pointer ");
  };

  [[nodiscard]] String listenRes() {
    String holdStr = "";
    if (rxPending.load() && MySerial->available()) {
      delay(200);
      holdStr = MySerial->readString();

      rxPending.store(false);
    }
    holdStr.trim();
    return holdStr;
  }


  void dl_ins_return(String &&coktempStr, int &atime, const char *nString);
  [[nodiscard]] String Send(const char *pdata, int port = 1);
  [[nodiscard]] String Send(int pdata, int port = 1);
  [[nodiscard]] String Send(double ddata, int port = 1);
  void set_keys(int choice = 0);
  void get_keys(void);
  void get_conf(void);
  void set_conf(int mode_selector = 0);
  [[nodiscard]] String CheckDownlink(void);
  void openP2pListen();
private:
  SoftwareSerial *MySerial;
  [[nodiscard]] String portSend(const char *pdata);
};
//Class -------------------------------------------------



String PortHandle::portSend(const char *pdata) {

  MySerial->println(pdata);
  delay(10);
  /*if (MySerial->available()) {
    String resValue = MySerial->readString();
    delay(200);
    resValue.trim(); */
  String resValue = listenRes();
  delay(100);
#ifdef DEBUG
  Serial.println("DEBUGGED RESVALUE = " + resValue);
#endif


  return resValue;
}
void PortHandle::openP2pListen()
{
  String checkReturn = portSend("AT+PRECV=65535");
    checkTemp:
    if(checkReturn.indexOf("OK") != -1)
      Serial.println("P2P Listen has opened.");
    else if(checkReturn.indexOf("AT_ERROR") != -1)
    {
      checkReturn = portSend("AT+PRECV=65535");
      goto checkTemp; 
    }   

  
}
void PortHandle::set_conf(int mode_selector) {
  recent_mode = mode_selector;
  if (1 == mode_selector) {  // NWM = LORAWAN

    String B = portSend("AT+NWM=1");
#ifdef DEBUG
    Serial.println("AT+NWM=1");
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+NWM=1");
    }

    B = portSend("AT+CLASS=" CLASS);
#ifdef DEBUG
    Serial.println("AT+CLASS=" CLASS);
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+CLASS=" CLASS);
    }
    B = portSend("AT+BAND=" BAND);
#ifdef DEBUG
    Serial.println("AT+BAND=" BAND);
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+BAND=" BAND);
    }
    B = portSend("AT+NJM=" NJM);
#ifdef DEBUG
    Serial.println("AT+NJM=" NJM);
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+NJM=" NJM);
    }

    B = portSend("AT+JOIN=" JOIN_REQ);
#ifdef DEBUG
    Serial.println("AT+JOIN=" JOIN_REQ);
#endif
    while (B.indexOf("JOINED") == -1) {
      B = listenRes();
      if (B.indexOf("JOIN FAILED") != -1) {
        Serial.println("Network Join Session has been failed. The response is = " + B + " Trying to join again..");
        B = portSend("AT+JOIN=" JOIN_REQ);
      }
    }
    Serial.println("Network Join Session has been COMPLETE. The response is = " + B);
  } else if (0 == mode_selector) {

    // NWM = P2P
    //<Freq>,<SF>,<Bandwidth>,<CR>,<Preamble>,<Power>
    //AT+P2P=868000000:7:125:0:8:15
    String B = portSend("AT+NWM=0");
#ifdef DEBUG
    Serial.println("AT+NWM=0");
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+NWM=0");
    }
    B = portSend("AT+P2P=" P2P_OPS);
#ifdef DEBUG
    Serial.println("AT+P2P=" P2P_OPS);
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+P2P=" P2P_OPS);
    }
  }
}
void PortHandle::get_conf(void) {

  if (1 == recent_mode) {
    String B = portSend("AT+CLASS=?");
#ifdef DEBUG
    Serial.println("AT+CLASS=?");
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+CLASS=?");
    }
    B = portSend("AT+BAND=?");
#ifdef DEBUG
    Serial.println("AT+BAND=?");
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+BAND=?");
    }

    B = portSend("AT+NWM=?");
#ifdef DEBUG
    Serial.println("AT+NWM=?");
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+NWM=?");
    }

    B = portSend("AT+NJM=?");
#ifdef DEBUG
    Serial.println("AT+NJM=?");
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+NJM=?");
    }
    Serial.println("MODE: 1 confs here completed");
  } else if (1 == recent_mode) {

    String B = portSend("AT+P2P=?");
#ifdef DEBUG
    Serial.println("AT+P2P=?");
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+P2P=?");
    }
  }
}
void PortHandle::get_keys(void) {
  /*#if NJM == "1"
      Serial.println("Devices Registered with OTAA to the network server");
    #else 
    Serial.println("Devices Registered with ABP to the network server");
    #endif */
  String B = "";
  if (0 == recent_mode)
    Serial.println("First change mode. \n Recent Mode is : P2P");
  else {
    if (NJM == "ABP") {
      Serial.println("Recent join mode is ABP.");

      B = portSend("AT+DEVADDR=?");
#ifdef DEBUG
      Serial.println("AT+DEVADDR=?");
#endif
      while (B.indexOf("OK") == -1) {
        B = portSend("AT+DEVADDR=?");
      }

      B = portSend("AT+APPSKEY=?");
#ifdef DEBUG
      Serial.println("AT+APPSKEY=?");
#endif
      while (B.indexOf("OK") == -1) {
        B = portSend("AT+APPSKEY=?");
      }

      B = portSend("AT+NWKSKEY=?");
#ifdef DEBUG
      Serial.println("AT+NWKS_KEY=?");
#endif
      while (B.indexOf("OK") == -1) {
        B = portSend("AT+NWKSKEY=?");
      }
      B = portSend("AT+APPKEY=?");
#ifdef DEBUG
      Serial.println("AT+APP_KEY=?");
#endif
      while (B.indexOf("OK") == -1) {
        B = portSend("AT+APPKEY=?");
      }
    } else {
      Serial.println("Recent join mode is OTAA.");
      B = portSend("AT+DEVEUI=?");
#ifdef DEBUG
      Serial.println("AT+DEVEUI=?");
#endif
      while (B.indexOf("OK") == -1) {
        B = portSend("AT+DEVEUI=?");
      }

      B = portSend("AT+APPKEY=?");
#ifdef DEBUG
      Serial.println("AT+APPKEY=?");
#endif
      while (B.indexOf("OK") == -1) {
        B = portSend("AT+APPKEY=?");
      }
    }
    Serial.println("Auto Register (choice = 0) completed");
  }
}





void PortHandle::set_keys(int choice) {
  String B = "";
  if (0 == choice) {
    if (NJM == "ABP") {

        Serial.println("Recent join mode is ABP.");
      B = portSend("AT+DEVADDR=" DEV_ADDR);
#ifdef DEBUG
      Serial.println("AT+DEVADDR=" DEV_ADDR);
#endif
      while (B.indexOf("OK") == -1) {
        B = portSend("AT+DEVADDR=" DEV_ADDR);
      }

      B = portSend("AT+APPSKEY=" APPS_KEY);
#ifdef DEBUG
      Serial.println("AT+APPSKEY=" APPS_KEY);
#endif
      while (B.indexOf("OK") == -1) {
        B = portSend("AT+APPSKEY=" APPS_KEY);
      }

      B = portSend("AT+NWKSKEY=" NWKS_KEY);
#ifdef DEBUG
      Serial.println("AT+NWKSKEY=" NWKS_KEY);
#endif
      while (B.indexOf("OK") == -1) {
        B = portSend("AT+NWKSKEY=" NWKS_KEY);
      }
    } else {
      Serial.println("Recent join mode is OTAA.");

      B = portSend("AT+DEVEUI=" DEV_EUI);
#ifdef DEBUG
      Serial.println("AT+DEVEUI=" DEV_EUI);
#endif
      while (B.indexOf("OK") == -1) {
        B = portSend("AT+DEVEUI=" DEV_EUI);
      }

      B = portSend("AT+APPKEY=" APP_KEY);
#ifdef DEBUG
      Serial.println("AT+APPKEY=" APP_KEY);
#endif
      while (B.indexOf("OK") == -1) {
        B = portSend("AT+APPKEY=" APP_KEY);
      }
    }
    Serial.println("Auto Register (choice = 0) completed");
  }
}






String PortHandle::Send(double ddata, int port) {
  char tempData[100];
  String dval = String(ddata);
  dval = (dval.length() % 2 ? dval + '0' : dval);
  dval.replace('.', 'b');
  if ("1" == NWM)
    sprintf(tempData, "AT+SEND=%d:%s", port, dval);
  else if ("0" == NWM)
    sprintf(tempData, "AT+PSEND=%s", port, dval);
#ifdef DEBUG
  Serial.println(tempData);
#endif
  String resp = portSend(tempData);

  if (resp == "OK") {
#ifdef DEBUG
    Serial.println("(if)Sending double response is: " + resp);
#endif
    return resp;
  }

  else {
#ifdef DEBUG
    Serial.println("(else)Sending double response is: " + resp);

#endif
    return String{ "FF" };
  }
}
String PortHandle::Send(int pdata, int port) {
  char tempData[100];
  int countertemp = 0;
  String tempStr = String(pdata);
  if (tempStr.length() % 2) {

    if ("1" == NWM)
      sprintf(tempData, "AT+SEND=%d:A%d", port, pdata);
    if ("0" == NWM)
      sprintf(tempData, "AT+PSEND=A%d", pdata);

  } else {

    if ("1" == NWM)
      sprintf(tempData, "AT+SEND=%d:%d", port, pdata);
    else if ("0" == NWM)
      sprintf(tempData, "AT+PSEND=%d", pdata);
  }
  tempStr = "";
#ifdef DEBUG
  Serial.println(tempData);
#endif
  String resp = portSend(tempData);
  delay(100);
  if (resp.indexOf("OK") != -1 || resp.indexOf("+EVT:") != -1) {

#ifdef DEBUG
    Serial.println("(if)Sending int response is: " + resp);
#endif
label:
    //resp = listenRes();
    while (resp.indexOf("CONFIRMED") == -1) {
      if (resp.indexOf("UNICAST") != -1) {
        goto uclabel;
      }
      resp = listenRes();
      delay(100);
      //Delay eklenebilir.
      Serial.println("Listen Resp == " + resp);
    }

    if (resp.indexOf("FAILED") != -1) {
      resp = portSend(tempData);
      if(resp.indexOf("CONFIRMED OK") != -1)
        goto label;
      resp = String{ "" };
      goto label;
    } else if (resp.indexOf("UNICAST") != -1) {
uclabel:
      resp = conokReturn(MOVE(resp));
      delay(100);
      if (resp.indexOf("No_DL_data") != -1) {

        Serial.println("No Downlink returned =>> " + resp);
        return String{ "00" };

      } else {
        Serial.println("Downlink has come =>> data is: " + resp);
        return resp;
      }
    } else if (resp.indexOf("CONFIRMED OK") != -1) {

      resp = conokReturn(MOVE(resp));
      delay(100);
      if (resp.indexOf("No_DL_data") != -1) {

        Serial.println("No Downlink returned =>> " + tempStr);
        return String{ "00" };

      } else {
        Serial.println("Downlink has come =>> data is: " + resp);
        return resp;
      }
    }
  }

  else {
#ifdef DEBUG
    Serial.println("(else)Sending int response is: " + resp);

#endif
    return String{ "FF" };
  }
}
String PortHandle::Send(const char *pdata, int port) {
  char tempData[100];
  if (String{ pdata }.length() % 2) {

    if ("1" == NWM)
      sprintf(tempData, "AT+SEND=%d:A%s", port, pdata);
    else if ("0" == NWM)
      sprintf(tempData, "AT+PSEND=A%s", port, pdata);

  } else {

    if ("1" == NWM)
      sprintf(tempData, "AT+SEND=%d:%s", port, pdata);
    else if ("0" == NWM)
      sprintf(tempData, "AT+PSEND=%s", port, pdata);
  }

#ifdef DEBUG
  Serial.println(tempData);
#endif
  String resp = portSend(tempData);


  if (resp.indexOf("OK") != -1 || resp.indexOf("+EVT:") != -1) {

#ifdef DEBUG
    Serial.println("(if)Sending int response is: " + resp);
#endif
label:
    //resp = listenRes();
    while (resp.indexOf("CONFIRMED") == -1) {
      if (resp.indexOf("UNICAST") != -1) {
        goto uclabel;
      }
      resp = listenRes();
      delay(100);
      //Delay eklenebilir.
      Serial.println("Listen Resp == " + resp);
    }

    if (resp.indexOf("FAILED") != -1) {
      portSend(tempData);
      resp = String{ "" };
      goto label;
    } else if (resp.indexOf("UNICAST") != -1) {
uclabel:
      resp = conokReturn(MOVE(resp));
      delay(100);
      if (resp.indexOf("No_DL_data") != -1) {

        Serial.println("No Downlink returned =>> " + resp);
        return String{ "00" };

      } else {
        Serial.println("Downlink has come =>> data is: " + resp);
        return resp;
      }
    } else if (resp.indexOf("CONFIRMED OK") != -1) {

      resp = conokReturn(MOVE(resp));
      delay(100);
      if (resp.indexOf("No_DL_data") != -1) {

        Serial.println("No Downlink returned =>> " + resp);
        return String{ "00" };

      } else {
        Serial.println("Downlink has come =>> data is: " + resp);
        return resp;
      }
    }
  }

  else {
#ifdef DEBUG
    Serial.println("(else)Sending int response is: " + resp);

#endif
    return String{ "FF" };
  }
}

String PortHandle::CheckDownlink(void) {
  MySerial->println("AT+RECV=?");
  delay(1000);
  String holdStr = MySerial->readString();

#ifdef DEBUG
  Serial.println("Downlink data -> " + holdStr);
#endif
  holdStr.trim();
  if (holdStr.indexOf("1:") != -1) {
    holdStr = holdStr.substring(holdStr.indexOf("1:") + 2, holdStr.length());
    holdStr.trim();
    Serial.println("Downlink data -> " + holdStr);
    return holdStr;
  }
}

void PortHandle::dl_ins_return(String &&coktempStr, int &atime, const char *nString) {
  Serial.println("DL entered to dl_ins_return = " + coktempStr);
  if (coktempStr.indexOf("ffab") != -1) {
    coktempStr.trim();
    coktempStr = coktempStr.substring(coktempStr.indexOf("ffab") + 4, coktempStr.length());
    Serial.println("(dl_ins_return) returned mins of => " + coktempStr);
    atime = coktempStr.toInt() > 100 ? 60 : coktempStr.toInt() <= 0 ? 1
                                                                    : coktempStr.toInt();
    if (atime)
      Serial.println("(dl_ins_return) changed the given time to => " + String{ atime } + " minutes");
    else Serial.println("(dl_ins_return) not changed the given time (data is not reasonable)");
  } else if (coktempStr.indexOf("ffff") != -1) {
    Serial.println("(dl_ins_return) returned a reset code... Restarting in 2 seconds.");
    delay(2000);
    ESP.restart();

  } else if (coktempStr.indexOf("ffcc") != -1) {
    //Bu kısım geçici olarak yazılıyor. Virtual keywordu öğrenildikten sonra bakılacak.
    String tempDL = Send(nString);
    //delay(2000);


  } else {
    Serial.println("not a reasonable DL");
  }
}


#endif
