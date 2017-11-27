/*
  CONFIGURATION for ENDNODE PCB
  Thermostat
  18/09/2017
  Version 18.1 XX
  Author: Franco Parenti
  Url:
  Support for Arduino UNO, Mini Pro
  XBee Series 2 Receiver
  Support Nextion Display
  Support for TV ir
  Support Methods


  Setup compiler
  \library\IRRemote\IRRemoteInt.h set -> #define #define IR_USE_TIMER3
  \hardware\arduino\avr\cores\Tone.cpp set -> #define USE_TIMER1
  \hardware\arduino\libraries\Servo\src\avr\ServoTimers.h set -> #define _useTimer3 and typedef enum { _timer3,....
*/

// Include libraries //
#include <XBee.h>
#include <Timer.h>
//#include <dht.h>
#include <SoftwareSerial.h>
#include <Nextion.h>
//#include <IRremote.h>
#include <EEPROM.h>
#include <TimeLib.h>
//#include <pitches.h>
#include <OneWire.h>
#include <DallasTemperature.h>
//#include <Wire.h>
//#include "RTClib.h"

//#include <avr/io.h>
//#include <avr/wdt.h>
//#define Reset_AVR() wdt_enable(WDTO_30MS); while(1) {}

// -- comment this part if you don't use remote control
// -- (optional) delete the file functions.ino from the sketch folder
// ---------- AC Remote control section ------------------------//
// Support for Midea, Hokkaido HVAC, Type:R51M/E remote control //
//#include <MideaIR.h>
//IRsend irsend;
//MideaIR remote_control(&irsend);
// ------------------------------------------------------------ //
int cnt = 0;
//-----------------------------------------------------------
#define PINNODE 0
//-----------------------------------------------------------
// Nextion Display
//-----------------------------------------------------------
//-----------------------------------------------------------
#define ON  1
#define OFF 0

#define AUTO 0
#define AUTOPROG 1
#define MANUAL 2
#define HUB 3

#define DAILY 0
#define WDAYS 1
#define WEND  2

#define TMONOFF    0
#define TMMODE     31
#define TMPROG     32
#define TMFIRE     33
#define TMADJUST   34

#define TMAUTO     0
#define TMAUTOPROG 1
#define TMMANUAL   2
#define TMHUB      3
#define TMDAILY    0
#define TMWDAYS    1
#define TMWEND     2

#define UNKNOWN     56
#define SUNNY       57
#define SCATT_CLOUD 58
#define PARTLY_SUNNY 59
#define CLOUDY      60
#define PARTLY_CLOUDY 61
#define OVERTCAST   62
#define RAIN   63
#define STORM 64
#define FLURRIES 65
#define SNOW 66
#define HAZE 67
#define FOG 68
#define CHANCE_SNOW 69
#define CHANCE_FLURRY 70
#define CHANCE_RAIN 71
#define CHANCE_STORM 72

uint8_t g_sts = OFF;
uint8_t g_mode = AUTO;
uint8_t g_prog = DAILY;
uint8_t g_fire = OFF;
uint8_t g_cpage = 0;
boolean g_bit = 0;
uint8_t g_meteo = UNKNOWN;
uint8_t g_link = OFF;
uint8_t c_timeline; //current timeline value (temp range)

float t_intTemp = 0;
float t_extTemp = 0;
float t_adjust = 19.0;
float h_intHum = 0;
float h_extHum = 0;
float p_extPre = 0;

