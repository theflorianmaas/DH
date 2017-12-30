// ------------------------------------------------------------ //
// EndNode_20.3_Thermo version with DS1307 RTC + AM2322 sensor
// V. 20.3 19/12/2017
// Meteo forecast support
// DS18B20 support
// ------------------------------------------------------------ //
// Include libraries //
#include <XBee.h>
#include <Timer.h>
#include <SoftwareSerial.h>
#include <Nextion.h>
#include <EEPROM.h>
#include <pitches.h>
#include <Wire.h>
#include <RTClib.h>
#include <AM2322.h>
#include <OneWire.h>

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

uint8_t g_sts = ON;
uint8_t g_mode = AUTO;
uint8_t g_prog = DAILY;
uint8_t g_fire = OFF;
uint8_t g_cpage = 0;
boolean g_bit = false;
uint8_t g_meteo = UNKNOWN;
uint8_t g_link = OFF;
uint8_t c_timeline; //current timeline value (temp range)
byte g_meteof = 0;

float t_intTemp = 0;
float t_extTemp = 0;
float t_adjust = 19.0;
float h_intHum = 0;
float h_extHum = 0;
float p_extPre = 0;

//--------------------------------------------//
// field labels
//--------------------------------------------//
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

//Codes received from Nextion screen on serial
#define NEX_RET_ONFF  0x20 //on/off
#define NEX_RET_MODE  0x21 //mode
#define NEX_RET_PROG  0x22 //program
#define NEX_RET_INCR  0x23 //manual temp increase
#define NEX_RET_DECR  0x24 //manual temp decrease
#define NEX_RET_SSAV  0x40 //Save
#define NEX_RET_SPRO  0x41 //Save timeline
#define NEX_RET_RSAV  0x50 //Save range
#define NEX_RET_REST  0x48 //reset
#define NEX_RET_TSAV  0x60 //Save time

#define PAGE_MAIN 0
#define PAGE_SETTINGS 1
#define PAGE_RANGES 2
#define PAGE_PROGRAMS 3
#define PAGE_DEBUG 4
#define PAGE_TIME 6
#define PAGE_METEO 7

SoftwareSerial nextion(NEXTION_RX, NEXTION_TX);// Nextion TX to pin 11 and RX to pin 10 of Arduino
Nextion HMISerial(nextion, 57600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps

#define TIMELINE_SEGMENTS 48 //timeline ticks. One every 30mins = 48
#define TIMELINE_ARRAY   145 //48* Daily+Working days+Week end +1

byte timeline[TIMELINE_ARRAY] = {9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 1};
byte range[4][2];
byte weekdays[7][3];

//-----------------------------------------------------------
//-----------------------------------------------------------

RTC_DS1307 rtc; //Real time clock instance
AM2322 am2322;  //Temp+Hum sensor instance
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire ds(ONE_WIRE_BUS);

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
#define TIMEt3 2000 // check fire

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

  //pinMode(IR_EMITER, OUTPUT);

  pinMode(PIN_RELE, OUTPUT);
  pinMode(IR_EMITER, OUTPUT);

  digitalWrite(PIN_RELE, LOW);
  analogWrite(IR_EMITER, 100);

  // start serial xbee
  Serial.begin(BAUD_RATE);
  xbee.setSerial(Serial);
  rtc.begin();
  am2322.begin();
  HMISerial.init();
  delay(1000);
  play();
  initScreen();
  initTimeLine();
  updatePinValues(); // read sensor once
  f_refreshMain();
  f_time();
  startTasks();
  analogWrite(IR_EMITER, 0);
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
  sendData(SENSOR); //send sensors
  getScreenTouch();
  sendData(ACTUATOR); //send actuators
  getScreenTouch();
  sendData(METHOD); //send methods
  getScreenTouch();
}

