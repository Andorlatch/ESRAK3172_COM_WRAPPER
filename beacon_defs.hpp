#ifndef BEACON_DEFS_H
#define BEACON_DEFS_H

#define SMOVE(x) (static_cast<SoftwareSerial &&>(x))
#define MOVE(x) (static_cast<String &&>(x))
#define getName(VariableName) #VariableName

#include <SoftwareSerial.h>
#include <Arduino.h>
#include "beacon_confs.hpp"


class PortHandle {

  public:

    PortHandle(SoftwareSerial &portNew)
      : MySerial(&portNew) {
      Serial.begin(115200);
      MySerial->begin(9600);
      MySerial->setTimeout(1);
      Serial.println("PortHandle started with the reference");
    };
    PortHandle(SoftwareSerial *portNew)
      : MySerial(portNew) {
      Serial.begin(9600);
      MySerial->begin(9600);
      MySerial->setTimeout(1500);
      Serial.println("PortHandle started with the pointer ");
    };

    /* String listenRes() {
      String holdStr = MOVE(MySerial->readString());
      holdStr.trim();
      return holdStr;


      }*/
    uint8_t Send(const char *pdata, int port = 1);
    uint8_t Send(int pdata, int port = 1);
    uint8_t Send(double ddata, int port = 1);
    void set_keys(int choice = 0);
    void get_keys(void);
    void get_conf(const int mode_selector = 0);
    void set_conf(const int mode_selector = 0);
    String CheckDownlink(void);
  private:
    SoftwareSerial *MySerial;
    String portSend(const char *pdata);
};
//Class -------------------------------------------------


void PortHandle::set_conf(const int mode_selector) {
  recent_mode = mode_selector;
  if (1 == mode_selector) { // NWM = LORAWAN

    String B = portSend("AT+NWM=1" );
#ifdef DEBUG
    Serial.println("AT+NWM=1" );
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+NWM=1");
    }

    B = portSend("AT+CLASS=" CLASS );
#ifdef DEBUG
    Serial.println("AT+CLASS=" CLASS );
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+CLASS=" CLASS );
    }
    B = portSend("AT+BAND=" BAND);
#ifdef DEBUG
    Serial.println("AT+BAND=" BAND );
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+BAND=" BAND );
    }
    B = MOVE(portSend("AT+NJM=" NJM));
#ifdef DEBUG
    Serial.println("AT+NJM=" NJM);
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+NJM=" NJM );
    }

    B = portSend("AT+JOIN=" JOIN_REQ);
#ifdef DEBUG
    Serial.println("AT+JOIN=" JOIN_REQ);
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+JOIN=" JOIN_REQ);
    }
  }
  else if (0 == mode_selector) {

    // NWM = P2P
    //<Freq>,<SF>,<Bandwidth>,<CR>,<Preamble>,<Power>
    //AT+P2P=868000000:7:125:0:8:15
    String B = portSend("AT+NWM=0" );
#ifdef DEBUG
    Serial.println("AT+NWM=0" );
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+NWM=0");
    }
    B = portSend("AT+P2P="P2P_OPS );
#ifdef DEBUG
    Serial.println("AT+P2P="P2P_OPS);
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+P2P="P2P_OPS);
    }
  }
}
void PortHandle::get_conf(const int mode_selector) {

  if ( 1 == recent_mode) {
    String B = portSend("AT+CLASS=?" );
#ifdef DEBUG
    Serial.println("AT+CLASS=?" );
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+CLASS=?" );
    }
    B = portSend("AT+BAND=?" );
#ifdef DEBUG
    Serial.println("AT+BAND=?" );
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+BAND=?" );
    }

    B = portSend("AT+NWM=?" );
#ifdef DEBUG
    Serial.println("AT+NWM=?" );
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+NWM=?" );
    }

    B = portSend("AT+NJM=?" );
#ifdef DEBUG
    Serial.println("AT+NJM=?" );
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+NJM=?" );
    }
    Serial.println("MODE: 1 confs here completed");
  }
  else if (1 == recent_mode) {

    String B = portSend("AT+P2P=?" );
#ifdef DEBUG
    Serial.println("AT+P2P=?" );
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+P2P=?" );
    }
  }


}
void PortHandle::get_keys(void) {

  if (0 == recent_mode)
    Serial.println("First change mode. \n Recent Mode is : P2P");
  else {


    String B = portSend("AT+DEVADDR=?" );
#ifdef DEBUG
    Serial.println("AT+DEVADDR=?" );
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+DEVADDR=?" );
    }
    B = portSend("AT+DEVEUI=?" );
#ifdef DEBUG
    Serial.println("AT+DEVEUI=?" );
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+DEVEUI=?" );
    }

    B = portSend("AT+APPSKEY=?" );
