// ------------------------------------------------------------ //
// EndNode_1_Switch
// V.1 27/11/2017
// First version
//
// ------------------------------------------------------------ //
// Include libraries //
#include <XBee.h>
#include <Timer.h>
#include <SoftwareSerial.h>
#include <Nextion.h>
//#include <EEPROM.h>


// ------------------------------------------------------------ //
//-----------------------------------------------------------
#define PINNODE 0
//-----------------------------------------------------------
// reserved pins
//-----------------------------------------------------------
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
const int pinBattery PROGMEM = A0;
#define IR_EMITER 3 //pin for IR Led 
#define PINt 24 //virtual pin for temperature
#define PINh 25 //virtual pin for humidity
#define NEXTION_RX 10
#define NEXTION_TX 11
//-----------------------------------------------------------
// Nextion Display
//-----------------------------------------------------------
//-----------------------------------------------------------
#define ON  1
#define OFF 0

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

#define PAGE_MAIN 0

SoftwareSerial nextion(NEXTION_RX, NEXTION_TX);// Nextion TX to pin 11 and RX to pin 10 of Arduino
Nextion HMISerial(nextion, 57600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps

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
#define LED_STATUS 3

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

/*
  //Actuator commands HVAC
  #define ACCOFF      0
  #define ACCON       1
  #define ACCTEMP     2
  #define ACCMODE     3
  #define ACCFAN      4
  #define ACCSWING    5

  //Actuator methods HVAC
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
*/

/*
  //Actuator methods TV
  #define TVVOLUMEUP    13
  #define TVVOLUMEDOWN  14
  #define TVCHANNELUP   15
  #define TVCHANNELDOWN 16
  #define TVMUTE        17
  #define TVANTENNA     18
  #define TVHDMI        19
  #define TVBUTRED      20
  #define TVBUTGREEN    21
  #define TVBUTYELLOW   22
  #define TVBUTBLUE     23
  #define TVKEYUP       24
  #define TVKEYDOWN     25
  #define TVKEYLEFT     26
  #define TVKEYRIGHT    27
  #define TVKEYOK       28
  #define TVKEYRETURN   29

  #define SONY      1
  #define SAMSUNG   2
*/

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

#define PIN_ADJU   23
#define PIN_ITEM   24
#define PIN_IHUM   25
#define PIN_RELE   8
#define PIN_TONE   9



/* ACTUATORS
  //0=actuator number
  //1=status
  //2=output type
  //3=value
*/
int actuators[NUM_ACTU][4] = {{PINNODE, 0, 0, 0}};
int aLights[NUM_LIGHTS][5]; //[x][0]=pin [x][1]=type [x][2]=status [x][3]=value [x][4]=color
byte aGroups[1][5] = {0, 0, 0, 0, 0}; //current mood, mood1, mood2...
byte c_light;
// --- Xbee section ----*/
XBee xbee = XBee();
#define TX_RESPONSE ZB_TX_STATUS_RESPONSE
#define request(addr64, payload, sizeofPayload) ZBTxRequest tx = ZBTxRequest(addr64, payload, sizeofPayload)
#define getStatus() txStatus.getDeliveryStatus()
#define TXStatusResponse(txStatus)  xbee.getResponse().getZBTxStatusResponse(txStatus)
ZBTxStatusResponse txStatus = ZBTxStatusResponse();
ZBRxResponse rx = ZBRxResponse();
#define RXStatusResponse(rx) xbee.getResponse().getZBRxResponse(rx);
#define getApiId() xbee.getResponse().getApiId();
#define getRssi() 100;
XBeeAddress64 COORD_ADDR = XBeeAddress64(0x0, 0x0);

// Array to hold data received [0]=command 0=Read 1=Write [1]=pin number [2]=pin value
// es. [R][0][0] read all values, [W][1][100] set pin 1 to value 100
long RxData[NUM_RCVD_VAL];

//convert received payload to long
union {
  long num;
  byte by[3];
} convLong;

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
  pinMode(IR_EMITER, OUTPUT);

  digitalWrite(PIN_RELE, LOW);

  // start serial xbee
  Serial.begin(BAUD_RATE);
  xbee.setSerial(Serial);
  HMISerial.init();
  //HMISerial.sendCommand("bauds=57600");
  delay(1000);
  refreshScreen();
  pBit();
  startTasks();
}  //setup()

void loop(void)
{
  t0.update();
  getScreenTouch();
  if (xbee.readPacket(1))
  {
    getData();
  }
}

// *******************************************************//
// Functions                                              //
// *******************************************************//
void sendSensorData() {
  sendData(ACTUATOR); //send actuators
  getScreenTouch();
}

