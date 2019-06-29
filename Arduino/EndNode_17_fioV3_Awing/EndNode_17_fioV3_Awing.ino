/*
  CONFIGURATION for ENDNODE PCB
  20/06/2019
  Version 17.2 XX
  Author: Franco Parenti
  Url:
  Support for Arduino Fio V3, Arduino Leonardo
  XBee Series 2 Receiver
  Support for IR Emitter Type:R51M/E remote control
  Support for TV ir
  Support for Power monitor

  Support for Awning

  Setup compiler
  \hardware\arduino\avr\cores\Tone.cpp set -> #define USE_TIMER1
*/

// comment this if you use servo, leave this if you use IR remote control
#define REMOTECONTROL_MODE 1

// Include libraries //
#include <XBee.h>
#include "Timer.h"
#include <dht.h>
//#include <LEDFader.h>

//#include <IRremote.h>
// -- comment this part if you don't use remote control
// -- (optional) delete the file functions.ino from the sketch folder
// ---------- AC Remote control section ------------------------//
// Support for Midea, Hokkaido HVAC, Type:R51M/E remote control //
//#include <MideaIR.h>
//IRsend irsend;
//MideaIR remote_control(&irsend);
// ------------------------------------------------------------ //
#define PINNODE 0

// reserved pins
#define DHT_PIN   4 //DHT 
//#define IR_EMITER 5 //pin for IR Led
#define SERVO_PIN 9
#define PINt      24 //virtual pin for temperature
#define PINh      25 //virtual pin for humidity
#define RGB_PIN   15
#define EMON_PIN  1 //energy monitor input pin
#define PINup     10
#define PINdown   11
#define PINstop   14

#define PINRED0   7
#define PINGRE0   6
#define PINRED1   5
#define PINGRE1   9

#define PINe 26 //virtual pin for Enaergy Monitor

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
#define AWNING        13

#define AWNINGUP      39
#define AWNINGDOWN    40
#define AWNINGSTOP    41
#define AWNINGSTATUS0 42
#define AWNINGSTATUS1 43
#define AWCOMMANDTIME 1500 //time to keep the command on
#define AWRUNTIME     50000 //time to open/close the awning

#define AWNINGOPEN    1
#define AWNINGCLOSE   0
#define AWNINGSTARTED  2

#define AWNINGVALIDATED 1
#define AWNINGNOTVALIDATED  0

#define AWNINGSTOP    0
#define AWNINGRUN     1

#define SONY      1
#define SAMSUNG   2

//Awning
boolean awning_started_command = false;
long int awning_starttime_command = millis();
boolean awning_started = false;
long int awning_starttime = millis();
int awning_direction = AWNINGOPEN;
int awning_value = 0;
int awning_value_init;
// pin salita, pin discesa, status, started, value, initial value, initial time, runtime
struct Awnings {
  int pinr;
  int ping;
  int sts;
  int run;
  int value;
  int initvalue;
  long int inittime;
  long int runtime;
  int validate;
};

Awnings awnings[2] = {{PINRED0, PINGRE0, 0, 0, 0, 0L, AWNINGVALIDATED}, {PINRED1, PINGRE1, 0, 0, 0, 0L, AWNINGVALIDATED}};

#define IN 1
#define OUT 0

#define TIMEt0 200 //update sensor data
#define TIMEt1 2000 //send data to coordinator
#define TIMEt2 5000 //read and update DHT sensor data

#define BAUD_RATE     9600  // Baud for both Xbee and serial monitor
#define NUM_ACTU      7 //Insert here the (# of actuators x NUM_DATA_VAL)
#define NUM_METH      2 //Insert here the (# of methods x NUM_DATA_VAL)
#define NUM_DATA      9 //Insert here the (# of sensors x NUM_DATA_VAL)
#define NUM_DATA_VAL  3 // number of values transmitted for each sensor: number, value, alarm status
#define NUM_DATA_PTS  NUM_DATA*NUM_DATA_VAL
#define NUM_ACTU_PTS  NUM_ACTU*NUM_DATA_VAL
#define NUM_METH_PTS  NUM_METH*NUM_DATA_VAL
#define NUM_BYTE_ARR  3 // Number of bytes of the array used to store long integers in the payload