#define NEX_RET_CMD_FINISHED            (0x01)
#define NEX_RET_EVENT_LAUNCHED          (0x88)
#define NEX_RET_EVENT_UPGRADED          (0x89)
#define NEX_RET_EVENT_TOUCH_HEAD            (0x65)
#define NEX_RET_EVENT_POSITION_HEAD         (0x67)
#define NEX_RET_EVENT_SLEEP_POSITION_HEAD   (0x68)
#define NEX_RET_CURRENT_PAGE_ID_HEAD        (0x66)
#define NEX_RET_STRING_HEAD                 (0x70)
#define NEX_RET_NUMBER_HEAD                 (0x71)
#define NEX_RET_INVALID_CMD             (0x00)
#define NEX_RET_INVALID_COMPONENT_ID    (0x02)
#define NEX_RET_INVALID_PAGE_ID         (0x03)
#define NEX_RET_INVALID_PICTURE_ID      (0x04)
#define NEX_RET_INVALID_FONT_ID         (0x05)
#define NEX_RET_INVALID_BAUD            (0x11)
#define NEX_RET_INVALID_VARIABLE        (0x1A)
#define NEX_RET_INVALID_OPERATION       (0x1B)

#define NEX_RET_ONFF  0x20
#define NEX_RET_MODE  0x21
#define NEX_RET_PROG  0x22
#define NEX_RET_INCR  0x23
#define NEX_RET_DECR  0x24
#define NEX_RET_SSAV  0x40
#define NEX_RET_SPRO  0x41
#define NEX_RET_RSAV  0x50
#define NEX_RET_REST  0x48

#define PAGE_MAIN 0
#define PAGE_SETTINGS 1
#define PAGE_RANGES 2
#define PAGE_PROGRAMS 3
#define PAGE_DEBUG 4

