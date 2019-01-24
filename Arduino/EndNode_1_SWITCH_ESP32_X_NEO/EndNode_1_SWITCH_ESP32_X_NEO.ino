

// ------------------------------------------------------------ //
// EndNode_1_Switch
// V.0.1 04/01/2019
// First version 0.1 Development
//
// ------------------------------------------------------------ //
// Include libraries //
#include <Timer.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "pitches.h"

#include <Scheduler.h>
#include <Task.h>

#include <NeoNextion.h>
#include <NextionPage.h>
#include <NextionButton.h>
#include <SoftwareSerial.h>
#include <NextionVariableString.h>
#include <INextionStringValued.h>
#include <NextionPicture.h>
#include <NextionProgressBar.h>
#include <NextionTimer.h>
#include <NextionVariableNumeric.h>
#include <NextionRadioButton.h>

// ------------------------------------------------------------ //
//-----------------------------------------------------------
#define PINNODE 0
//-----------------------------------------------------------
// reserved pins
//-----------------------------------------------------------
#define NEXTION_RX 5 //D2
#define NEXTION_TX 4 //D1
#define PIN_RELE   2 //D4
#define PIN_TONE   12 //D6

//-----------------------------------------------------------
// Nextion Display
//-----------------------------------------------------------
//-----------------------------------------------------------
#define ON  1
#define OFF 0
#define PAGE_MAIN           0
#define PAGE_AC             1
#define PAGE_TV             2
#define PAGE_SETTINGS       3
#define PAGE_OPTIONS        4
#define PAGE_CONFIG_LIGHTS  5
#define PAGE_WIFI           6

#define SWITCH_MODE_HWSW    0
#define SWITCH_MODE_SW      1

SoftwareSerial nextionSerial(NEXTION_RX, NEXTION_TX);// Nextion TX to pin 11 and RX to pin 10 of Arduino
Nextion nex(nextionSerial); //create a Nextion object named myNextion using the nextion serial port @ 9600bps

//------Pages---------------------------------------------------
NextionPage pgMain(nex, 0, 0, "Main");
NextionPage pgAC(nex, 1, 0, "AC");
NextionPage pgTV(nex, 2, 0, "TV");
NextionPage pgSettings(nex, 3, 0, "Settings");
NextionPage pgOptions(nex, 4, 0, "Options");
NextionPage pgConfigLights(nex, 5, 0, "Config_Lights");
NextionPage pgWifi(nex, 6, 0, "Wifi");
NextionPage pgTradfri(nex, 7, 0, "Tradfri");

//----- Main ---------------------------//
NextionPicture bswitch(nex, 0, 6, "s1");
NextionProgressBar bdimmer(nex, 0, 1, "d1");

NextionPicture bmood1(nex, 0, 60, "md1");
NextionPicture bmood2(nex, 0, 59, "md2");
NextionPicture bmood3(nex, 0, 61, "md3");
NextionPicture bmood4(nex, 0, 62, "md4");

NextionPicture bmoncolor1(nex, 0, 20, "m1");
NextionPicture bmoncolor2(nex, 0, 21, "m2");
NextionPicture bmoncolor3(nex, 0, 22, "m3");

NextionPicture brgbcolor1(nex, 0, 37, "c1");
NextionPicture brgbcolor2(nex, 0, 38, "c2");
NextionPicture brgbcolor3(nex, 0, 39, "c3");
NextionPicture brgbcolor4(nex, 0, 40, "c4");
NextionPicture brgbcolor5(nex, 0, 41, "c5");
NextionPicture brgbcolor6(nex, 0, 42, "c6");

NextionPicture blight1(nex, 0, 12, "l1");
NextionPicture blight2(nex, 0, 13, "l2");
NextionPicture blight3(nex, 0, 11, "l3");
NextionPicture blight4(nex, 0, 10, "l4");
NextionPicture blight5(nex, 0, 14, "l5");
NextionPicture blight6(nex, 0, 17, "l6");