#define SENSOR 0
#define ACTUATOR 1
#define METHOD   2

#define RANGE_IN  1 //the actuator is in range 
#define RANGE_OUT 0 //the actuator is out of range 

#define NUM_RCVD_VAL  12 // Numero di valori ricevuri dal cordinatore 

/* SENSORS
   arrays don't support more than 10 cols so I create the index of sensors. Other values are in the trigger array
  //0=pin
  //1=value
*/
int sensors[NUM_DATA][2] = {{18, 0}, {19, 0}, {20, 0}, {21, 0}, {22, 0}, {23, 0}, {24, 0}, {25, 0}, {26, 0}};

/* TRIGGERS
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
int triggers[NUM_DATA][10] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

/* ACTUATORS
  //0=actuator number
  //1=status
  //2=output type
  //3=value
*/
long actuators[NUM_ACTU][4] = {{8, 0, 4, 0}, {10, 0, 0, 0}, {11, 0, 0, 0}, {14, 0, 0, 0}, {15, 0, 0, 0}, {16, 0, 0, 0}, {17, 0, 0, 0}};
//long actuators[NUM_ACTU][4] = {{6, 0, 0, 0}, {7, 0, 0, 0}, {8, 0, 4, 0}, {9, 0, 2, 0}, {10, 0, 0, 0}, {11, 0, 0, 0}, {14, 0, 0, 0}, {15, 0, 0, 0}, {16, 0, 0, 0}, {17, 0, 0, 0}};

//LEDFader actu[NUM_ACTU] = {6, 7, 8, 9, 10, 11, 14, 15, 16, 17};
dht DHT; // Object for DHT22 sensor

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

//convert received payload to long
union {
  long color;
  byte c[3];
} aRGB;

int res;
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

  for (int i = 0; i < NUM_ACTU; i++)
  {
    if (actuators[i][0] != SERVO_PIN) {
      pinMode(actuators[i][0], OUTPUT);
    }
  };

  for (int i = 0; i < NUM_DATA; i++)
  {
    if (sensors[i][0] != 24 && sensors[i][0] != 25)
      pinMode(sensors[i][0], INPUT);

    for (int n = 0; n < 10; n++) {
      triggers[i][n] = 0;
    }
  }
  // end initialize arrays and pins

  // reserved to I2C
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  //reserved to IR led
  //pinMode(IR_EMITER, OUTPUT);

  //Reserved to awning
  pinMode(PINup, OUTPUT);
  pinMode(PINdown, OUTPUT);
  pinMode(PINstop, OUTPUT);
  pinMode(PINRED0, INPUT);
  pinMode(PINGRE0, INPUT);
  pinMode(PINRED1, INPUT);
  pinMode(PINGRE1, INPUT);

  awnings[0].pinr = PINRED0;
  awnings[0].ping = PINGRE0;
  awnings[1].pinr = PINRED1;
  awnings[1].ping = PINGRE1;
  for (int i; i = 0; i++) {
    awnings[i].sts = AWNINGCLOSE;
    awnings[i].run = 0;
    awnings[i].value = 0;
  }

  // reserved to temp sensor
  pinMode(DHT_PIN, INPUT);

  //read Temperature ann Humidity for the first time
  delay(5000); // I wait for 5 sec before reading data
  updatePinValuesDHTinit(); //read DHT22 data

  // start serial xbee
  Serial.begin(BAUD_RATE);
  Serial1.begin(BAUD_RATE);
  xbee.setSerial(Serial1);
  digitalWrite(17, LOW);
  delay(1000);
  updatePinValues(); // read sensor once
  t0.every(TIMEt0, updatePinValues);
  t0.every(TIMEt1, sendSensorData);
  t0.every(TIMEt2, updatePinValuesDHT);

}  //setup()

