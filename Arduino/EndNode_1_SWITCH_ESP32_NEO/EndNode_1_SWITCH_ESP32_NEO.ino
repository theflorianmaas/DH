// ------------------------------------------------------------ //
// EndNode_1_Switch
// V.0.1 3/11/2018
// First version 0.1 Development
//
// ------------------------------------------------------------ //
// Include libraries //
#include <Timer.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>

#include <Nextion.h>
#include <NextionPage.h>
#include <NextionButton.h>
#include <SoftwareSerial.h>
#include <NextionVariableString.h>
#include <INextionStringValued.h>
#include <NextionPicture.h>

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



//--------------------------------------------//
// Codes received from Nextion screen on serial
//--------------------------------------------//

#define NEX_RET_ON    0x99 //on
#define NEX_RET_OFF   0x88 //off
#define NEX_RET_DIMMER   0x55 //dimmer value
#define NEX_RET_COLOR1   0x25 //rgb color 1
#define NEX_RET_COLOR2   0x26 //rgb color 2
#define NEX_RET_COLOR3   0x27 //rgb color 3
#define NEX_RET_COLOR4   0x28 //rgb color 4
#define NEX_RET_COLOR5   0x29 //rgb color 5
#define NEX_RET_COLOR6   0x2a //rgb color 6
#define NEX_RET_COLORMONO1   0x14 //mono color 1
#define NEX_RET_COLORMONO2   0x15 //mono color 2
#define NEX_RET_COLORMONO3   0x16 //mono color 3
#define NEX_RET_MOOD1   0x3c //mood 1
#define NEX_RET_MOOD2   0x3b //mood 2
#define NEX_RET_MOOD3   0x3d //mood 3
#define NEX_RET_MOOD4   0x3e //mood 4
#define NEX_RET_MOOD4   0x3e //mood 4
#define NEX_RET_GROUP   0x70 //group
#define NEX_RET_LIGHT1  0x71 //light 1
#define NEX_RET_LIGHT2  0x72 //light 2
#define NEX_RET_LIGHT3  0x73 //light 3
#define NEX_RET_LIGHT4  0x74 //light 4
#define NEX_RET_LIGHT5  0x75 //light 5
#define NEX_RET_LIGHT6  0x76 //light 6
#define NEX_RET_STRDIM  0x90 //start dimmer
#define NEX_SEL_GROUP   0x40 //selected group
#define NEX_CON_WIFI    0x34 //connect WIFI

//-----------------------------------------------------------
// Nextion Display
//-----------------------------------------------------------
//-----------------------------------------------------------
#define ON  1
#define OFF 0
#define NEX_WIFI_CONNECTED_PICTURE 0
#define NEX_WIFI_NOTCONNECTED_PICTURE 1

boolean isDimmerStarted = false;

#define PAGE_MAIN 0

SoftwareSerial nextionSerial(NEXTION_RX, NEXTION_TX);// Nextion TX to pin 11 and RX to pin 10 of Arduino
Nextion nex(nextionSerial); //create a Nextion object named myNextion using the nextion serial port @ 9600bps

//------Pages---------------------------------------------------
NextionPage pgMain(nex, 0, 0, "Main");
NextionPage pgWifi(nex, 6, 0, "Wifi");

//----- Main ---------------------------//
NextionPicture bswitch(nex, 0, 0, "s1");
NextionPicture bmoncolor[4] = {(nex, 0, 0, "ml1"), (nex, 0, 0, "ml2"), (nex, 0, 0, "ml3"), (nex, 0, 0, "ml4")};
NextionPicture brgbcolor[8] = {(nex, 0, 0, "rl1"), (nex, 0, 0, "rl2"), (nex, 0, 0, "rl3"), (nex, 0, 0, "rl4"),(nex, 0, 0, "rl5"), (nex, 0, 0, "rl6"), (nex, 0, 0, "rl7"), (nex, 0, 0, "rl8")};
NextionPicture blight[6] = {(nex, 0, 0, "l1"), (nex, 0, 0, "l2"), (nex, 0, 0, "l3"), (nex, 0, 0, "l4"),(nex, 0, 0, "l5"), (nex, 0, 0, "l6")};
NextionPicture bgroup(nex, 0, 0, "group");
NextionPicture iwifi(nex, 0, 0, "wifi");

//----- Wifi ----------------------------//
INextionStringValued t_ssid(nex,6,2,"t_ssid");
INextionStringValued t_pass(nex,6,3,"t_pass");
NextionButton bconnect(nex,6,6,"bconn");
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

//Service command and status
#define READ          0
#define WRITE         1
#define ALARM         2
#define TIME          3
#define METEO         4
#define METEO_FCST    5
#define METEO_FCST_TEMP 6
#define SMLIGHT_COMMAND 7
#define SMLIGHT_CONFIG  8
#define SMLIGHT_GROUP 9

#define NOLIGHT -1
#define RESETALARM -999

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
#define NUM_ACTU      1 //Insert here the (# of actuators x NUM_DATA_VAL)
#define NUM_METH      5 //Insert here the (# of methods x NUM_DATA_VAL)
#define NUM_DATA      3 //Insert here the (# of sensors x NUM_DATA_VAL)
#define NUM_DATA_VAL  3 // number of values transmitted for each sensor: number, value, alarm status
#define NUM_DATA_PTS  NUM_DATA*NUM_DATA_VAL
#define NUM_ACTU_PTS  NUM_ACTU*NUM_DATA_VAL
#define NUM_BYTE_ARR  3 // Number of bytes of the array used to store long integers in the payload

#define NUM_LIGHTS  7 //max number of lights available (# of lights+1) 
#define NUM_LIGHT_PTS  NUM_LIGHTS*NUM_DATA_VAL