#ifdef DEBUG
    Serial.println("AT+APPSKEY=?" );
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+APPSKEY=?" );
    }

    B = portSend("AT+NWKSKEY=?" );
#ifdef DEBUG
    Serial.println("AT+NWKS_KEY=?" );
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+NWKSKEY=?" );
    }

    Serial.println("Auto Register (choice = 0) completed");
  }
}





void PortHandle::set_keys(int choice) {

  if (0 == choice) {

    String B = portSend("AT+DEVADDR=" DEV_ADDR);
#ifdef DEBUG
    Serial.println("AT+DEVADDR=" DEV_ADDR);
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+DEVADDR=" DEV_ADDR);
    }
    B = portSend("AT+DEVEUI=" DEV_EUI);
#ifdef DEBUG
    Serial.println("AT+DEVEUI=" DEV_EUI);
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+DEVEUI=" DEV_EUI);
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
    Serial.println("AT+NWKS_KEY=" NWKS_KEY);
#endif
    while (B.indexOf("OK") == -1) {
      B = portSend("AT+NWKSKEY=" NWKS_KEY);
    }

    Serial.println("Auto Register (choice = 0) completed");
  }

}

String PortHandle::portSend(const char *pdata) {

  MySerial->println(pdata);
  delay(100);
  if (MySerial->available()) {
    String resValue = MySerial->readString();
    resValue.trim();
#ifdef DEBUG
    Serial.println(resValue);
#endif
    return resValue;
  }
}

uint8_t PortHandle::Send(double ddata, int port) {
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
    Serial.println("(if)Sending double response is: " +  resp);
#endif
    return 0;
  }

  else {
#ifdef DEBUG
    Serial.println("(else)Sending double response is: " + resp);

#endif
    return 1;
  }
}
uint8_t PortHandle::Send(int pdata, int port) {
  char tempData[100];
  int countertemp = 0;
  String tempStr = String(pdata);
  if (tempStr.length() % 2)
  {

    if ("1" == NWM)
      sprintf(tempData, "AT+SEND=%d:A%d", port, pdata);
    if ("0" == NWM)
      sprintf(tempData, "AT+PSEND=A%d", pdata);

  }
  else
  {

    if ("1" == NWM)
      sprintf(tempData, "AT+SEND=%d:%d", port, pdata);
    else if ("0" == NWM)
      sprintf(tempData, "AT+PSEND=%d", pdata);
  }

#ifdef DEBUG
  Serial.println(tempData);
#endif
  String resp = portSend(tempData);
  if (resp == "OK") {

#ifdef DEBUG
    Serial.println("(if)Sending int response is: " + resp);
#endif
    portSend(tempData);
    while ((delay(800), resp = MySerial->readString())) {

      Serial.println("Response = " + resp);

      if (resp.indexOf("CONFIRMED OK") != -1)
        return 0;
      else {

        if (resp.indexOf("FAILED") != -1)
          portSend(tempData);

        
      }

      if(countertemp++ % 10 == 0){
        portSend(tempData);
      }

      



    }

  }

  else {
#ifdef DEBUG
    Serial.println("(else)Sending int response is: " + resp);

#endif
    return 1;
  }

}
uint8_t PortHandle::Send(const char *pdata, int port) {
  char tempData[100];
  if (String{pdata} .length() % 2)
  {

    if ("1" == NWM)
      sprintf(tempData, "AT+SEND=%d:A%s", port, pdata);
    else if ("0" == NWM)
      sprintf(tempData, "AT+PSEND=A%s", port, pdata);

  }
  else
  {

    if ("1" == NWM)
      sprintf(tempData, "AT+SEND=%d:%s", port, pdata);
    else if ("0" == NWM)
      sprintf(tempData, "AT+PSEND=%s", port, pdata);
  }

#ifdef DEBUG
  Serial.println(tempData);
#endif
  String resp = portSend(tempData);


  if (resp == "OK") {
#ifdef DEBUG
    Serial.println("(if)Sending const char* response is: " +  resp);
#endif
    return 0;
  }

  else {
#ifdef DEBUG
    Serial.println("(else)Sending const char * response is: " + resp);

#endif
    return 1;
  }
}

String PortHandle::CheckDownlink(void) {
  MySerial->println("AT+RECV=?");
  delay(1000);
  String holdStr = MySerial->readString();
  
  #ifdef DEBUG 
  Serial.println("Downlink data -> "+holdStr);
  #endif
  holdStr.trim();
  if(holdStr.indexOf("1:") != -1){
   holdStr = holdStr.substring(holdStr.indexOf("1:")+2,holdStr.length());
    Serial.println("Downlink data -> "+holdStr);
      return holdStr;
  }
  

}

#endif