//============================================================================
// continuously reads packets, looking for RX16 or RX64
//============================================================================
void loop()
{
  if (xbee.readPacket(1))
  {
    getData();
  }
  else
  {
    t0.update();
    checkAlarm();
    checkAwinig();
  }
} // loop()

void sendSensorData() {
  log(0);
  sendData(SENSOR); //send sensors
  delay(10);
  log(1);
  sendData(ACTUATOR); //send actuators
  delay(10);
  sendData(METHOD); //send methods
  log(14);
}

// *******************************************************//
// Functions                                              //
// *******************************************************//
void updatePinValues()
{
  // Update sensor values
  // 0=sensor number
  // 1=immediate alarm
  // 2=actuator
  // 3=level
  // 4=status
  for (int i = 0; i < NUM_DATA; i++)
  {
    if (sensors[i][0] == PINt || sensors[i][0] == PINh || sensors[i][0] == PINe) //check pin 24 and 25
    {
      if (sensors[i][0] == PINe) // if energy monitor
      {
        // read
        //        Irms = emon.calcIrms(100);
        //sensors[i][1] = round(Irms * 230.0);
      }
    }
    else
    { //read sensors
      sensors[i][1] = analogRead(sensors[i][0]);
    }
  }

  // Update actuators status
  // 0=actuator number
  // 1=status
  // 2=output type
  // 3=value
  for (int i = 0; i < NUM_ACTU; i++)
  {

    if (actuators[i][0] != SERVO_PIN) //if pin for servo
    {
      if (actuators[i][2] == DIGITAL) //if DIGITAL OUTPUT
      {
        //read digital output pins
        actuators[i][1] = digitalRead(actuators[i][0]);
      }
    }
  }
} // updatePinValues()

void updatePinValuesDHT()
{
  int chk = DHT.read22(DHT_PIN);
  int getval;
  // Update sensor values
  // 0=sensor number
  // 1=immediate alarm
  // 2=actuator
  // 3=level
  // 4=status
  for (int i = 0; i < NUM_DATA; i++)
  {
    if (sensors[i][0] == PINt || sensors[i][0] == PINh) //check pin 24 and 25
    {
      if (sensors[i][0] == PINt)
      {
        if (chk == DHTLIB_OK & DHT.temperature < 60)
        {
          float valt = DHT.temperature * 100;
          getval = (int) valt;
          if (sensors[i][1] + 500 > getval || sensors[i][1] - 500 < getval ) { //if the last read returns a value 5 C° greater or lower than the last value I assume it is wrong and discharge the value
            sensors[i][1] = getval;
          }
          else
            exit; //if no valid value exit loop
        }
      }
      else if (sensors[i][0] == PINh)
      {
        if (chk == DHTLIB_OK & DHT.humidity > 5)
        {
          float valh = DHT.humidity * 100;
          getval = (int) valh;
          sensors[i][1] = getval;
        }
      }
    }
  } //end FOR loop
  res = chk;
} // updatePinValuesDHT()


void updatePinValuesDHTinit()
{
  int tidx;
  int hidx;
  int getval;
  int chk; //store dht22 response

  // get the temp and humidity sensor array index
  for (int i = 0; i < NUM_DATA; i++)
  {
    if (sensors[i][0] == PINt)
      tidx = i;
    if (sensors[i][0] == PINh)
      hidx = i;
  }
  for (int i = 0; i < 10; i++) {
    chk = DHT.read22(DHT_PIN); // read data
    // Update sensor values

    if (chk == DHTLIB_OK & DHT.temperature < 60)
    {
      float valt = DHT.temperature * 100;
      sensors[tidx][1] = (int) valt;
      float valh = DHT.humidity * 100;
      sensors[hidx][1] = (int) valh;
      exit;
    }
    delay(2000); // wait 2 sec and retry
  }
} // updatePinValuesDHTinit()