#define SENSOR 0
#define ACTUATOR 1
#define METHOD 2
#define SMCMD 99

#define RANGE_IN  1 //the actuator is in range 
#define RANGE_OUT 0 //the actuator is out of range 

#define NUM_RCVD_VAL  12 // Numero di valori ricevuri dal cordinatore 


/* ACTUATORS
  //0=actuator number
  //1=status
  //2=output type
  //3=value
*/
int actuators[NUM_ACTU][4] = {{PINNODE, 0, 0, 0}};
int aGroups[11]; //current group available in the gateway...
int aMoods[1][5] = {0, 0, 0, 0, 0}; //moods of the selected group
int aLights[NUM_LIGHTS][5]; //lights of the selected group [x][0]=pin [x][1]=type [x][2]=status [x][3]=value [x][4]=color
int c_light; //current light
int c_group; //current group
int c_mood; //current mood
/*

  // -----------------------------------------------------------------
  // ---- Wifi section ----*/
struct WifiObj {
  char* ssid;
  char* passcode;
};

WifiObj wifiParams = {"", ""};

// --- Xbee section ----*/

// Array to hold data received [0]=command 0=Read 1=Write [1]=pin number [2]=pin value
// es. [R][0][0] read all values, [W][1][100] set pin 1 to value 100
long RxData[NUM_RCVD_VAL];

int res;

Timer t0; //timer to schedule the sensors and actuators values update

/*
   Setup function. Here we do the basics
*/
void setup()
{

  // initialize arrays and pins
  for (int i = 0; i < NUM_RCVD_VAL; i++)
  {
    RxData[i] = 0;
  }
  //initialize light array. Set all pin to 999 (no light configured)
  for (int i = 1; i < NUM_LIGHTS; i++)
  {
    aLights[i][0] = NOLIGHT;
    aLights[i][1] = 0;
    aLights[i][2] = 999;
    aLights[i][3] = 0;
    aLights[i][4] = 1;
  }

  pinMode(PIN_RELE, OUTPUT);
  digitalWrite(PIN_RELE, LOW);
  Serial.begin(BAUD_RATE);
  nextionSerial.begin(57600);

  //--- Callback attachement -----------------------//
  bconnect.attachCallback(&_bconnect); //wifi connect button
  bswitch.attachCallback(&_bswitch); //main switch
  
  initScreenMain();
  
  //refreshScreen();
  pBit();
  //startTasks();
  wifiTryConnect();
}  //setup()

void loop(void)
{
  //t0.update();
  nex.poll();
}

// *******************************************************//
// Functions  NEXTION                                            //
// *******************************************************//
void initScreenMain() {
  // get available groups from gateway
  bswitch.setPictureID(OFF);
  for (int i = 0; i < 4, i++){
	  bmoncolor[i].hide();
  }  
  for (int i = 0; i < 8, i++){
	  brgbcolor[i].hide();
  }
  for (int i = 0; i < 6, i++){
	  blight[i].hide();
  }
  
  //nex.sendCommand("vis p7,0")
	
}

// ******************************************************* //


// ******************************************************* //
// ******************************************************* //

void getGroups() {
  // get available groups from gateway
}

void getMoods() {
  // get available moods for selected group from gateway
}

void getLights() {
  // get available lights assigned to the selected group from gateway
}

boolean wifiTryConnect() {
  //se la pagina corrente non è wifi legge i parametri da eeprom
  Serial.print("Provo connessione ");
  //prova la connessione
  WiFi.mode(WIFI_STA);
  Serial.println(wifiParams.ssid);
  Serial.println(wifiParams.passcode);
  WiFi.begin(wifiParams.ssid, wifiParams.passcode);

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
    iwifi.setPictureID(NEX_WIFI_CONNECTED_PICTURE)
    putWifiParams(); //scrive valori su eeprom
    Serial.println("Connesso");
    pBit();
    return true;
  }
  else {
    iwifi.setPictureID(NEX_WIFI_NOTCONNECTED_PICTURE)
    Serial.println("Non connesso");
    return false;
  }
}

boolean wifiCheckConnect() {
  if (WiFi.status() == WL_CONNECTED) { //connessione attiva
    //se connesso aggiorna variabile wifi su schermo a 1 altrimenti 0
    iwifi.setPictureID(NEX_WIFI_CONNECTED_PICTURE)
    putWifiParams(); //scrive valori su eeprom
    Serial.println("Connesso");
    pBit();
    return true;
  }
  else {
    iwifi.setPictureID(NEX_WIFI_NOTCONNECTED_PICTURE)
    Serial.println("Non connesso");
	wifiTryConnect();
    return false;
  }
	
}

void putWifiParams() {
  EEPROM.put(0, wifiParams);
}

int getWifiParams() {
  EEPROM.get(0, wifiParams);
}

//--- Callsback funcions ------------------------//
void _bconnect(NextionEventType type, INextionTouchable *widget)
{
  char buffer_ssid[30];
  char buffer_pass[30];
  if (type == NEX_EVENT_PUSH)
  {
    digitalWrite(13, HIGH);
    //wifiParams.ssid = buffer;
    if (t_ssid.getText(buffer_ssid, 30)) {
      Serial.println(buffer_ssid);
      wifiParams.ssid = buffer_ssid;
    }
	
    if (t_pass.getText(buffer_pass, 30)) {
      Serial.println(buffer_pass);
      wifiParams.passcode = buffer_pass;
    }
    if (wifiTryConnect())
      bconnect.setText("Connected!");
  }
  else if (type == NEX_EVENT_POP)
  {
    digitalWrite(13, LOW);
  }
}

void _bswitch(NextionEventType type, INextionTouchable *widget)
{
	//
}


