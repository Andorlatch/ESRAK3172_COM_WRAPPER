#ifndef BEACON_CONFS_H
#define BEACON_CONFS_H

//To set debug mode
#define DEBUG


////P2P
//<Freq>,<SF>,<Bandwidth>,<CR>,<Preamble>,<Power>
//AT+P2P=868000000:9:125:0:8:15
#define P2P_FREQ "868000000"
#define P2P_SF "7"
#define P2P_BANDWITH "125"
#define P2P_CR "0"
#define P2P_PREAMBLE "8"
#define P2P_POWER "15"
//#define BAND
#define P2P_OPS P2P_FREQ ":" P2P_SF ":" P2P_BANDWITH ":" P2P_CR ":" P2P_PREAMBLE ":" P2P_POWER
//Programmer needs to configure lora operation confs here
#define JOIN_REQ "1:0:10:8"  // <Join command><Auto-Join config><Reattempt interval><numbers of join attempts>
//NWM
#define P2P "0"
#define LORAWAN "1"


////LORAWAN
//CLASS
#define CLASS_A "A"
#define CLASS_B "B"
#define CLASS_C "C"
//BAND
#define BAND "4"
//NJM
#define ABP "0"
#define OTAA "1"

//Programmer needs to configure lora join keys here

#define DEV_EUI "ac1f09fffe075ef0"
#define DEV_ADDR "fe075ef0"
#define APPS_KEY "ac1f09fffe075ef0ac1f09fff8683172"
#define NWKS_KEY "ac1f09fffe075ef0ac1f09fff8683172"
#define APP_KEY

//Programmer needs to set conf parameters here
#define NWM LORAWAN
#define NJM ABP
#define CLASS CLASS_A

int recent_mode = 0;



















#endif