SoftwareSerial nextion(4, 5);// Nextion TX to pin 11 and RX to pin 10 of Arduino
Nextion HMISerial(nextion, 57600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps

int timeline[145] = {9,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,4,4,4,4,4,4,4,3,2,1,1};
// 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24
//{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,1,1,1,1};
//{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,1,1,1,1}
//{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,4,4,4,4,4,4,4,3,2,2,1}
int range[4][2];
int weekdays[7][3];

char buffer[50] = {0};
//-----------------------------------------------------------
//-----------------------------------------------------------
// reserved pins
#define DHT_PIN 5 //DHT
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature temperature(&oneWire);
// arrays to hold device address

//TC_DS1307 rtc;

const int pinTempSensor = A0;
//int arr[] = {0,0,0,0,0,0,0,0,0,0};
//int arr[] = {0};
//int seq = 0;
//int maxseq = sizeof(arr)/2;
//int maxseq = 1;

#define IR_EMITER 3 //pin for IR Led 
#define PINt 24 //virtual pin for temperature
#define PINh 25 //virtual pin for humidity

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
#define READ        0
#define WRITE       1
#define ALARM       2
#define TIME        3
#define METEO       4

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
#define TIMEt2 30000 //set time

#define BAUD_RATE     115200  // Baud for both Xbee and serial monitor
#define NUM_ACTU      1 //Insert here the (# of actuators x NUM_DATA_VAL)
#define NUM_METH      5 //Insert here the (# of methods x NUM_DATA_VAL)
#define NUM_DATA      3 //Insert here the (# of sensors x NUM_DATA_VAL)
#define NUM_DATA_VAL  3 // number of values transmitted for each sensor: number, value, alarm status
#define NUM_DATA_PTS  NUM_DATA*NUM_DATA_VAL
#define NUM_ACTU_PTS  NUM_ACTU*NUM_DATA_VAL
#define NUM_METH_PTS  NUM_METH*NUM_DATA_VAL
#define NUM_BYTE_ARR  3 // Number of bytes of the array used to store long integers in the payload

#define SENSOR   0
#define ACTUATOR 1
#define METHOD   2

#define RANGE_IN  1 //the actuator is in range 
#define RANGE_OUT 0 //the actuator is out of range 

#define NUM_RCVD_VAL  12 // Numero di valori ricevuri dal cordinatore 

#define PIN_ADJU   23
#define PIN_ITEM   24
#define PIN_IHUM   25
#define PIN_RELE   8
#define PIN_TONE   9
/* SENSORS
   arrays don't support more than 10 cols so I create the index of sensors. Other values are in the trigger array
  //0=pin
  //1=value

// int sensors[NUM_DATA][2] = {{PIN_ADJU, 0}, {PIN_ITEM, 0}, {PIN_IHUM, 0}};

 TRIGGERS
  pin 6 Digital/Analog, 7 Digital/Analog, 8 Tone, 9 Servo, 10 Digital/Analog, 11 Digital/Analog, 14 Digital/Analog, 15 Digital/Analog, 16 Digital/Analog, 17 Digital/Analog
  // 0=immediate alarm 1=permanent 2=temporary
  // 1=actuator
  // 2=level min
  // 3=status
  // 4=level max
  // 5=output type 0=analog 1=digital
  // 6=action in range
  // 7=action out of range range
  // 8=timer time
  // 9=dimming time 0=no dimmer
*/

#define ACT_STAT   0
#define ACT_MODE   31
#define ACT_PROG   32
#define ACT_FIRE   33
#define ACT_ADJU   34

/* ACTUATORS
  //0=actuator number
  //1=status
  //2=output type
  //3=value
*/

int actuators[NUM_ACTU][4] = {{PINNODE, 0, 0, 0}};

//dht DHT; // Object for DHT22 sensor

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

void sendSensorData();
void updatePinValues();
void f_time();
void beep();

Timer t0; //timer to schedule the sensors and actuators values update

//============================================================================
//  Initialize
//============================================================================
void setup()
{
  // initialize arrays and pins
  for (int i = 0; i < NUM_RCVD_VAL; i++)
  {
    RxData[i] = 0;
  }

  //pinMode(IR_EMITER, OUTPUT);

  // reserved to temp sensor
  pinMode(DHT_PIN, INPUT);

  //read Temperature ann Humidity for the first time
  //delay(5000); // I wait for 5 sec before reading data
  //updatePinValuesDHTinit(); //read DHT22 data

  pinMode(PIN_RELE, OUTPUT);

  // start serial xbee
  Serial.begin(BAUD_RATE);
  xbee.setSerial(Serial);
  delay(3000);
  play();

  // locate devices on the bus
  temperature.begin();
  DeviceAddress ThermometerAddress;
  temperature.getAddress(ThermometerAddress, 0);
  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  temperature.setResolution(ThermometerAddress, 12);
  setTime(0, 0, 0, 30, 9, 1961); 
  //rtc.begin(); 
  //delay(10);  
  g_link = ON;
  HMISerial.init();
  initScreen();
  initTimeLine();
  f_timeline();
  f_time();  
  f_refreshMain();
  updatePinValues(); // read sensor once
  //updatePinValuesDHT(); // read sensor once
  startTasks();

}  //setup()

//============================================================================
// continuously reads packets, looking for RX16 or RX64
//============================================================================
void loop()
{
  t0.update();
  //slave.execute();
  getScreenTouch();
  if (xbee.readPacket(1))
  {
    getData();
  }
  else
  {
    //  checkAlarm();
    //  checkDimmers();
  }
} // loop()

void sendSensorData() {
  sendData(SENSOR); //send sensors
  getScreenTouch();
  sendData(ACTUATOR); //send actuators
  getScreenTouch();
  sendData(METHOD); //send methods
  getScreenTouch();
}

// *******************************************************//
// Functions                                              //
// *******************************************************//
/*
void updatePinValuesDHT()
{
  int chk = DHT.read22(DHT_PIN);
  if (chk == DHTLIB_OK & DHT.temperature < 60)
     {     
       t_intTemp = (t_intTemp + DHT.temperature)/2;
       h_intHum = DHT.humidity;
     }
  //Serial.println(chk);
  //Serial.println(t_intTemp,DEC);   
  getScreenTouch();
  checkFire();
  f_refreshMain(); 
} // updatePinValuesDHT()
*/

void updatePinValues()
{
//printNow();
  temperature.requestTemperatures(); // Send the command to get temperatures
  float tempC = temperature.getTempCByIndex(0);
   
  t_intTemp = (t_intTemp + (tempC-2.1))/2; //tolo 2.1 per ricalibrare la temperatura troppo alta
  h_intHum = 0;

  getScreenTouch();
  checkFire();
  f_refreshMain(); 

} // updatePinValues()

// ******************************************************* //

/*
// ******************************************************* //
int getActuatorStatus(int actuator, int inRange, int outRange)
{
  for (int i = 0; i < NUM_ACTU; i++)
  {
    if (actuators[i][0] == actuator)
    {
      if (actuators[i][1] == inRange)
        return RANGE_IN;
      if (actuators[i][1] == outRange)
        return RANGE_OUT;
    }
  }
}
*/
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
    if (RxData[3] == THERMOSTAT)
    {
      // 2=value, 3=termostat 4=num method
      setPIN(RxData[1], RxData[2], RxData[3], RxData[4], RxData[5]);
      sendData(ACTUATOR);
      sendData(METHOD);
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
    if (RxData[3] != THERMOSTAT) {
      setPIN(RxData[1], val, RxData[3], RxData[4], RxData[5]);
      sendData(ACTUATOR); //send actuators value
    }
  }
  //--------------------------------------------
  if (x == TIME) //Set time
  {
    /*
      RxData[1]; //day
      RxData[2]; //month
      RxData[3]; //year
      RxData[4]; //hours
      RxData[5]; //minutes
    */
    //rtc.adjust(DateTime(RxData[3], RxData[2], RxData[1], RxData[4], RxData[5], 0));
    setTime(RxData[4], RxData[5], 0, RxData[1], RxData[2], RxData[3]);
  }
  if (x == METEO) //Set meteo
  {
    t_extTemp =  RxData[1] / 100; //EXT TEMP
    h_extHum =  RxData[2]; //EXT HUMIDITY
    p_extPre =  RxData[3]; //EXT PRESSURE
    g_meteo =  RxData[4]; //METEO
  }
  if (x == ALARM)
  { // set the sensors and the triggers
    if (RxData[4] == RESETALARM)
    { //trigger reset
      //      triggers[getSensorId(RxData[1])][3] = OFF;
      //      setPIN(RxData[3], RxData[9], RxData[7], 0, 0);
    }
    else if (RxData[2] == 0)
    {
      //disable trigger
      //  triggers[getSensorId(RxData[1])][0] = OFF;
      //set actuator to "Out of range" value
      // setPIN(triggers[getSensorId(RxData[1])][1], triggers[getSensorId(RxData[1])][9], triggers[getSensorId(RxData[1])][7], 0, 0);
    }
    else
    {
      //1,20,1,4,500,0,1024,0,1,0,0 set remote alarm digital
      //1,20,1,4,500,0,1024,1,255,0,0 set remote alarm analog
      /*
        triggers[getSensorId(RxData[1])][0] = RxData[2]; //alarm type
        triggers[getSensorId(RxData[1])][1] = RxData[3]; //actuator
        triggers[getSensorId(RxData[1])][2] = RxData[4]; //min val
        triggers[getSensorId(RxData[1])][3] = RxData[5]; //status
        triggers[getSensorId(RxData[1])][4] = RxData[6]; //max val
        triggers[getSensorId(RxData[1])][5] = RxData[7]; //output type
        triggers[getSensorId(RxData[1])][6] = RxData[8]; //action in range
        triggers[getSensorId(RxData[1])][7] = RxData[9]; //action out of range
        triggers[getSensorId(RxData[1])][8] = RxData[10]; //timer time
        triggers[getSensorId(RxData[1])][9] = RxData[11]; //dimming time
      */
    }
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
      if (RxData[0] == WRITE || RxData[0] == ALARM || RxData[0] == TIME || RxData[0] == METEO) { //[0]=1 command to set sensor, [0]=2 set trigger
        parseXbeeReceivedData(RxData[0]);
      }
    }
  }
  else if (xbee.getResponse().isError())
  { // Got something, but not a packet
    //null
  }
  else
  {
    // xbee not available and no error
    //null
  }
}
// ******************************************************* //