void updatePinValues()
{

  am2322.readTemperatureAndHumidity(t_intTemp, h_intHum);
  t_intTemp = t_intTemp - 3; //compensate components heating
  getScreenTouch();
  checkFire();
  f_refreshMain();

} // updatePinValues()

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
    if (RxData[3] == THERMOSTAT)
    {
      // 2=value, 3=termostat 4=num method
      setPIN(RxData[1], RxData[2], RxData[3], RxData[4], RxData[5]);
      sendData(ACTUATOR);
      sendData(METHOD);
    }
    else
    {
      setPIN(RxData[1], val, RxData[3], RxData[4], RxData[5]);
      sendData(ACTUATOR); //send actuators value
    }

  }
  //--------------------------------------------
  if (x == TIME) //Set time
  {
    rtc.adjust(DateTime(RxData[3], RxData[2], RxData[1], RxData[4], RxData[5], 0));
  }
  if (x == METEO) //Set meteo
  {
    if (RxData[1] >= 9000) 
      t_extTemp =  (RxData[1]-9000)*-1; //EXT TEMP negative value
    else
      t_extTemp =  RxData[1] / 100; //EXT TEMP positive value
    h_extHum =  RxData[2]; //EXT HUMIDITY
    p_extPre =  RxData[3]; //EXT PRESSURE
    g_meteo =  RxData[4]; //METEO
  }
  if (x == METEO_FCST) //Set meteo forecast
  {
    f_meteo_forecast();
  }
  if (x == METEO_FCST_TEMP) //Set meteo forecast temperature
  {
    f_meteo_forecast_temp();
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

  boolean readPacketResponse; //store the response of xbee.readPacket(timeout)
  uint8_t payload[elements];
  int16_t xbeeData[NUM_DATA_PTS + 1]; // Array to hold integers that will be sent to other xbee [pari]=valore pin [dispari]=valore pin
  int16_t xbeeActu[NUM_ACTU_PTS + 1]; // Array to hold integers that will be sent to other xbee [pari]=valore pin [dispari]=valore pin
  int16_t xbeeMeth[NUM_METH_PTS + 1]; // Array to hold integers that will be sent to other xbee [pari]=valore pin [dispari]=valore pin
  byte idx = 1;
  int response = 0;
  switch (t)
  {
    case SENSOR:
      // Sensors data
      xbeeData[0] = SENSOR;
      xbeeData[idx] = 24;
      idx++;
      xbeeData[idx] = (int)(t_intTemp * 100);
      idx++;
      xbeeData[idx] = 0;
      idx++;
      xbeeData[idx] = 25;
      idx++;
      xbeeData[idx] = (byte)(h_intHum);
      idx++;
      xbeeData[idx] = 0;
      idx++;

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
  getScreenTouch();
  xbee.readPacket(50);
  if (xbee.getResponse().isAvailable()) //got something
  {
    // should be a znet tx status
    response = getApiId();
    if (response == TX_RESPONSE) {
      TXStatusResponse(txStatus);
      // got a response!
      // should be a znet tx status
      if (getStatus() == SUCCESS) {
        g_link = ON;
      }
      else
      {
        g_link = OFF;
      }
    }
    else if ((response == ZB_RX_RESPONSE))
    {
      g_link = ON;
      getData();
    }
  }
  else if (xbee.getResponse().isError())
  {
    g_link = OFF;
  }
  else
  {
    g_link = OFF;
  } // Finished waiting for XBee packet
  if (g_link == ON)
    HMISerial.sendCommand("vis pLink,1");
  else
    HMISerial.sendCommand("vis pLink,0");

} // sendData()
// ******************************************************* //


// ******************************************************* //
void setPIN(byte pin, byte sts, byte outputType, int p1, int p2)
{
  // p1 = time time, where output type = HVAV p1 = component where type = Thermostat
  // p2 = fading time
  byte actuatorId = getActuatorId(pin);
  switch (outputType) {
    // -- Thermostat -----------------------//
    case THERMOSTAT:  // Thermostat type
      switch (p1) {
        case TMONOFF:
          g_sts = sts;
          actuators[getActuatorId(PINNODE)][1] = g_sts;
          HMISerial.setComponentValue("gStatus", g_sts);
          f_sts;
          checkFire();
          break;
        case TMMODE:
          g_mode = sts;
          HMISerial.setComponentValue("gMode", g_mode);
          f_mode();
          checkFire();
          break;
        case TMFIRE:
          g_fire = sts;
          f_fire;
          break;
        case TMPROG:
          g_prog = sts;
          HMISerial.setComponentValue("gProg", g_prog);
          f_prog();
          checkFire();
          break;
        case TMADJUST:
          t_adjust = sts;
          HMISerial.setComponentValue("gAdjInt", getInt(t_adjust));
          HMISerial.setComponentValue("gAdjDec", getDec(t_adjust));
          checkFire();
          break;
      }
      break;

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

void play()
{

  // notes in the melody:
  int melody[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
  };

  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  uint8_t noteDurations[] = {
    4, 8, 8, 4, 4, 4, 4, 4
  };

  for (uint8_t thisNote = 0; thisNote < 8; thisNote++) {

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
}

void startTasks() {
  t0.every(TIMEt1, sendSensorData);
  t0.every(TIMEt0, updatePinValues);
  t0.every(TIMEt2, f_time);
  t0.every(TIMEt3, checkFire);
}

float getTemp() {
  //returns the temperature from one DS18B20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
    //no more sensors on chain, reset search
    ds.reset_search();
    return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad

  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  int TemperatureSumInt = int(TemperatureSum * 100);
  TemperatureSum = float(TemperatureSumInt) / 100;
  return TemperatureSum;

}