NextionPicture pdummy(nex, 0, 73, "pDummy"); //used to simulate triggers on numeric valiable
NextionPicture ptimer(nex, 0, 80, "pDummy"); //used to simulate triggers on numeric valiable

NextionPicture bgroup(nex, 0, 19, "g0");
NextionPicture iwifi(nex, 0, 72, "i_wifi");
NextionVariableNumeric vwifi(nex, 0, 71, "wifi");
NextionVariableNumeric vdeflight(nex, 0, 68, "defLight");
NextionVariableNumeric vdefmainsw(nex, 0, 69, "defMainSW");
NextionVariableNumeric vvl0(nex, 0, 36, "vlg0");
NextionVariableNumeric vvl1(nex, 0, 23, "vl1");
NextionVariableNumeric vvl2(nex, 0, 24, "vl2");
NextionVariableNumeric vvl3(nex, 0, 25, "vl3");
NextionVariableNumeric vvl4(nex, 0, 26, "vl4");
NextionVariableNumeric vvl5(nex, 0, 27, "vl5");
NextionVariableNumeric vvl6(nex, 0, 28, "vl6");
NextionVariableNumeric vst0(nex, 0, 50, "stg0");
NextionVariableNumeric vst1(nex, 0, 44, "st1");
NextionVariableNumeric vst2(nex, 0, 45, "st2");
NextionVariableNumeric vst3(nex, 0, 46, "st3");
NextionVariableNumeric vst4(nex, 0, 47, "st4");
NextionVariableNumeric vst5(nex, 0, 48, "st5");
NextionVariableNumeric vst6(nex, 0, 49, "st6");

NextionTimer t_icons(nex, 0, 63, "t_icons");
NextionTimer t_select_type(nex, 0, 43, "t_select_type");
NextionTimer t_default(nex, 0, 70, "default");
NextionTimer t_t1(nex, 0, 4, "tm1");

//----- Settings ----------------------------//
NextionPicture bback(nex, 3, 1, "p0");

//----- Wifi ----------------------------//
INextionStringValued t_ssid(nex, 6, 2, "t_ssid");
INextionStringValued t_pass(nex, 6, 3, "t_pass");
NextionButton bconnect(nex, 6, 6, "bconn");

//----- Wifi ----------------------------//
INextionStringValued t_ip(nex, 7, 2, "t_ip");
INextionStringValued t_key(nex, 7, 3, "t_key");
NextionButton btradfri(nex, 7, 6, "btradfri");

//----- Group select ----------------------------//
NextionRadioButton r_t0(nex, 5, 13, "g0");
NextionRadioButton r_t1(nex, 5, 14, "g1");
NextionRadioButton r_t2(nex, 5, 15, "g2");
NextionRadioButton r_t3(nex, 5, 18, "g3");
NextionRadioButton r_t4(nex, 5, 16, "g4");
NextionRadioButton r_t5(nex, 5, 17, "g5");
NextionRadioButton r_t6(nex, 5, 21, "g6");
NextionRadioButton r_t7(nex, 5, 19, "g7");
NextionRadioButton r_t8(nex, 5, 20, "g8");
NextionRadioButton r_t9(nex, 5, 22, "g9");
NextionRadioButton r_t10(nex, 5, 23, "g10");
NextionButton bloadgroups(nex, 5, 24, "b0");

//-----------------------------------------------------------

//-----------------------------------------------------------
//-----------------------------------------------------------

#define receivedOK 0
#define noResponse 1
#define dataError  2
#define xbeeError  3
#define nodeStatusOk 0
#define nodeStatusNotOk 1

//Service Led status
#define ON  HIGH
#define OFF LOW

#define NOLIGHT 999

#define PERMANENT 1
#define TEMPORARY 2
#define FLIPFLOP  3
#define TIMER     4

// Output type for Actuator
#define DIGITAL       0
#define ANALOG        1
#define SERVO         2
#define TONE          4
#define HVAC          5
#define TV            6
#define LEDRGB        7
#define THERMOSTAT    9