// ******************************************************* //

// ******************************************************* //
void checkAlarm()
{
  int triggerOldStatus;
  // 0=sensor number 1=immediate alarm 2=actuator 3=level 4=status
  for (int i = 0; i < NUM_DATA; i++)
  {
    switch (triggers[i][0]) {
      case PERMANENT: //se è un allarme interno permanente
        if (sensors[i][1] >= triggers[i][2] && sensors[i][1] <= triggers[i][4] && triggers[i][3] == 0)
        {
          setPIN(triggers[i][1], triggers[i][6], triggers[i][5], triggers[i][8], triggers[i][9] );  // actuator, value, output type, timer, dimmer
          triggers[i][3] = RANGE_IN; //set alarm status
        }
        break;
      case TEMPORARY: //if it is se è un allarme interno temporaneo
        if (sensors[i][1] >= triggers[i][2] && sensors[i][1] <= triggers[i][4])
        {
          setPIN(triggers[i][1], triggers[i][6], triggers[i][5], triggers[i][8], triggers[i][9]);  // actuator, value, output type, timer, dimmer
          triggers[i][3] = RANGE_IN;
        }
        else // if it is off limits then reset the alarm
        {
          setPIN(triggers[i][1], triggers[i][7], triggers[i][5], triggers[i][8], triggers[i][9]);  // actuator, value, output type, timer, dimmer
          triggers[i][3] = RANGE_OUT;
        }
        break;
      case FLIPFLOP: //it is triggered when there is a change on the sensor status (from ACTION OUT OF RANGE(9) -> ACTOION IN RANGE(8))
        if (sensors[i][0] != -256) {
          triggerOldStatus = triggers[i][3]; // old (current) trigger status
          if (sensors[i][1] >= triggers[i][2] && sensors[i][1] <= triggers[i][4])
          {
            triggers[i][3] = RANGE_IN;
            if (triggerOldStatus == RANGE_OUT)
            {
              switch (getActuatorStatus(triggers[i][1], triggers[i][6], triggers[i][7]))
              {
                case RANGE_OUT:   //if it is "out of range" set "in range"
                  log(2);
                  setPIN(triggers[i][1], triggers[i][6], triggers[i][5], triggers[i][8], triggers[i][9]);  // actuator, value, output type, timer, dimmer
                  break;
                case RANGE_IN:   //if it is "in range" set "out of range"
                  log(3);
                  setPIN(triggers[i][1], triggers[i][7], triggers[i][5], triggers[i][8], triggers[i][9]);  // actuator, value, output type, timer, dimmer
                  break;
              }
            }
          }
          else // if out of range
          {
            triggers[i][3] = RANGE_OUT;
          }
        }
        break;
      case TIMER: //it is triggered when there is a change on the sensor status (from ACTION OUT OF RANGE(9) -> ACTOION IN RANGE(8) but returns to the original value after n seconds
        if (sensors[i][1] >= triggers[i][2] && sensors[i][1] <= triggers[i][4] && triggers[i][3] == 0)
        {
          setPIN(triggers[i][1], triggers[i][6], triggers[i][5], triggers[i][8], triggers[i][9]);  // actuator, value, output type, timer, dimmer
          triggers[i][3] = RANGE_IN; //set alarm status
        }
        break;
    }
  } //end FOR loop
} // checkAlarm()
// ******************************************************* //

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

// ******************************************************* //

