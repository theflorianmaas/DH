// ------------------------------------------------------------ //
// EndNode_1_Switch
// V.0.1 24/02/2019
// First version 0.1 Development
//
// ------------------------------------------------------------ //
// Include libraries //
#include <Timer.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include "pitches.h"
#include "config.h"

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Midea.h>

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
NextionPage pgReset(nex, 8, 0, "Reset");

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
NextionPicture psettings(nex, 0, 67, "p0"); //settings icon
NextionPicture ptv(nex, 0, 14, "tv"); //settings icon
NextionPicture pac(nex, 0, 9, "ac"); //settings icon

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

//----- Options ----------------------------//
NextionRadioButton r_l0(nex, 4, 4, "l0");
NextionRadioButton r_l1(nex, 4, 5, "l1");
NextionRadioButton r_l2(nex, 4, 6, "l2");
NextionRadioButton r_l3(nex, 4, 7, "l3");
NextionRadioButton r_l4(nex, 4, 8, "l4");
NextionRadioButton r_l5(nex, 4, 9, "l5");
NextionRadioButton r_l6(nex, 4, 10, "l6");
NextionRadioButton r_msHWSW(nex, 4, 18, "msHWSW");
NextionRadioButton r_msSW(nex, 4, 19, "msSW");

//----- Wifi ----------------------------//
INextionStringValued t_ssid(nex, 6, 2, "t_ssid");
INextionStringValued t_pass(nex, 6, 3, "t_pass");
NextionButton bconnect(nex, 6, 6, "bconn");
INextionStringValued t_ip(nex, 7, 2, "t_ip");
INextionStringValued t_key(nex, 7, 3, "t_key");
NextionButton btradfri(nex, 7, 6, "btradfri");

//----- AC ----------------------------//
NextionPicture acon(nex, 1, 7, "pon");
NextionPicture acfanup(nex, 1, 12, "pfu");
NextionPicture acfandown(nex, 1, 11, "pfd");
NextionPicture actempup(nex, 1, 9, "ptu");
NextionPicture actempdown(nex, 1, 10, "ptd");
NextionPicture acswing(nex, 1, 14, "ps");
NextionPicture acmode(nex, 1, 13, "pm");
NextionPicture bbackac(nex, 1, 8, "p1");
NextionVariableNumeric acvst(nex, 1, 15, "st");
NextionVariableNumeric acvmd(nex, 1, 16, "md");
NextionVariableNumeric acvsw(nex, 1, 17, "sw");
NextionVariableNumeric acvfa(nex, 1, 18, "fa");

//----- TV ----------------------------//
NextionPicture tvon(nex, 2, 7, "pon");
NextionPicture tvvolup(nex, 2, 12, "pu");
NextionPicture tvvoldown(nex, 2, 11, "pd");
NextionPicture tvchannelup(nex, 2, 9, "pr");
NextionPicture tvchanneldown(nex, 2, 10, "pl");
NextionPicture tvsource(nex, 2, 14, "px");
NextionPicture tvok(nex, 2, 13, "pok");
NextionPicture tvret(nex, 2, 15, "pret");
NextionPicture bbacktv(nex, 2, 8, "p1");

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

//----- Reset ----------------------------//
NextionButton breset(nex, 8, 2, "rs");
//-----------------------------------------------------------

//-----------------------------------------------------------
void calculateGroupStatus();
void getStatusLight();
void enableTask();
void clearLightsArrays();
//-----------------------------------------------------------
Timer t0; //timer to schedule the sensors and actuators values update

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
int c_light = 0; //current light
int c_group; //current group
char c_group_id[10]; //current group id
int c_mood; //current mood
bool c_status = OFF; //switch current status
int c_switch_mode = SWITCH_MODE_HWSW;

boolean isDimmerStarted = false;
boolean runningTimer = true;

// -----------------------------------------------------------------
// ---- Wifi section ----*/

char wifiParams_ssid[32]; // = {"ssid"};
char wifiParams_passcode[64]; // = {"password"};

// ---- Tradfri section ----*/
char tradfriParams_ip[15] = {"192.168.1.75"};
char tradfriParams_key[20] = {"FdkbvH1mxUFhb"};

WiFiClient http;    //Declare object of class HTTPClient
AsyncClient* client = new AsyncClient;
String line;
bool loadString = false;