#define IN 1
#define OUT 0

#define TIMEt0 5000 //update lights data
#define TIMEt1 1000 //calculate group values

#define BAUD_RATE     115200  // Baud for both Xbee and serial monitor
//#define NUM_BYTE_ARR  3 // Number of bytes of the array used to store long integers in the payload

#define NUM_LIGHTS  7 //max number of lights available (# of lights+1) 

String aGroups[11][2]; //current groups available in the gateway...
int aMoods[1][5] = {0, 0, 0, 0, 0}; //[0][0] group/mood - moods of the selected group

typedef struct  {
  String id;
  String name;
  String type;
  int status;
  int value;
  int color;
} light;

light aLights[NUM_LIGHTS]; //lights of the selected group [x][0]=id [x][1]=name [x][2]=type [x][3]=status
int aLightsStatus[11][4]; //lights status values of the selected group [x][0]=id [x][1]=status [x][2]=value, color=[x][3]
int c_light; //current light
int c_group; //current group
char c_group_id[10]; //current group id
int c_mood; //current mood
bool c_status = OFF; //switch current status
int c_switch_mode = SWITCH_MODE_HWSW;

boolean isDimmerStarted = false;

// -----------------------------------------------------------------
// ---- Wifi section ----*/

char wifiParams_ssid[32]; // = {"Astronomy-Domine"};
char wifiParams_passcode[64];// = {""};

// ---- Tradfri section ----*/
char tradfriParams_ip[15] = {"192.168.1.75"};
char tradfriParams_key[20] = {"FdkbvH1mxUFhb"};

WiFiClient http;    //Declare object of class HTTPClient
const int httpPort = 90;
char* host = "192.168.1.33";

int res;

Timer t0; //timer to schedule the sensors and actuators values update


/*
   Setup function. Here we do the basics
*/
void setup()
{

  // initialize arrays and pins
  //initialize light array. Set all pin to 999 (no light configured)
  clearLightsArrays();

  pinMode(PIN_RELE, OUTPUT);
  digitalWrite(PIN_RELE, LOW);
  Serial.begin(BAUD_RATE);
  nextionSerial.begin(57600);
  EEPROM.begin(512);

  //--- Callback attachement -----------------------//
  bconnect.attachCallback(&_bconnect); //wifi connect button
  btradfri.attachCallback(&_btradfri); //tradfri test button
  bswitch.attachCallback(&_bswitch); //main switch
  bdimmer.attachCallback(&_bdimmer); //dimmer control
  pdummy.attachCallback(&_pdummy); //default light
  ptimer.attachCallback(&_ptimer); //timer to resfresh light data from gateway

  bgroup.attachCallback(&_bgroup);
  blight1.attachCallback(&_blight1);
  blight2.attachCallback(&_blight2);
  blight3.attachCallback(&_blight3);
  blight4.attachCallback(&_blight4);
  blight5.attachCallback(&_blight5);
  blight6.attachCallback(&_blight6);
  bback.attachCallback(&_bback);
  //t_t1.attachCallback(&_t_t1);

  r_t0.attachCallback(&_r_t0);
  r_t1.attachCallback(&_r_t1);
  r_t2.attachCallback(&_r_t2);
  r_t3.attachCallback(&_r_t3);
  r_t4.attachCallback(&_r_t4);
  r_t5.attachCallback(&_r_t5);
  r_t6.attachCallback(&_r_t6);
  r_t7.attachCallback(&_r_t7);
  r_t8.attachCallback(&_r_t8);
  r_t9.attachCallback(&_r_t9);

  bmoncolor1.attachCallback(&_bmoncolor1);
  bmoncolor2.attachCallback(&_bmoncolor2);
  bmoncolor3.attachCallback(&_bmoncolor3);

  brgbcolor1.attachCallback(&_brgbcolor1);
  brgbcolor2.attachCallback(&_brgbcolor2);
  brgbcolor3.attachCallback(&_brgbcolor3);
  brgbcolor4.attachCallback(&_brgbcolor4);
  brgbcolor5.attachCallback(&_brgbcolor5);
  brgbcolor6.attachCallback(&_brgbcolor6);

  bloadgroups.attachCallback(&_bloadgroups);

  nex.init();
  delay(2000);
  refreshScreen();
  delay(100);
  start();

  t_default.setCycle(30000);

  pBit();
}  //setup()