// ******************************************************* //
void parseXbeeReceivedData(int x)
{
  int val = OFF;
  if (x == WRITE) {
    if (RxData[3] == DIGITAL) //digital
    {
      if (RxData[2] == 0)
      {
        val = OFF;
      }
      if (RxData[2] == 1)
      {
        val = ON;
      }
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
    if (RxData[3] == LEDRGB) //RGB strip
    {
      val = RxData[2];
    }
    if (RxData[3] == AWNING) //AWNING movement
    {
      val = RxData[2];
    }

    setPIN(RxData[1], val, RxData[3], RxData[4], RxData[5]);
    sendData(ACTUATOR); //send actuators value

  }
  if (x == ALARM)
  { // set the sensors and the triggers
    if (RxData[4] == RESETALARM)
    { //trigger reset
      triggers[getSensorId(RxData[1])][3] = OFF;
      setPIN(RxData[3], RxData[9], RxData[7], 0, 0);
    }
    else if (RxData[2] == 0)
    {
      //disable trigger
      triggers[getSensorId(RxData[1])][0] = OFF;
      //set actuator to "Out of range" value
      setPIN(triggers[getSensorId(RxData[1])][1], triggers[getSensorId(RxData[1])][9], triggers[getSensorId(RxData[1])][7], 0, 0);
    }
    else
    {
      //1,20,1,4,500,0,1024,0,1,0,0 set remote alarm digital
      //1,20,1,4,500,0,1024,1,255,0,0 set remote alarm analog
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
    log(5);
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
      if (RxData[0] == WRITE || RxData[0] == ALARM) { //[0]=1 command to set sensor, [0]=2 set trigger
        parseXbeeReceivedData(RxData[0]);
      }
      log(4);
    }
  }
  else if (xbee.getResponse().isError())
  { // Got something, but not a packet
    log(6);
  }
  else
  {
    // xbee not available and no error
    log(6);
  }
}
// ******************************************************* //

// ******************************************************* //
void sendData(int t) // t=0 = sensors 1 = actuators 2=methods
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
      for (int i = 0; i < NUM_DATA; i++)
      {
        delay(1); //to avoid reading wrong values
        xbeeData[idx] = sensors[i][0]; //sensor number
        idx++;
        xbeeData[idx] = sensors[i][1]; //value
        idx++;
        xbeeData[idx] = triggers[i][3]; // alarm
        idx++;
      }
      xbeeData[0] = SENSOR;
      parseTxData(payload, xbeeData, idx);
      break;

    case METHOD:
      xbeeMeth[0] = METHOD;

      xbeeMeth[idx] = PINNODE;
      idx++;
      xbeeMeth[idx] = AWNINGSTATUS0;
      idx++;
      xbeeMeth[idx] = awnings[0].value;
      //Serial.println(awnings[0].value);
      idx++;

      xbeeMeth[idx] = PINNODE;
      idx++;
      xbeeMeth[idx] = AWNINGSTATUS1;
      idx++;
      xbeeMeth[idx] = awnings[1].value;
      //Serial.println(awnings[1].value);
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
        log(8);
      }
      else
      {
        log(9);
      }
    }
    else if ((response == ZB_RX_RESPONSE))
    {
      log(10);
      getData();
    }
  }
  else if (xbee.getResponse().isError())
  {
    log(9);
  }

} // sendData()
// ******************************************************* //