// ------------------------------------------------------------------
// Remote control section -------------------------------------------
// ------------------------------------------------------------------
IRsend irTV(LED_IR);

// ---------- AC Remote control section ------------------------//
// Support for Midea, Hokkaido HVAC, Type:R51M/E remote control //
IRMideaAC irAC(LED_IR);

/*
   Setup function. Here we do the basics
*/
void setup()
{
  irTV.begin();
  irAC.begin();
  irAC.stateReset();
  //initialize arrays and pins
  //initialize light array. Set all pin to 999 (no light configured)
  clearLightsArrays();

  pinMode(PIN_RELE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
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
  psettings.attachCallback(&_psettings); //settings icon
  ptv.attachCallback(&_ptv); //tv icon
  pac.attachCallback(&_pac); //ac icon

  bgroup.attachCallback(&_bgroup);
  blight1.attachCallback(&_blight1);
  blight2.attachCallback(&_blight2);
  blight3.attachCallback(&_blight3);
  blight4.attachCallback(&_blight4);
  blight5.attachCallback(&_blight5);
  blight6.attachCallback(&_blight6);
  bback.attachCallback(&_bback);

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

  //----- Options ------------------------//
  r_msHWSW.attachCallback(&_r_msHWSW);
  r_msSW.attachCallback(&_r_msSW);

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

  //----- AC ----------------------------//
  acon.attachCallback(&_acon);
  acfanup.attachCallback(&_acfanup);
  acfandown.attachCallback(&_acfandown);
  actempup.attachCallback(&_actempup);
  actempdown.attachCallback(&_actempdown);
  acswing.attachCallback(&_acswing);
  acmode.attachCallback(&_acmode);
  bbackac.attachCallback(&_bbackac);

  //----- TV ----------------------------//
  tvon.attachCallback(&_tvon);
  tvvolup.attachCallback(&_tvvolup);
  tvvoldown.attachCallback(&_tvvoldown);
  tvchannelup.attachCallback(&_tvchannelup);
  tvchanneldown.attachCallback(&_tvchanneldown);
  tvsource.attachCallback(&_tvsource);
  tvok.attachCallback(&_tvok);
  tvret.attachCallback(&_tvret);
  bbacktv.attachCallback(&_bbacktv);
  //----- Reset ----------------------------//
  breset.attachCallback(&_breset);

  nex.init();
  delay(1000);
  refreshScreen();
  delay(100);
  start();

  t_default.setCycle(60000);
  pBit();
}  //setup()

void loop() {
  nex.poll();
  if (runningTimer == true)
    t0.update();
}

void start() {
  //putWifiParams();
  //putTradfriParams();;
  getTradfriParams();
  getWifiParams();
  getGroupID();
  wifiTryConnect();
  if (aLights[0].id != "")
    getLights();
  delay(3000);
  if (vdefmainsw.getValue() == SWITCH_MODE_HWSW) {
    c_switch_mode = SWITCH_MODE_HWSW;
  }
  else   if (vdefmainsw.getValue() == SWITCH_MODE_SW) {
    c_switch_mode = SWITCH_MODE_SW;
  }
  getStatusLight();
  enableTask();
}

// *******************************************************//
// Functions  NEXTION                                            //
// *******************************************************//
boolean wifiTryConnect() {
  //se la pagina corrente non è wifi legge i parametri da eeprom
  Serial.print("Provo connessione ");
  //prova la connessione
  WiFi.mode(WIFI_STA);
  WiFi.begin((char*)wifiParams_ssid, (char*)wifiParams_passcode);
  //unsigned long timeout = millis() + 5000;
  WiFi.reconnect();
  while (WiFi.status() != WL_CONNECTED) {
    wifiOff();
    delay(500);
    Serial.print(".");
    wifiOn();
    nex.poll();
  }
  if (WiFi.status() == WL_CONNECTED) { //connessione attiva
    //se connesso aggiorna variabile wifi su schermo a 1 altrimenti 0
    //iwifi.show();
    vwifi.setValue(ON);
    putWifiParams(); //scrive valori su eeprom
    Serial.println("Connesso");
    //crea HTTP Client
    nex.poll();
    //connecy TCP client
    client->onData(&handleData, client);
    client->onConnect(&onConnect, client);

    while (!client->connected()) {
      Serial.print('.');
      delay(500);
      client->connect(SERVER_HOST_NAME, TCP_PORT);
      nex.poll();
      wifiCheckConnect();
      return true;
    }
  }
  else {
    vwifi.setValue(OFF);
    Serial.println("Non connesso");
    wifiCheckConnect();
    return false;
  }
}

void wifiCheckConnect() {
  if (WiFi.status() != WL_CONNECTED) { //connessione attiva
    wifiOff();
    Serial.println("Non connesso");
    WiFi.reconnect();
    nex.poll();
    if (WiFi.status() == WL_CONNECTED) {
      wifiRed();
      wifiOn();
      Serial.println("riconnesso WIFI");
    }
  }
  else {
    if (client->freeable()) {
      wifiRed();
      client->connect(SERVER_HOST_NAME, TCP_PORT);
      Serial.println("tentata riconnessione TCP");
      nex.poll();
    }
    else {
      wifiGreen();
    }
  }
}

void wifiOn() {
  String cmd = "vis i_wifi,1";
  digitalWrite(LED_GREEN, HIGH);
  sendCommand(cmd.c_str());
  nex.poll();
}

void wifiOff() {
  String cmd = "vis i_wifi,0";
  digitalWrite(LED_GREEN, LOW);
  sendCommand(cmd.c_str());
  nex.poll();
}

void wifiGreen() {
  String cmd = "i_wifi.pic=54";
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);
  sendCommand(cmd.c_str());
  nex.poll();
}