void loop() {
  nex.poll();
  t0.update();
}

void start() {
  //putTradfriParams();
  //putWifiParams();
  getTradfriParams();
  getWifiParams();
  getGroupID();
  wifiTryConnect();
  if (aLights[0].id != "")
    getLights();
  getStatusLight();
  startTasks();
}


// *******************************************************//
// Functions  NEXTION                                            //
// *******************************************************//
boolean wifiTryConnect() {
  //se la pagina corrente non è wifi legge i parametri da eeprom
  Serial.print("Provo connessione ");
  //prova la connessione
  WiFi.mode(WIFI_STA);
  Serial.println(wifiParams_ssid);
  Serial.println(wifiParams_passcode);
  WiFi.begin((char*)wifiParams_ssid, (char*)wifiParams_passcode);

  unsigned long timeout = millis() + 5000;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (timeout < millis()) { //exit loop after 10 seconds
      break;
    }
  }
  if (WiFi.status() == WL_CONNECTED) { //connessione attiva
    //se connesso aggiorna variabile wifi su schermo a 1 altrimenti 0
    iwifi.show();
    vwifi.setValue(1);
    putWifiParams(); //scrive valori su eeprom
    Serial.println("Connesso");
    //crea HTTP Client
    delay(100);
    if (!http.connect(host, httpPort)) {
      Serial.println("connection failed");
    }
    return true;
  }
  else {
    iwifi.hide();
    vwifi.setValue(0);
    Serial.println("Non connesso");
    return false;
  }
}

boolean wifiCheckConnect() {
  if (WiFi.status() == WL_CONNECTED) { //connessione attiva
    //se connesso aggiorna variabile wifi su schermo a 1 altrimenti 0
    iwifi.show();
    putWifiParams(); //scrive valori su eeprom
    Serial.println("Connesso");
    return true;
  }
  else {
    iwifi.hide();
    Serial.println("Non connesso");
    wifiTryConnect();
    return false;
  }

}

void putWifiParams() {
  EEPROM.put(0, wifiParams_ssid);
  EEPROM.put(33, wifiParams_passcode);
  EEPROM.commit();
  setText("Wifi", "t_ssid", String(wifiParams_ssid));
  setText("Wifi", "t_pass", String(wifiParams_passcode));
}

void getWifiParams() {
  EEPROM.get(0, wifiParams_ssid);
  EEPROM.get(33, wifiParams_passcode);
  setText("Wifi", "t_ssid", String(wifiParams_ssid));
  setText("Wifi", "t_pass", String(wifiParams_passcode));
}

boolean wifiTradfriTestConnect() {

  //aggiunegere test qui
  boolean testResult = true;
  if (testResult == true) { //connessione attiva
    //se connesso aggiorna variabile wifi su schermo a 1 altrimenti 0
    putTradfriParams(); //scrive valori su eeprom
    Serial.println("Tradfri OK");
  }
  return testResult;
}

void putTradfriParams() {
  EEPROM.put(100, tradfriParams_ip);
  EEPROM.put(116, tradfriParams_key);
  EEPROM.commit();
  setText("Tradfri", "t_ip", String(tradfriParams_ip));
  setText("Tradfri", "t_key", String(tradfriParams_key));
}

int getTradfriParams() {
  EEPROM.get(100, tradfriParams_ip);
  EEPROM.get(116, tradfriParams_key);
  setText("Tradfri", "t_ip", String(tradfriParams_ip));
  setText("Tradfri", "t_key", String(tradfriParams_key));
}