// ******************************************************* //
void sendData(int t) // t=0 = sensors 1 = actuators
{
  int elements = 0;
  if (t == SENSOR)
  {
    elements = (NUM_DATA_PTS + 1) * 2;
  }
  else if (t == METHOD) {
    elements = (NUM_METH_PTS + 1) * 2;
  }
  else if (t == ACTUATOR) {
    elements = (NUM_ACTU_PTS + 1) * 2;
  }


  unsigned long receiveResponseInitTime;
  boolean readPacketResponse; //store the response of xbee.readPacket(timeout)
  uint8_t payload[elements];
  int16_t xbeeData[NUM_DATA_PTS + 1]; // Array to hold integers that will be sent to other xbee [pari]=valore pin [dispari]=valore pin
  int16_t xbeeActu[NUM_ACTU_PTS + 1]; // Array to hold integers that will be sent to other xbee [pari]=valore pin [dispari]=valore pin
  int16_t xbeeMeth[NUM_METH_PTS + 1]; // Array to hold integers that will be sent to other xbee [pari]=valore pin [dispari]=valore pin
  int idx = 1;
  int x = 0;
  switch (t)
  {
    case SENSOR:
      // Sensors data
      xbeeData[idx] = 24;
      idx++;
      xbeeData[idx] = (int) (t_intTemp * 100);
      idx++;
      xbeeData[idx] = 0;
      idx++;
      xbeeData[idx] = 25;
      idx++;
      xbeeData[idx] = int(h_intHum);
      idx++;
      xbeeData[idx] = 0;
      idx++;

      xbeeData[0] = SENSOR;
      parseTxData(payload, xbeeData, idx);
      break;

    case METHOD:
      xbeeMeth[0] = METHOD;

      xbeeMeth[idx] = PINNODE;
      idx++;
      xbeeMeth[idx] = ACT_STAT;
      idx++;
      xbeeMeth[idx] = g_sts;
      idx++;

      xbeeMeth[idx] = PINNODE;
      idx++;
      xbeeMeth[idx] = ACT_MODE;
      idx++;
      xbeeMeth[idx] = g_mode;

      idx++;
      xbeeMeth[idx] = PINNODE;
      idx++;
      xbeeMeth[idx] = ACT_PROG;
      idx++;
      xbeeMeth[idx] = g_prog;

      idx++;
      xbeeMeth[idx] = PINNODE;
      idx++;
      xbeeMeth[idx] = ACT_FIRE;
      idx++;
      xbeeMeth[idx] = g_fire;

      idx++;
      xbeeMeth[idx] = PINNODE;
      idx++;
      xbeeMeth[idx] = ACT_ADJU;
      idx++;
      xbeeMeth[idx] = int(t_adjust);
      idx++;

      parseTxData(payload, xbeeMeth, idx);
      break;

    case ACTUATOR:
      // Actuators data
      for (int i = 0; i < NUM_ACTU; i++)
      {
        xbeeActu[idx] = actuators[i][0]; //actuator number
        idx++;
        xbeeActu[idx] = actuators[i][1]; //status
        idx++;
        xbeeActu[idx] = actuators[i][1]; //value
        idx++;
      }

      xbeeActu[0] = ACTUATOR;
      parseTxData(payload, xbeeActu, idx);
      break;
  }

  request(COORD_ADDR, payload, sizeof(payload));
  /* begin the common part */

  if (g_link == OFF) {
    g_link = ON;
    //HMISerial.setComponentValue("gLink", g_link);
    HMISerial.sendCommand("vis pLink,1");
  }
  xbee.send(tx);
  getScreenTouch();
  xbee.readPacket(50);
  if (xbee.getResponse().isAvailable()) //got something
  {
    // should be a znet tx status
    x = getApiId();
    if (x == TX_RESPONSE) {
      TXStatusResponse(txStatus);
      // got a response!
      // should be a znet tx status
      if (getStatus() == SUCCESS) {
        // success.  time to celebrate
        //null
      }
      else
      {
        HMISerial.sendCommand("vis pLink,0");
        // the remote XBee did not receive our packet. is it powered on?
      }
    }
    else if ((x == ZB_RX_RESPONSE))
    {
      //null
      getData();
    }
  }
  else if (xbee.getResponse().isError())
  {
    g_link = OFF;
    //HMISerial.setComponentValue("gLink", g_link);
    HMISerial.sendCommand("vis pLink,0");
  }
  else
  {
    g_link = OFF;
    //HMISerial.setComponentValue("gLink", g_link);
    HMISerial.sendCommand("vis pLink,0");
  } // Finished waiting for XBee packet
} // sendData()
// ******************************************************* //