void wifiRed() {
  String cmd = "i_wifi.pic=53";
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);
  sendCommand(cmd.c_str());
  nex.poll();
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

void enableTask() {
  t0.every(TIMEt1, wifiCheckConnect);
  t0.every(TIMEt0, getStatusLight);
  //}
}

//--- Callback funcions ------------------------//
//--- Callback funcions ------------------------//
//--- Callback funcions ------------------------//

void _bconnect(NextionEventType type, INextionTouchable * widget)
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
  nex.poll();
}

void _bswitch(NextionEventType type, INextionTouchable * widget)
{
  String setValue;
  if (type == NEX_EVENT_PUSH)
  {
    isDimmerStarted = true;
  }
  else if (type == NEX_EVENT_POP) {
    isDimmerStarted = false;
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
          if (c_switch_mode == SWITCH_MODE_HWSW) {
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
    nex.poll();
    setLight();
  }
  nex.poll();
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
  nex.poll();
  setLight();
  nex.poll();
  Serial.println(aLights[c_light].value);
  Serial.println(bdimmer.getValue());
  Serial.println("boiona de!");
}

void _bgroup(NextionEventType type, INextionTouchable * widget)
{
  c_light = 0;
  aLights[c_light].status = vst0.getValue();
  aLights[c_light].value = vvl0.getValue();
  pTic();
  nex.poll();
  getStatusLight();
  nex.poll();
  refreshScreen();
  nex.poll();
  //getGroups();
}

void _blight1(NextionEventType type, INextionTouchable * widget)
{
  c_light = 1;
  aLights[c_light].status = vst1.getValue();
  aLights[c_light].value = vvl1.getValue();
  pTic();
  nex.poll();
  refreshScreen();
  nex.poll();
}

void _blight2(NextionEventType type, INextionTouchable * widget)
{
  c_light = 2;
  aLights[c_light].status = vst2.getValue();
  aLights[c_light].value = vvl2.getValue();
  pTic();
  nex.poll();
  refreshScreen();
  nex.poll();
}

void _blight3(NextionEventType type, INextionTouchable * widget)
{
  c_light = 3;
  aLights[c_light].status = vst3.getValue();
  aLights[c_light].value = vvl3.getValue();
  pTic();
  nex.poll();
  refreshScreen();
  nex.poll();
}

void _blight4(NextionEventType type, INextionTouchable * widget)
{
  c_light = 4;
  aLights[c_light].status = vst4.getValue();
  aLights[c_light].value = vvl4.getValue();
  pTic();
  nex.poll();
  refreshScreen();
  nex.poll();
}

void _blight5(NextionEventType type, INextionTouchable * widget)
{
  c_light = 5;
  aLights[c_light].status = vst5.getValue();
  aLights[c_light].value = vvl5.getValue();
  pTic();
  nex.poll();
  refreshScreen();
  nex.poll();
}

void _blight6(NextionEventType type, INextionTouchable * widget)
{
  c_light = 6;
  aLights[c_light].status = vst6.getValue();
  aLights[c_light].value = vvl6.getValue();
  pTic();
  nex.poll();
  refreshScreen();
  nex.poll();
}

void _bback(NextionEventType type, INextionTouchable * widget)
{
  //read global variables
  Serial.println("Eccomiiiii");
  pTic();
  c_light = vdeflight.getValue();
  c_switch_mode = vdefmainsw.getValue();
  nex.poll();
  refreshScreen();
  nex.poll();
  runningTimer = true;
}


void _bbacktv(NextionEventType type, INextionTouchable * widget)
{
  //read global variables
  c_light = vdeflight.getValue();
  c_switch_mode = vdefmainsw.getValue();
  nex.poll();
  refreshScreen();
  nex.poll();
  runningTimer = true;
  pTic();
}


void _bbackac(NextionEventType type, INextionTouchable * widget)
{
  //read global variables
  c_light = vdeflight.getValue();
  c_switch_mode = vdefmainsw.getValue();
  nex.poll();
  refreshScreen();
  nex.poll();
  runningTimer = true;
  pTic();
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

void _bloadgroups(NextionEventType type, INextionTouchable * widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    pTic();
    nex.poll();
    getGroups();
    refreshScreen();
    nex.poll();
  }
}

void _pdummy(NextionEventType type, INextionTouchable * widget)
{
  c_light = vdeflight.getValue();
  Serial.print("default light ");
  Serial.println(c_light);
  getStatusLight();
  nex.poll();
}

void _psettings(NextionEventType type, INextionTouchable * widget)
{
  pTic();
  runningTimer = false;
}

void _ptv(NextionEventType type, INextionTouchable * widget)
{
  pTic();
  runningTimer = false;
}

void _pac(NextionEventType type, INextionTouchable * widget)
{
  pTic();
  runningTimer = false;
}

//-----------------------------------------------------//
// 3 colors bulbs
//-----------------------------------------------------//
void _bmoncolor1(NextionEventType type, INextionTouchable * widget)
{
  //set cold color
  String url = createUrl(tradfriParams_ip, tradfriParams_key, "0", aLights[c_light].id, "setcolor", 1);
  pTic();
  execUrl(url);
  nex.poll();
}

void _bmoncolor2(NextionEventType type, INextionTouchable * widget)
{
  //set normal color
  String url = createUrl(tradfriParams_ip, tradfriParams_key, "0", aLights[c_light].id, "setcolor", 2);
  pTic();
  execUrl(url);
  nex.poll();
}

void _bmoncolor3(NextionEventType type, INextionTouchable * widget)
{
  //set warm color
  String url = createUrl(tradfriParams_ip, tradfriParams_key, "0", aLights[c_light].id, "setcolor", 3);
  pTic();
  execUrl(url);
  nex.poll();
}


//-----------------------------------------------------//
// RGB colors bulbs
//-----------------------------------------------------//
void _brgbcolor1(NextionEventType type, INextionTouchable * widget)
{
  //set cold color //white
  String url = createUrl(tradfriParams_ip, tradfriParams_key, "0", aLights[c_light].id, "setcolor", 1);
  pTic();
  execUrl(url);
  nex.poll();
}

void _brgbcolor2(NextionEventType type, INextionTouchable * widget)
{
  //set cold color //green
  String url = createUrl(tradfriParams_ip, tradfriParams_key, "0", aLights[c_light].id, "setcolor", 19);
  pTic();
  execUrl(url);
  nex.poll();
}

void _brgbcolor3(NextionEventType type, INextionTouchable * widget)
{
  //set cold color //orange
  String url = createUrl(tradfriParams_ip, tradfriParams_key, "0", aLights[c_light].id, "setcolor", 8);
  pTic();
  execUrl(url);
  nex.poll();
}

void _brgbcolor4(NextionEventType type, INextionTouchable * widget)
{
  //set cold color //red
  String url = createUrl(tradfriParams_ip, tradfriParams_key, "0", aLights[c_light].id, "setcolor", 11);
  pTic();
  execUrl(url);
  nex.poll();
}

void _brgbcolor5(NextionEventType type, INextionTouchable * widget)
{
  //set cold color //purple
  String url = createUrl(tradfriParams_ip, tradfriParams_key, "0", aLights[c_light].id, "setcolor", 16);
  pTic();
  execUrl(url);
  nex.poll();
}

void _brgbcolor6(NextionEventType type, INextionTouchable * widget)
{
  //set cold color //blue
  String url = createUrl(tradfriParams_ip, tradfriParams_key, "0", aLights[c_light].id, "setcolor", 17);
  pTic();
  execUrl(url);
  nex.poll();
}

void _tvon(NextionEventType type, INextionTouchable * widget)
{
  pTic();
  tv_remote(TV0NOFF, TV_SONY);
}

void _tvvolup(NextionEventType type, INextionTouchable * widget)
{
  pTic();
  tv_remote(TVVOLUMEUP, TV_SONY);
}

void _tvvoldown(NextionEventType type, INextionTouchable * widget)
{
  pTic();
  tv_remote(TVVOLUMEDOWN, TV_SONY);
}

void _tvchannelup(NextionEventType type, INextionTouchable * widget)
{
  pTic();
  tv_remote(TVCHANNELUP, TV_SONY);
}

void _tvchanneldown(NextionEventType type, INextionTouchable * widget)
{
  pTic();
  tv_remote(TVCHANNELDOWN, TV_SONY);
}

void _tvsource(NextionEventType type, INextionTouchable * widget)
{
  pTic();
  tv_remote(TVHDMI, TV_SONY);
}

void _tvok(NextionEventType type, INextionTouchable * widget)
{
  pTic();
  tv_remote(TVKEYOK, TV_SONY);
}

void _tvret(NextionEventType type, INextionTouchable * widget)
{
  pTic();
  //tv_remote(TVKEYRETURN, TV_SONY);
}

#define ACCOFF      0
#define ACCON       1
#define ACCTEMP     2
#define ACCMODE     3
#define ACCFAN      4
#define ACCSWING    5

//AC methods HVAC
#define ACFAN1        3
#define ACFAN2        4
#define ACFAN3        5
#define ACFANAUTO     6
#define ACMODECOOL    7
#define ACMODEDRY     8
#define ACMODEHEAT    9
#define ACMODEAUTO    10
#define ACTEMPERATURE 11
#define ACSWING       12

//------AC--------------------------------------------
void _acon(NextionEventType type, INextionTouchable * widget)
{
  pTic();
  ac_remote(ACPOWER, AC_MIDEA);
}

void _acfanup(NextionEventType type, INextionTouchable * widget)
{
  pTic();
  ac_remote(ACFAN, AC_MIDEA);
}

void _acfandown(NextionEventType type, INextionTouchable * widget)
{
  pTic();
  ac_remote(ACFAN, AC_MIDEA);
}

void _actempup(NextionEventType type, INextionTouchable * widget)
{
  pTic();
  ac_remote(ACTEMPUP, AC_MIDEA);
}

void _actempdown(NextionEventType type, INextionTouchable * widget)
{
  pTic();
  ac_remote(ACTEMPDOWN, AC_MIDEA);
}

void _acswing(NextionEventType type, INextionTouchable * widget)
{
  pTic();
  delay(10);
  if (acvsw.getValue() == 0) {
    ac_remote(ACSWING, AC_MIDEA);
  }
  else if (acvst.getValue() == 1) {
    ac_remote(ACSWING, AC_MIDEA);
  }
}

void _acmode(NextionEventType type, INextionTouchable * widget)
{
  pTic();
  ac_remote(ACMODE, AC_MIDEA);
}

void _r_msHWSW(NextionEventType type, INextionTouchable * widget)
{
  pTic();
  c_switch_mode == SWITCH_MODE_HWSW;
}

void _r_msSW(NextionEventType type, INextionTouchable * widget)
{
  pTic();
  c_switch_mode == SWITCH_MODE_SW;
  digitalWrite(PIN_RELE, ON);
}

void _breset(NextionEventType type, INextionTouchable * widget)
{
  pTic();
  ESP.restart();
}