// ******************************************************* //


// *******************************************************//
// Functions                                              //
// *******************************************************//

void getMoods() {
  // get available moods for selected group from gateway
}



int convStatus(String sts) {
  if (sts == "True")
    return 1;
  else
    return 0;
}


// ******************************************************* //
// Common functions                                        //
// ******************************************************* //

// ******************************************************* //

void startTasks() {
  // t0.every(TIMEt0, getStatusLight);
  t0.every(TIMEt1, calculateGroupStatus);

}


//--- Callback funcions ------------------------//
//--- Callback funcions ------------------------//
//--- Callback funcions ------------------------//
//void _t_t1(NextionEventType type, INextionTouchable *widget)
//{
//Serial.println(bdimmer.getValue());
//}

void _bconnect(NextionEventType type, INextionTouchable *widget)
{
  char buffer_ssid[40];
  char buffer_pass[40];
  if (type == NEX_EVENT_PUSH)
  {
    t_ssid.getText(buffer_ssid, 40);
    t_pass.getText(buffer_pass, 40);

    for (int i = 0; i < 40; i++)
    {
      wifiParams_passcode[i] = buffer_pass[i];
      wifiParams_ssid[i] = buffer_ssid[i];
    }

    if (wifiTryConnect())
      bconnect.setText("Connected!");
  }
}