// ******************************************************* //
void setPIN(int pin, int sts, int outputType, int p1, int p2)
{
  // p1 = time time, where output type = HVAV p1 = component
  // p2 = fading time
  int i = 0;
  int actuatorId = getActuatorId(pin);
  switch (outputType) {
    // -- Thermostat -----------------------//
    case THERMOSTAT:  // Thermostat type
      switch (p1) {
        case TMONOFF:
          g_sts = int(sts);
          actuators[getActuatorId(PINNODE)][1] = g_sts;
          actuators[getActuatorId(PINNODE)][3] = g_sts;
          HMISerial.setComponentValue("gStatus", g_sts);
          f_sts;
          checkFire();
          break;
        case TMMODE:
          g_mode = int(sts);
          HMISerial.setComponentValue("gMode", g_mode);
          f_mode();
          checkFire();
          break;
        case TMFIRE:
          g_fire = int(sts);
          f_fire;
          break;
        case TMPROG:
          g_prog = int(sts);
          HMISerial.setComponentValue("gProg", g_prog);
          f_prog();
          checkFire();
          break;
        case TMADJUST:
          t_adjust = int(sts);
          HMISerial.setComponentValue("gAdjInt", getInt(t_adjust));
          HMISerial.setComponentValue("gAdjDec", getDec(t_adjust));
          checkFire();
          break;
      }
      break;

/*
    case DIGITAL: //digital
      if (p1 == 0) {
        if (pin != 0)
          digitalWrite(pin, sts);
        else
          g_sts = sts;
      }
      else {
        int dummy = 1;
        //t0.pulseImmediate(pin, (unsigned long)(p1 * 1000), sts);
      }
      // update actuator status
      actuators[actuatorId][1] = sts;
      break;
    case ANALOG: //analog
      //dimmTime = 5;
      if (p2 == 0) {
        analogWrite(pin, sts);
      }
      else
      {
        // set the dimmer (pin, time, target value)
        //setDimmer(actuatorId, p2, sts);
      }
      // update actuator status
      if (sts == OFF) { //if = OFF
        actuators[actuatorId][1] = OFF; //set actuator status inactive
      }
      else { //if != OFF
        actuators[actuatorId][1] = ON; //set actuator status active
      }
      break;
 */
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
/*
      // -- AC Remote Control section --//
#if defined (REMOTECONTROL_MODE)
    case HVAC:  // Air Condition
      // p1 = 0,1 Off,On 2=temperature 3=mode, 4=fan, 5=swing
      // sts 3=fan1 4=fan2 5=fan3 6=fanauto 7=cool 8=dry 9=heat 10=auto 11=temperature 12=swing
      switch (p1)  {
        case ACCOFF:
          // update actuator status
          if (sts == OFF) { //if = OFF
            r_off(); //turn off AC
          }
          else { //if != OFF
            r_on(); //turn on AC
          }
          break;
        case ACCON:
          // update actuator status
          if (sts == OFF) { //if = OFF
            r_off(); //turn off AC
          }
          else { //if != OFF
            r_on(); //turn on AC
          }
          break;
        case ACCTEMP:
          r_temp(sts);
          break;
        case ACCMODE:  //mode
          switch (sts) {
            case ACMODECOOL: //Cool
              r_mode(mode_cool);
              break;
            case ACMODEDRY: //dry
              r_mode(mode_no_humidity);
              break;
            case ACMODEHEAT: //Heat
              r_mode(mode_heat);
              break;
            case ACMODEAUTO: //Auto
              r_mode(mode_auto);
              break;
          }
          break;
        case ACCFAN:  //fan
          switch (sts) {
            case ACFAN1: //fan 1
              r_fan(fan_speed_1);
              break;
            case ACFAN2: //fan 2
              r_fan(fan_speed_2);
              break;
            case ACFAN3: //fan 3
              r_fan(fan_speed_3);
              break;
            case ACFANAUTO: //fan Auto
              r_fan(fan_auto);
              break;
          }
          break;
        case ACCSWING:
          r_swing();
          break;
      }
      break;

    // -- TV Remote Control section --//
    case TV:  // TV
      // p1 = 0,1 Off,On 2=temperature 3=mode, 4=fan, 5=swing
      // sts 3=fan1 4=fan2 5=fan3 6=fanauto 7=cool 8=dry 9=heat 10=auto 11=temperature 12=swing
      switch (sts) { //method
        case ON:
          tv_on(p1);
          break;
        case OFF:
          tv_off(p1);
          break;
        case TVVOLUMEUP:
          tv_volumeup(p1);
          break;
        case TVVOLUMEDOWN:
          tv_volumedown(p1);
          break;
        case TVCHANNELUP:
          tv_channelup(p1);
          break;
        case TVCHANNELDOWN:
          tv_channeldown(p1);
          break;
        case TVMUTE:
          tv_mute(p1);
          break;
        case TVANTENNA:
          tv_source(p1);
          break;
        case TVHDMI:
          tv_home(p1);
          break;
        case TVBUTRED:
          tv_red(p1);
          break;
        case TVBUTGREEN:
          tv_green(p1);
          break;
        case TVBUTYELLOW:
          tv_yellow(p1);
          break;
        case TVBUTBLUE:
          tv_blue(p1);
          break;
        case TVKEYUP:
          tv_keyup(p1);
          break;
        case TVKEYDOWN:
          tv_keydown(p1);
          break;
        case TVKEYLEFT:
          tv_keyleft(p1);
          break;
        case TVKEYRIGHT:
          tv_keyright(p1);
          break;
        case TVKEYOK:
          tv_keyok(p1);
          break;
        case TVKEYRETURN:
          tv_keyreturn(p1);
          break;
      }
      break;

    case LEDRGB:  // TV

      convertColors(RxData[4]);

      for (uint16_t i = 0; i < LEDCOUNT; i++)
      {
        colors[i] = color;
      }
      ledStrip.write(colors, LEDCOUNT);

      break;

#endif
*/
  }
  //actuators[actuatorId][3] = sts; //set the actuator value

}


