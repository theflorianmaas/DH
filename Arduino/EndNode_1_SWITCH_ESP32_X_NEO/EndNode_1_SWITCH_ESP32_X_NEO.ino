// ------------------------------------------------------------ //
// EndNode_1_Switch
// V.0.1 26/11/2018
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

NextionPicture bgroup(nex, 0, 19, "g0");
NextionPicture iwifi(nex, 0, 72, "i_wifi");
NextionVariableNumeric vwifi(nex, 0, 71, "wifi");
NextionVariableNumeric vdefLight(nex, 0, 68, "defLight");
NextionVariableNumeric vdefMainSW(nex, 0, 69, "defMainSW");

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

#define NOLIGHT -1

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

#define TIMEt0 10000 //update sensor data
#define TIMEt1 3000 //send data to coordinator
#define TIMEt2 11000 //set time

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

/*

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
  clearLightsArrays(0);
  clearLightsArrays(1);

  pinMode(PIN_RELE, OUTPUT);
  digitalWrite(PIN_RELE, LOW);
  Serial.begin(BAUD_RATE);
  nextionSerial.begin(57600);
  EEPROM.begin(512);

  //--- Callback attachement -----------------------//
  bconnect.attachCallback(&_bconnect); //wifi connect button
  btradfri.attachCallback(&_btradfri); //tradfri test button
  bswitch.attachCallback(&_bswitch); //main switch
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

  bloadgroups.attachCallback(&_bloadgroups);

  nex.init();
  delay(2000);
  refreshScreen();
  delay(100);
  start();
  pBit();
  //startTasks();
}  //setup()

void loop(void)
{
  //t0.update();
  nex.poll();
}

void start() {
  //putTradfriParams();
  //putWifiParams();
  getTradfriParams();
  getWifiParams();
  getGroupID();
  wifiTryConnect();
  if (c_group_id != "")
    getLights();
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
//-----------------------------------
// Read data from gateway
//------------------------------------
void getGroups() {

  String url = createUrl(tradfriParams_ip, tradfriParams_key, "0", "0", "listgroup", 0);
  String result = execUrl(url);
  char *str = (char*)result.c_str();
  Serial.println(result);
  String arr[41]; //max 10 groups. 4 values per group
  //arr[0] = command
  //arr[1] = group id
  //arr[2] = group name
  //arr[3] = not used
  //arr[4] = not used
  //...

  char *p = strtok(str, ",");
  int index = 0;

  while (p != nullptr && index < 20) {
    arr[index++] = String(p);
    p = strtok(NULL, ",");
  }

  // memset(aGroups, 0, sizeof(aGroups)); //reset group array
  int idx = 0;
  //if (String(arr[0]) == String("listgroup")) {
  //read received groups from gateway
  for (int i = 1; i < index; i = i + 4) {
    aGroups[idx][0] = arr[i];
    aGroups[idx][1] = arr[i + 1];
    if (aGroups[idx][0] == c_group_id) {
      c_group = idx;
    }
    idx++;
  }
  // }
  setGroupList(idx);
}

void getMoods() {
  // get available moods for selected group from gateway
}

void getLights() {
  // get available lights assigned to the selected group from gateway
  String url = createUrl(tradfriParams_ip, tradfriParams_key, c_group_id, "0", "listlight", 0);
  String result = execUrl(url);
  char *str = (char*)result.c_str();
  Serial.println(result);
  String arr[NUM_LIGHTS * 6 + 1]; //max 10 lights. 4 values per light
  //arr[0] = command
  //arr[1] = light id
  //arr[2] = type
  //arr[3] = status
  //arr[4] = value
  //...

  char *p = strtok(str, ",");
  int index = 0;

  while (p != nullptr && index < NUM_LIGHTS * 6 + 1) {
    arr[index++] = String(p);
    p = strtok(NULL, ",");
  }

  clearLightsArrays(0);
  int idx = 0;
  //if (String(arr[0]) == String("listgroup")) {
  //read received groups from gateway
  for (int i = 1; i < index; i = i + 6) {
    aLights[idx].id = arr[i];
    aLights[idx].name = arr[i + 1];
    aLights[idx].type = arr[i + 2];
    aLights[idx].status = convStatus(arr[i + 3]);
    aLights[idx].value = arr[i + 4].toInt();
    aLights[idx].color = arr[i + 5].toInt();
    idx++;
  }
  // }
  setLightList(idx);

  for (int i = 0; i < idx; i++) {
    Serial.println(aLights[i].id);
    Serial.println(aLights[i].name);
    Serial.println(aLights[i].type);
    Serial.println(aLights[i].status);
  }
  Serial.println("------------------");
  Serial.println(index);
  refreshScreen;
}

int convStatus(String sts){
  if (sts = "True"){
    return 1;
  }else
  {
    return 0;
  }
}


// ******************************************************* //

// ******************************************************* //

// ******************************************************* //


// ******************************************************* //
// Common functions                                        //
// ******************************************************* //

// ******************************************************* //

void startTasks() {
  //t0.every(TIMEt1, sendSensorData);
  //t0.every(TIMEt0, updatePinValues);
}


//--- Callsback funcions ------------------------//
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
  if (type == NEX_EVENT_PUSH)
  {
    Serial.println("boia de'");
    //get current status
    Serial.println(bswitch.getPictureID());
    if (c_status != bswitch.getPictureID()) {
      pTic();
      if (c_status == OFF) {
        c_status = ON;
      }
      else if (c_status == ON) {
        c_status = OFF;
      }
      if (c_switch_mode = SWITCH_MODE_HWSW) {
        digitalWrite(PIN_RELE, c_status);
      }
      else {
        digitalWrite(PIN_RELE, ON);
      }
      refreshScreen();
    }
    else { //se non cmabia stato ed è ON legge valori dimmer
      if (c_status == ON) {

      }
    }
  }

  if (type == NEX_EVENT_POP)
  {
    pTic();
    Serial.println(bdimmer.getValue());
    Serial.println("boiona");
  }

}

void _bgroup(NextionEventType type, INextionTouchable *widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_light = 0;
  }
  //getGroups();
}

void _blight1(NextionEventType type, INextionTouchable *widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_light = 1;
    refreshScreen();
  }
}

void _blight2(NextionEventType type, INextionTouchable *widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_light = 2;
    refreshScreen();
  }
}

void _blight3(NextionEventType type, INextionTouchable *widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_light = 3;
    refreshScreen();
  }
}

void _blight4(NextionEventType type, INextionTouchable *widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_light = 4;
    refreshScreen();
  }
}

void _blight5(NextionEventType type, INextionTouchable *widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_light = 5;
    refreshScreen();
  }
}

void _blight6(NextionEventType type, INextionTouchable *widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_light = 6;
    refreshScreen();
  }
}

void _bback(NextionEventType type, INextionTouchable *widget)
{
  Serial.println(NEX_EVENT_PUSH);
  if (type == NEX_EVENT_PUSH)
  {
    //read global variables
    c_light = vdefLight.getValue();
    c_switch_mode = vdefMainSW.getValue();
    refreshScreen();
  }
}

void _btradfri(NextionEventType type, INextionTouchable *widget)
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


void _r_t0(NextionEventType type, INextionTouchable *widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 0;
    configGroup();
  }
}

void _r_t1(NextionEventType type, INextionTouchable *widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 1;
    configGroup();
  }
}

void _r_t2(NextionEventType type, INextionTouchable *widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 2;
    configGroup();
  }
}

void _r_t3(NextionEventType type, INextionTouchable *widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 3;
    configGroup();
  }
}

void _r_t4(NextionEventType type, INextionTouchable *widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 4;
    configGroup();
  }
}

void _r_t5(NextionEventType type, INextionTouchable *widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 5;
    configGroup();
  }
}

void _r_t6(NextionEventType type, INextionTouchable *widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 6;
    configGroup();
  }
}

void _r_t7(NextionEventType type, INextionTouchable *widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 7;
    configGroup();
  }
}

void _r_t8(NextionEventType type, INextionTouchable *widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 8;
    configGroup();
  }
}

void _r_t9(NextionEventType type, INextionTouchable *widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 9;
    configGroup();
  }
}


void _r_t10(NextionEventType type, INextionTouchable *widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    c_group = 10;

  }
}

void configGroup() {
  aGroups[c_group][0].toCharArray(c_group_id, 10);
  putGroupID();
  Serial.println(c_group_id);
  getLights();
}

void _bloadgroups(NextionEventType type, INextionTouchable *widget)
{
  if (type == NEX_EVENT_PUSH)
  {
    Serial.println("eccolo!");
    getGroups();
  }
}