void _bswitch(NextionEventType type, INextionTouchable *widget)
{
  String setValue;
  Serial.println("PUSH");
  Serial.println("boia de'");
  //get current status
  Serial.println(vst0.getValue());
  Serial.println(vst0.getValue());

  switch (c_light) {
    case 0: //It is the group
      // toggle status
      if (aLights[c_light].status  != vst0.getValue()) {
        pTic();
        if (aLights[c_light].status == OFF) {
          aLights[c_light].status = ON;
          aLights[c_light].value = bdimmer.getValue();
        }
        else if (aLights[c_light].status == ON) {
          aLights[c_light].status = OFF;
          aLights[c_light].value = 0;
        }
        if (c_switch_mode = SWITCH_MODE_HWSW) {
          digitalWrite(PIN_RELE, aLights[c_light].status);
        }
        else {
          digitalWrite(PIN_RELE, ON);
        }
      }
      else {
        if (aLights[c_light].status == ON) {
          aLights[c_light].value = bdimmer.getValue();
        }
        else {
          aLights[c_light].value = 0;
        }
      }
      //set all lights to the same values

      for (int i = 1; i < NUM_LIGHTS; i++) {
        //I don't use the library as this is faster
        if (aLights[i].status != 999) { //if the lights are visible
          aLights[i].status = aLights[0].status;
          aLights[i].value = aLights[0].value;
        }
        setValue = "st" + String(i) + ".val=" + int(aLights[i].status); //status
        sendCommand(setValue.c_str());
        setValue = "vl" + String(i) + ".val=" + int(aLights[i].value); //value
        sendCommand(setValue.c_str());
        //setValue = "cx" + String(i) + ".val=" + int(aLights[i].color); //color
        //sendCommand(setValue.c_str());
      }
      break;
    case 1:
      if (aLights[c_light].status  != vst1.getValue()) {
        pTic();
        if (aLights[c_light].status == OFF) {
          aLights[c_light].status = ON;
          aLights[c_light].value = bdimmer.getValue();
        }
        else if (aLights[c_light].status == ON) {
          aLights[c_light].status = OFF;
          aLights[c_light].value = 0;
        }
      }
      else {
        if (aLights[c_light].status == ON) {
          aLights[c_light].value = bdimmer.getValue();
        }
        else {
          aLights[c_light].value = 0;
        }

      }
      break;
    case 2:
      if (aLights[c_light].status  != vst2.getValue()) {
        pTic();
        if (aLights[c_light].status == OFF) {
          aLights[c_light].status = ON;
          aLights[c_light].value = bdimmer.getValue();
        }
        else if (aLights[c_light].status == ON) {
          aLights[c_light].status = OFF;
          aLights[c_light].value = 0;
        }
      }
      else {
        if (aLights[c_light].status == ON) {
          aLights[c_light].value = bdimmer.getValue();
        }
        else {
          aLights[c_light].value = 0;
        }

      }
      break;
    case 3:
      if (aLights[c_light].status  != vst3.getValue()) {
        pTic();
        if (aLights[c_light].status == OFF) {
          aLights[c_light].status = ON;
          aLights[c_light].value = bdimmer.getValue();
        }
        else if (aLights[c_light].status == ON) {
          aLights[c_light].status = OFF;
          aLights[c_light].value = 0;
        }
      }
      else {
        if (aLights[c_light].status == ON) {
          aLights[c_light].value = bdimmer.getValue();
        }
        else {
          aLights[c_light].value = 0;
        }

      }
      break;
    case 4:
      if (aLights[c_light].status  != vst4.getValue()) {
        pTic();
        if (aLights[c_light].status == OFF) {
          aLights[c_light].status = ON;
          aLights[c_light].value = bdimmer.getValue();
        }
        else if (aLights[c_light].status == ON) {
          aLights[c_light].status = OFF;
          aLights[c_light].value = 0;
        }
      }
      else {
        if (aLights[c_light].status == ON) {
          aLights[c_light].value = bdimmer.getValue();
        }
        else {
          aLights[c_light].value = 0;
        }

      }
      break;
    case 5:
      if (aLights[c_light].status  != vst5.getValue()) {
        pTic();
        if (aLights[c_light].status == OFF) {
          aLights[c_light].status = ON;
          aLights[c_light].value = bdimmer.getValue();
        }
        else if (aLights[c_light].status == ON) {
          aLights[c_light].status = OFF;
          aLights[c_light].value = 0;
        }
      }
      else {
        if (aLights[c_light].status == ON) {
          aLights[c_light].value = bdimmer.getValue();
        }
        else {
          aLights[c_light].value = 0;
        }
      }
      break;
    case 6:
      if (aLights[c_light].status  != vst6.getValue()) {
        pTic();
        if (aLights[c_light].status == OFF) {
          aLights[c_light].status = ON;
          aLights[c_light].value = bdimmer.getValue();
        }
        else if (aLights[c_light].status == ON) {
          aLights[c_light].status = OFF;
          aLights[c_light].value = 0;
        }
      }
      else {
        if (aLights[c_light].status == ON) {
          aLights[c_light].value = bdimmer.getValue();
        }
        else {
          aLights[c_light].value = 0;
        }
        break;
      }
  }
  pTic();
  setLight();
  Serial.println(aLights[c_light].value);
  Serial.println(bdimmer.getValue());
  Serial.println(vvl0.getValue());
  Serial.println("boiona");
}


void _bdimmer(NextionEventType type, INextionTouchable * widget)
{
  String setValue;
  //if it is ON read values
  switch (c_light) {
    case 0:
      aLights[c_light].status = vst0.getValue();
      aLights[c_light].value = bdimmer.getValue();

      for (int i = 1; i < NUM_LIGHTS; i++) {
        //I don't use the library as this is faster
        if (aLights[i].status != 999) { //if the lights are visible
          aLights[i].status = aLights[c_light].status;
          aLights[i].value = aLights[c_light].value;
        }
        setValue = "st" + String(i) + ".val=" + int(aLights[i].status); //status
        sendCommand(setValue.c_str());
        setValue = "vl" + String(i) + ".val=" + int(aLights[i].value); //value
        sendCommand(setValue.c_str());
        //setValue = "cx" + String(i) + ".val=" + int(aLights[i].color); //color
        //sendCommand(setValue.c_str());
      }

      break;
    case 1:
      aLights[c_light].status = vst1.getValue();
      aLights[c_light].value = bdimmer.getValue();
      break;
    case 2:
      aLights[c_light].status = vst2.getValue();
      aLights[c_light].value = bdimmer.getValue();
      break;
    case 3:
      aLights[c_light].status = vst3.getValue();
      aLights[c_light].value = bdimmer.getValue();
      break;
    case 4:
      aLights[c_light].status = vst4.getValue();
      aLights[c_light].value = bdimmer.getValue();
      break;
    case 5:
      aLights[c_light].status = vst5.getValue();
      aLights[c_light].value = bdimmer.getValue();
      break;
    case 6:
      aLights[c_light].status = vst6.getValue();
      aLights[c_light].value = bdimmer.getValue();
      break;
  }
  pTic();
  setLight();
  Serial.println(aLights[c_light].value);
  Serial.println(bdimmer.getValue());
  Serial.println("boiona te");
}