#define TVKEYUP       24
#define TVKEYDOWN     25
#define TVKEYLEFT     26
#define TVKEYRIGHT    27
#define TVKEYOK       28
#define TVKEYRETURN   29

// ******************************************************* //
// Common functions                                        //
// ******************************************************* //
/*
  int getSensorId(int pin)
  {
  for (int i = 0; i < sizeof(sensors); i++)
  { //cerco il tipo di nodo
    if (sensors[i][0] == pin)
    {
      return i;
      break;
    }
  }
  }
*/
// ******************************************************* //
int getActuatorId(int pin)
{
  for (int i = 0; i < sizeof(actuators); i++)
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


void play()
{
  tone(9, 1500, 500);
  /*
// notes in the melody:
  int melody[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
  };

  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int noteDurations[] = {
    4, 8, 8, 4, 4, 4, 4, 4
  };

  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(9, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(9);
    
  }
*/
}

/*
  void reset() {
  Reset_AVR();
  }
*/

void startTasks() {
  t0.every(TIMEt1, sendSensorData);
  t0.every(TIMEt0, updatePinValues);
  t0.every(TIMEt2, f_time);
}

void beep() {
   tone(9, 1500, 30);
   noTone;
}

/*
void printNow()
{ 

   DateTime now = rtc.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(f_weekday(now.dayOfTheWeek()),DEC);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
}
*/