// ******************************************************* //
void setPIN(int pin, int sts, int outputType, int p1, int p2)
{
  log(12);
  // p1 = time time, where output type = HVAV p1 = component
  // p2 = fading time
  int i = 0;
  int actuatorId = getActuatorId(pin);
  switch (outputType) {
    case DIGITAL: //digital
      if (p1 == 0) {
        digitalWrite(pin, sts);
      }
      else {
        t0.pulseImmediate(pin, (unsigned long)(p1 * 1000), sts);
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
        setDimmer(actuatorId, p2, sts);
      }
      // update actuator status
      if (sts == OFF) { //if = OFF
        actuators[actuatorId][1] = OFF; //set actuator status inactive
      }
      else { //if != OFF
        actuators[actuatorId][1] = ON; //set actuator status active
      }
      break;
    case TONE: //Tone
      Serial.println("tone");
      if (sts == OFF) {
        noTone(pin);
      }
      if (sts == ON) {
        tone(pin, 4500);
      }
      // update actuator status
      if (sts == OFF) { //if = OFF
        actuators[actuatorId][1] = OFF; //set actuator status inactive
      }
      else { //if != OFF
        actuators[actuatorId][1] = ON; //set actuator status active
      }
      break;

    case AWNING:  // Tende
      switch (sts) { //method
        case AWNINGUP:
          Serial.println("up");
          //attivo relay di salita
          if (awning_started_command == false) {
            tone(8, 4500, 100);
            digitalWrite(PINdown, LOW); //relay per impostare up/down up=HIGH down=LOW (1 tenda)
            digitalWrite(PINstop, LOW); //relay per impostare up/down up=HIGH down=LOW (1 tenda)
            digitalWrite(PINup, HIGH); //relay per impostare up/down up=HIGH down=LOW (1 tenda)
            awning_started_command = true;
            awning_starttime_command = millis();
            awning_started = true;
            awning_starttime = millis();
            awnings[0].sts = AWNINGOPEN;
            awnings[1].sts = AWNINGOPEN;
            awnings[0].validate = AWNINGVALIDATED;
            awnings[1].validate = AWNINGVALIDATED;
          }
          break;
        case AWNINGSTOP:
          if (awning_started_command == false) {
            tone(8, 4500, 100);
            digitalWrite(PINup, LOW); //relay per impostare up/down up=HIGH down=LOW (1 tenda)
            digitalWrite(PINdown, LOW); //relay per impostare up/down up=HIGH down=LOW (1 tenda)
            digitalWrite(PINstop, HIGH); //relay per impostare up/down up=HIGH down=LOW (1 tenda)
            awning_started_command = true;
            awning_starttime_command = millis();
            awning_started = false;
          }
          break;
        case AWNINGDOWN:
          //attivo relay di discesa
          if (awning_started_command == false) {
            tone(8, 4500, 100);
            digitalWrite(PINup, LOW); //relay per impostare up/down up=HIGH down=LOW (1 tenda)
            digitalWrite(PINstop, LOW); //relay per impostare up/down up=HIGH down=LOW (1 tenda)
            digitalWrite(PINdown, HIGH); //relay per impostare up/down up=HIGH down=LOW (1 tenda)
            awning_started_command = true;
            awning_starttime_command = millis();
            awning_started = true;
            awning_starttime = millis();
            awnings[0].sts = AWNINGCLOSE;
            awnings[1].sts = AWNINGCLOSE;
            awnings[0].validate = AWNINGVALIDATED;
            awnings[1].validate = AWNINGVALIDATED;
          }
          break;
      }
      break;
  }
  actuators[actuatorId][3] = sts; //set the actuator value

}


// ******************************************************* //
// Common functions                                        //
// ******************************************************* //
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
// ******************************************************* //
int getActuatorId(int pin)
{
  for (int i = 0; i < sizeof(actuators); i++)
  {
    if (actuators[i][0] == pin)
    {
      return i;
      break;
    }
  }
}
// ******************************************************* //
void setDimmer(int actuatorId, int time, int val) //set the dimmer initial value
{
  //LEDFader *act = &actu[actuatorId];
  //act->update();
  //if (act->is_fading() == true) {
  //act->stop_fade();
  //}
  log(13);
  //act->fade(val, time * 1000);
}
// ******************************************************* //
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
// ******************************************************* //
void log(int num)
{
  String msg[15];
  msg[0] = "Sending sensors ";
  msg[1] = "Sending actuators ";
  msg[2] = "Activate ";
  msg[3] = "Inactivate ";
  msg[4] = "Command to execute: ";
  msg[5] = "Response available - ";
  msg[6] = "No response ";
  msg[7] = "There is a response ";
  msg[8] = "Success!! ";
  msg[9] = "No success ";
  msg[10] = "Executing command ";
  msg[11] = "Dimmer stopped ";
  msg[12] = "Set Pin ";
  msg[13] = "Start Dimmer ";
  msg[14] = "Sending methods ";
  // See on serial monitor
  Serial.println( msg[num]);
}