// ******************************************************* //
// ******************************************************* //
void parseXbeeReceivedData(int x)
{
  int val = OFF;
  if (x == WRITE) {
    if (RxData[3] == DIGITAL) //digital
    {
      val = RxData[2];
    }
    if (RxData[3] == ANALOG || RxData[3] == SERVO || RxData[3] == TONE) //analog
    {
      val = RxData[2];
    }
    if (RxData[3] == HVAC) //AC
    {
      val = RxData[2];
    }
    if (RxData[3] == TV) //TV
    {
      val = RxData[2];
    }
  }
  //--------------------------------------------
  if (x == SMLIGHT_CONFIG) //set light in the array
  {
    setLightConfig(RxData[1], RxData[2], RxData[3]); // pin, type, mode
  }

  if (x == SMLIGHT_GROUP) //set group light moods in the array
  {
    setLightGroup(RxData);
  }

}
// ******************************************************* //

// ******************************************************* //
//-----------------------------------
// Read received data on Xbee serial
//------------------------------------
void getData()
{
  uint8_t option;            // Should return zero, not sure how to use this
  uint8_t dataLength;        // number of bytes of data being sent to xbee
  int resXbee;
  if (xbee.getResponse().isAvailable()) //got something
  {
    resXbee = getApiId();
    if (resXbee == RX_16_RESPONSE || resXbee == RX_64_RESPONSE || resXbee == ZB_RX_RESPONSE || resXbee == ZB_EXPLICIT_RX_RESPONSE)
    {
      RXStatusResponse(rx);
      option = rx.getOption();
      dataLength = rx.getDataLength();
      COORD_ADDR = rx.getRemoteAddress64(); // Serial of Tx (remember MY is set as a hex number.  Useful if you have multiple transmitters

      for (int i = 0; i < dataLength; i = i + NUM_BYTE_ARR)
      {
        // convert received payload to long
        for (int k = 0; k < NUM_BYTE_ARR; k++)
        {
          convLong.by[k] = rx.getData(i + ((NUM_BYTE_ARR - 1) - k));
        }
        RxData[i / NUM_BYTE_ARR] = convLong.num;
      }

      if (RxData[0] == READ) { // if [0]=0 command to read sensors and comando di lettura sensori o attuatori
        sendData(RxData[1]); //definisce cosa leggere
      }
      else
        //if (RxData[0] == WRITE || RxData[0] == ALARM || RxData[0] == TIME || RxData[0] == METEO || RxData[0] == METEO_FCST || RxData[0] == METEO_FCST_TEMP)
      {
        parseXbeeReceivedData(RxData[0]);
      }
    }
  }
}
// ******************************************************* //

// ******************************************************* //
void sendData(int t) // t=0 = sensors 1 = actuators 2=method 3=light
{
  int elements = 0;
  if (t == SMCMD) {
    elements = (NUM_LIGHT_PTS + 1) * 2;
  }
  else if (t == ACTUATOR) {
    elements = (NUM_ACTU_PTS + 1) * 2;
  }

  boolean readPacketResponse; //store the response of xbee.readPacket(timeout)
  uint8_t payload[elements];
  int16_t xbeeData[NUM_DATA_PTS + 1]; // Array to hold integers that will be sent to other xbee [pari]=valore pin [dispari]=valore pin
  int16_t xbeeActu[NUM_ACTU_PTS + 1]; // Array to hold integers that will be sent to other xbee [pari]=valore pin [dispari]=valore pin
  int16_t xbeeLoad[NUM_LIGHT_PTS + 1]; // Array to hold integers that will be sent to other xbee [pari]=valore pin [dispari]=valore pin
  byte idx = 1;
  int response = 0;
  switch (t)
  {
    case SMCMD:
      xbeeLoad[0] = SMCMD;
      for (int i = 1; i < NUM_LIGHTS; i++) {
        xbeeLoad[idx] = aLights[i][0]; //pin
        idx++;
        xbeeLoad[idx] = aLights[i][2]; //status
        idx++;
        xbeeLoad[idx] = aLights[i][3]; //value
        idx++;
        xbeeLoad[idx] = aLights[i][4]; //color
        idx++;
      }

      parseTxData(payload, xbeeLoad, idx);
      break;

    /* ACTUATORS
      //0=actuator number
      //1=status
      //2=output type
      //3=value
    */
    case ACTUATOR:
      // Actuators data
      xbeeActu[0] = ACTUATOR;
      for (byte i = 0; i < NUM_ACTU; i++)
      {
        xbeeActu[idx] = actuators[i][0]; //actuator number
        idx++;
        xbeeActu[idx] = actuators[i][1]; //status
        idx++;
        xbeeActu[idx] = actuators[i][3]; //value
        idx++;
      }
      parseTxData(payload, xbeeActu, idx);
      break;
  }

  request(COORD_ADDR, payload, sizeof(payload));
  /* begin the common part */
  xbee.send(tx);

  xbee.readPacket(50);
  if (xbee.getResponse().isAvailable()) //got something
  {
    // should be a znet tx status
    response = getApiId();
    if (response == TX_RESPONSE) {
      TXStatusResponse(txStatus);
      if (getStatus() == SUCCESS) {
        // g_link = ON;
      }
      else
      {
        // g_link = OFF;
      }
    }
    else if ((response == ZB_RX_RESPONSE))
    {
      //g_link = ON;
      getData();
    }
  }
  else if (xbee.getResponse().isError())
  {
    //g_link = OFF;
  }
  else
  {
    //g_link = OFF;
  } // Finished waiting for XBee packet


} // sendData()
// ******************************************************* //