void _bgroup(NextionEventType type, INextionTouchable * widget)
{
  c_light = 0;
  aLights[c_light].status = vst0.getValue();
  aLights[c_light].value = vvl0.getValue();
  pTic();
  getStatusLight();
  refreshScreen();
  //getGroups();
}

void _blight1(NextionEventType type, INextionTouchable * widget)
{
  c_light = 1;
  aLights[c_light].status = vst1.getValue();
  aLights[c_light].value = vvl1.getValue();
  pTic();
  refreshScreen();
}

void _blight2(NextionEventType type, INextionTouchable * widget)
{
  c_light = 2;
  aLights[c_light].status = vst2.getValue();
  aLights[c_light].value = vvl2.getValue();
  pTic();
  refreshScreen();
}

void _blight3(NextionEventType type, INextionTouchable * widget)
{
  c_light = 3;
  aLights[c_light].status = vst3.getValue();
  aLights[c_light].value = vvl3.getValue();
  pTic();
  refreshScreen();
}

void _blight4(NextionEventType type, INextionTouchable * widget)
{
  c_light = 4;
  aLights[c_light].status = vst4.getValue();
  aLights[c_light].value = vvl4.getValue();
  pTic();
  refreshScreen();
}

void _blight5(NextionEventType type, INextionTouchable * widget)
{
  c_light = 5;
  aLights[c_light].status = vst5.getValue();
  aLights[c_light].value = vvl5.getValue();
  pTic();
  refreshScreen();
}

void _blight6(NextionEventType type, INextionTouchable * widget)
{
  c_light = 6;
  aLights[c_light].status = vst6.getValue();
  aLights[c_light].value = vvl6.getValue();
  pTic();
  refreshScreen();
}

void _bback(NextionEventType type, INextionTouchable * widget)
{
  Serial.println(NEX_EVENT_PUSH);
  if (type == NEX_EVENT_PUSH)
  {
    //read global variables
    c_light = vdeflight.getValue();
    c_switch_mode = vdefmainsw.getValue();
    refreshScreen();
  }
}

void _btradfri(NextionEventType type, INextionTouchable * widget)
{

  char buffer_ip[40];
  char buffer_key[40];
  if (type == NEX_EVENT_PUSH)
  {
    t_ip.getText(buffer_ip, 40);
    t_key.getText(buffer_key, 40);

    for (int i = 0; i < 40; i++)
    {
      tradfriParams_ip[i] = buffer_ip[i];
      tradfriParams_key[i] = buffer_key[i];
    }

    putTradfriParams();
  }

}


void _r_t0(NextionEventType type, INextionTouchable * widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 0;
    configGroup();
  }
}

void _r_t1(NextionEventType type, INextionTouchable * widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 1;
    configGroup();
  }
}

void _r_t2(NextionEventType type, INextionTouchable * widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 2;
    configGroup();
  }
}

void _r_t3(NextionEventType type, INextionTouchable * widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 3;
    configGroup();
  }
}

void _r_t4(NextionEventType type, INextionTouchable * widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 4;
    configGroup();
  }
}

void _r_t5(NextionEventType type, INextionTouchable * widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 5;
    configGroup();
  }
}