void checkAwinig()
{
  long int awning_runtime_gap;
  int awning_value_temp;

  if (awning_started_command == true) {
    if (millis() - AWCOMMANDTIME > awning_starttime_command) {
      digitalWrite(10, LOW); //Disattiva il comando (fase/neutro)
      digitalWrite(11, LOW); //disattiva led tenda 1
      digitalWrite(14, LOW); //disattiva led tenda 2
      awning_started_command = false;
      tone(8, 4500, 100);
    }
  }

  //calculate last status
  for (int i = 0; i < 2; i++) {
    //Serial.println("xxxxxxxx");
    //Serial.println(digitalRead(awnings[i].pinr));
    //Serial.println(digitalRead(awnings[i].ping));
    if (digitalRead(awnings[i].pinr) == HIGH && digitalRead(awnings[i].ping) == HIGH) {
      if (awnings[i].run != AWNINGRUN) { //iniziallizzo tempo di esecuzione
        awnings[i].run = AWNINGRUN;
        //riproporziono awning_starttime in base al valore attuale awning_value
        if (awnings[i].sts == AWNINGOPEN) { //chiudo tenda
          awnings[i].runtime = AWRUNTIME * awnings[i].value / 100;
          awnings[i].sts = AWNINGCLOSE;
        }
        else if (awnings[i].sts == AWNINGCLOSE) { //apro tenda
          awnings[i].runtime = AWRUNTIME * (100 - awnings[i].value) / 100;
          awnings[i].sts = AWNINGOPEN;
        }
        awnings[i].inittime = millis();
        awnings[i].initvalue = awnings[i].value;
        //tone(8, 4500, 50);
      }
      else if (awnings[i].run == AWNINGRUN) {

        awning_runtime_gap = millis() - awnings[i].inittime;
        if (awnings[i].sts == AWNINGOPEN) { //apro tenda
          awning_value_temp = (float)awnings[i].initvalue + ((float)awning_runtime_gap / (float)awnings[i].runtime * (float)100);
        }
        else if (awnings[i].sts == AWNINGCLOSE) { //chiudo tenda
          awning_value_temp = (float)awnings[i].initvalue - ((float)awning_runtime_gap / (float)awnings[i].runtime * (float)100);
        }

        if (awning_value_temp >= 100) {
          awnings[i].value  = 100;
        }
        else if (awning_value_temp <= 0) {
          awnings[i].value  = 0;
        }
        else {
          awnings[i].value = (int)awning_value_temp;
        }
        awnings[i].validate = AWNINGNOTVALIDATED;
      }
    }

    //fine corsa aperto
    if (digitalRead(awnings[i].pinr) == HIGH && digitalRead(awnings[i].ping) == LOW) {
      awnings[i].sts = AWNINGOPEN;
      awnings[i].run = AWNINGSTOP;
      awnings[i].value  = 100;
      awnings[i].validate = AWNINGVALIDATED;
    }

    //fine corsa chiuso
    if (digitalRead(awnings[i].pinr) == LOW && digitalRead(awnings[i].ping) == HIGH) {
      awnings[i].sts = AWNINGCLOSE;
      awnings[i].run = AWNINGSTOP;
      awnings[i].value  = 0;
      awnings[i].validate = AWNINGVALIDATED;
      //Serial.println("chiuso");
    }

    if (digitalRead(awnings[i].pinr) == LOW && digitalRead(awnings[i].ping) == LOW) {
      awnings[i].run = AWNINGSTOP;
      if (awnings[i].validate == AWNINGNOTVALIDATED) {
        awnings[i].value  = 50;
      }
    }
  }
}
