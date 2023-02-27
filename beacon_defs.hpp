#ifndef BEACON_DEFS_H
#define BEACON_DEFS_H
#define DEV_EUI "68cbd47450d82f3d"
#define DEV_ADDR "01c94f60"
#define APPS_KEY "1f264b78a764570f53d900f3d435cbce"
#define NWKS_KEY "1f264b78a764570f53d900f3d435cbce"
#define APP_KEY
#define SMOVE(x) (static_cast<SoftwareSerial &&>(x))
#define MOVE(x) (static_cast<String &&>(x))
#define getName(VariableName) #VariableName

#include <SoftwareSerial.h>
#include <Arduino.h>



class PortHandle {

public:

  PortHandle(SoftwareSerial &portNew)
    : MySerial(&portNew) {
    Serial.begin(115200);
    MySerial->begin(9600);
    MySerial->setTimeout(1500);
    Serial.println("PortHandle started with the reference");
  };
  PortHandle(SoftwareSerial *portNew)
    : MySerial(portNew) {
    Serial.begin(115200);
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
private:
  SoftwareSerial *MySerial;
  String portSend(const char *pdata);
};
//Class -------------------------------------------------




void PortHandle::get_keys(void) {

 
    String B = MOVE(portSend("AT+DEVADDR=" DEV_ADDR));
    while (B != "OK") {
      B = MOVE(portSend("AT+DEVADDR=" DEV_ADDR));
    }
    B = MOVE(portSend("AT+DEVEUI=" DEV_EUI));
    while (B != "OK") {
      B = MOVE(portSend("AT+DEVEUI=" DEV_EUI));
    }

    B = MOVE(portSend("AT+APPSKEY=" APPS_KEY));
    while (B != "OK") {
      B = MOVE(portSend("AT+APPSKEY=" APPS_KEY));
    }

    B = MOVE(portSend("AT+NWKSKEY=" NWKS_KEY));
    while (B != "OK") {
      B = MOVE(portSend("AT+NWKSKEY=" NWKS_KEY));
    }

}




void PortHandle::set_keys(int choice) {

  if (0 == choice) {
    String B = MOVE(portSend("AT+DEVADDR="));
    while (B != "OK") {
      B = MOVE(portSend("AT+DEVADDR="));
    }
    B = MOVE(portSend("AT+DEVEUI="));
    while (B != "OK") {
      B = MOVE(portSend("AT+DEVEUI="));
    }

    B = MOVE(portSend("AT+APPSKEY="));
    while (B != "OK") {
      B = MOVE(portSend("AT+APPSKEY="));
    }

    B = MOVE(portSend("AT+NWKSKEY="));
    while (B != "OK") {
      B = MOVE(portSend("AT+NWKSKEY="));
    }

    Serial.println("Auto Register (choice = 0) completed");
  }


  Serial.println("Auto Register (choice = 0) completed");
}

String PortHandle::portSend(const char *pdata) {

  MySerial->println(pdata);
  delay(100);
  if (MySerial->available()) {
    String resValue = String{ MySerial->readString() };
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
  sprintf(tempData, "AT+SEND=%d:%s", port, dval);
  Serial.println(tempData);
  String resp = portSend(tempData);


  if (resp == "OK")
    return 0;
  else return 1;
}
uint8_t PortHandle::Send(int pdata, int port) {
  char tempData[100];
  String(pdata).length() % 2 ? sprintf(tempData, "AT+SEND=%d:0%s", port, pdata) : sprintf(tempData, "AT+SEND=%d:%s", port, pdata);
  Serial.println(tempData);
  String resp = portSend(tempData);


  if (resp == "OK")
    return 0;
  else return 1;
}
uint8_t PortHandle::Send(const char *pdata, int port) {
  char tempData[100];
  sprintf(tempData, "AT+SEND=%d:%s", port, pdata);
  Serial.println(tempData);
  String resp = portSend(tempData);


  if (resp == "OK")
    return 0;
  else return 1;
}


#endif