void _r_t6(NextionEventType type, INextionTouchable * widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 6;
    configGroup();
  }
}

void _r_t7(NextionEventType type, INextionTouchable * widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 7;
    configGroup();
  }
}

void _r_t8(NextionEventType type, INextionTouchable * widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 8;
    configGroup();
  }
}

void _r_t9(NextionEventType type, INextionTouchable * widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 9;
    configGroup();
  }
}


void _r_t10(NextionEventType type, INextionTouchable * widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 10;

  }
}

void configGroup() {
  aGroups[c_group][0].toCharArray(c_group_id, 10);
  aLights[0].id = aGroups[c_group][0];
  putGroupID();
  Serial.println(aLights[0].id);
  getLights();
}

void _bloadgroups(NextionEventType type, INextionTouchable * widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    Serial.println("eccolo!");
    getGroups();
  }
}

void _pdummy(NextionEventType type, INextionTouchable * widget)
{
  c_light = 0;
  Serial.println("default light");
  getStatusLight();
}

void _ptimer(NextionEventType type, INextionTouchable * widget)
{
  //c_light = 0;
  Serial.println("timer");
  getStatusLight();
}


//-----------------------------------------------------//
// 3 colors bulbs
//-----------------------------------------------------//
void _bmoncolor1(NextionEventType type, INextionTouchable * widget)
{
  //set cold color
  String url = createUrl(tradfriParams_ip, tradfriParams_key, "0", aLights[c_light].id, "setcolor", 1);
  pTic();
  execUrl(url, 0);
}

void _bmoncolor2(NextionEventType type, INextionTouchable * widget)
{
  //set normal color
  String url = createUrl(tradfriParams_ip, tradfriParams_key, "0", aLights[c_light].id, "setcolor", 2);
  pTic();
  execUrl(url, 0);
}

void _bmoncolor3(NextionEventType type, INextionTouchable * widget)
{
  //set warm color
  String url = createUrl(tradfriParams_ip, tradfriParams_key, "0", aLights[c_light].id, "setcolor", 3);
  pTic();
  execUrl(url, 0);
}


//-----------------------------------------------------//
// RGB colors bulbs
//-----------------------------------------------------//
void _brgbcolor1(NextionEventType type, INextionTouchable * widget)
{
  //set cold color //white
  String url = createUrl(tradfriParams_ip, tradfriParams_key, "0", aLights[c_light].id, "setcolor", 1);
  pTic();
  execUrl(url, 0);
}

void _brgbcolor2(NextionEventType type, INextionTouchable * widget)
{
  //set cold color //green
  String url = createUrl(tradfriParams_ip, tradfriParams_key, "0", aLights[c_light].id, "setcolor", 19);
  pTic();
  execUrl(url, 0);
}

void _brgbcolor3(NextionEventType type, INextionTouchable * widget)
{
  //set cold color //orange
  String url = createUrl(tradfriParams_ip, tradfriParams_key, "0", aLights[c_light].id, "setcolor", 8);
  pTic();
  execUrl(url, 0);
}

void _brgbcolor4(NextionEventType type, INextionTouchable * widget)
{
  //set cold color //red
  String url = createUrl(tradfriParams_ip, tradfriParams_key, "0", aLights[c_light].id, "setcolor", 11);
  pTic();
  execUrl(url, 0);
}

void _brgbcolor5(NextionEventType type, INextionTouchable * widget)
{
  //set cold color //purple
  String url = createUrl(tradfriParams_ip, tradfriParams_key, "0", aLights[c_light].id, "setcolor", 16);
  pTic();
  execUrl(url, 0);
}

void _brgbcolor6(NextionEventType type, INextionTouchable * widget)
{
  //set cold color //blue
  String url = createUrl(tradfriParams_ip, tradfriParams_key, "0", aLights[c_light].id, "setcolor", 17);
  pTic();
  execUrl(url, 0);
}