// ******************************************************* //
void sendCommand(byte cmd)
{ //send single command of changed light
  pBit();
  //read light or group data to transmit
  if (c_light != 0) { //light
    aLights[c_light][2] = HMISerial.getComponentValue("st" + String(c_light)); //get status
    aLights[c_light][3] = HMISerial.getComponentValue("vl" + String(c_light)); //get value
    aLights[c_light][4] = HMISerial.getComponentValue("cx" + String(c_light)); //get color
  } else { //group
    aLights[c_light][2] = HMISerial.getComponentValue("stg" + String(c_light)); //get status
    aLights[c_light][3] = HMISerial.getComponentValue("vlg" + String(c_light)); //get value
    aLights[c_light][4] = HMISerial.getComponentValue("cxg" + String(c_light)); //get mood
  }
  byte array_size = 5;
  int elements = array_size * 2;
  boolean readPacketResponse; //store the response of xbee.readPacket(timeout)
  uint8_t payload[elements];
  int16_t xbeeLoad[array_size]; // Array to hold integers that will be sent
  int response = 0;

  xbeeLoad[0] = SMCMD; //it is a command
  xbeeLoad[1] = aLights[c_light][0]; //pin
  xbeeLoad[2] = aLights[c_light][2]; //status
  xbeeLoad[3] = map(aLights[c_light][3], 0, 100, 0, 254);
  xbeeLoad[4] = aLights[c_light][4]; //color/mood
  parseTxData(payload, xbeeLoad, array_size);
  request(COORD_ADDR, payload, sizeof(payload));
  /* begin the common part */

  //for (int attempts = 0; attempts < 5; attempts++) {
    xbee.send(tx);
    xbee.readPacket(50);
    if (xbee.getResponse().isAvailable()) //got something
    {
      response = getApiId();
      HMISerial.setComponentText("t0", String(response));
      if (response == TX_RESPONSE || response == ZB_TX_STATUS_RESPONSE) {
        TXStatusResponse(txStatus);
        if (getStatus() == SUCCESS) {
          HMISerial.setComponentText("t0", "Ok");
        }
        else
        {
          HMISerial.setComponentText("t0", "Errore");
        }
      }
      else if ((response == ZB_RX_RESPONSE))
      {
        getData();
        xbee.send(tx);
      }
    }
    else
    {
      HMISerial.setComponentText("t0", "No response");
      //break;
    }
    delay(100);
  //}

} // sendCommand()
// ******************************************************* //



// ******************************************************* //
void setPIN(byte pin, byte sts, byte outputType, int p1, int p2)
{
  // p1 = time time, where output type = HVAV p1 = component where type = Thermostat
  // p2 = fading time
  byte actuatorId = getActuatorId(pin);
  switch (outputType) {
    case TONE: //Tone
      if (sts == OFF) {
        noTone(pin);
      }
      if (sts == ON) {
        tone(pin, 4500);
        //TimerFreeTone(pin, false);
      }
      // update actuator status
      if (sts == OFF) { //if = OFF
        actuators[actuatorId][1] = OFF; //set actuator status inactive
      }
      else { //if != OFF
        actuators[actuatorId][1] = ON; //set actuator status active
      }
      break;

  }
}

// ******************************************************* //
// Common functions                                        //
// ******************************************************* //

// ******************************************************* //
int getActuatorId(uint8_t pin)
{
  for (uint8_t i = 0; i < sizeof(actuators); i++)
  { //cerco il tipo di nodo
    if (actuators[i][0] == pin)
    {
      return i;
      break;
    }
  }
}

// ******************************************************* //
void parseTxData(uint8_t* payload, int16_t* txData, int idx)
{
  // SRG put the xbee code in it's own function   bool xbSend =  sendToXBee(&xbeeData);
  // break down integers into two bytes and place in payload
  for (int t = 0; t < idx; t++)
  {
    payload[t * 2]     = txData[t] >> 8 & 0xff; // High byte - shift bits 8 places, 0xff masks off the upper 8 bits
    payload[(t * 2) + 1] = txData[t] & 0xff;  // low byte, just mask off the upper 8 bits
  }
}


void startTasks() {
  t0.every(TIMEt1, sendSensorData);
  //t0.every(TIMEt0, updatePinValues);
}

int getPinIdx(byte pin)
{
  int ret = -1;
  for (byte i = 1; i < NUM_LIGHTS; i++)
  {
    if (aLights[i][0] == pin)
    {
      ret = i;
      break;
    }
  }
  return ret;
}

int getFreePinIdx()
{
  int ret = -1;
  for (byte i = 1; i < NUM_LIGHTS; i++)
  {
    if (aLights[i][0] == NOLIGHT)
    {
      ret = i;
      break;
    }
  }
  return ret;
}


