/*
  DH HUB
  Platform: Arduino Due MULTITASKING (Arduino DUE only)
  Purpose: Collect data from/to end nodes and link from/to the main database
           Receive data from end nodes
           Send commands to end nodes
           Send data to the main database (on Serial)

  Author: Franco Parenti

  Rel. 16 XX
  27/10/2017

  Before compiling:
  Set the Serial Buffer to 2048 in the Arduino IDE
  /hardware/arduino/
  or
  AppData/Local/Arduino15/
  /sam/cores/arduino/RingBuffer.h -> #define SERIAL_BUFFER_SIZE 2048

  Command Examples (on Serial)
  IX2,1,1286656,1085167784,2,1286656,1085167843
  IX1,1,1286656,1085167843
  IS60,1,18,0,0,0,1,19,0,0,0,1,20,0,0,0,1,21,0,0,0,1,22,0,0,0,1,23,0,0,0,2,18,0,0,0,2,19,0,0,0,2,20,0,0,0,2,21,0,0,0,2,22,0,0,0,2,23,0,0,0
  IN10,1,0,0,0,0,2,0,0,0,0
  IA10,2,4,0,0,0,2,5,0,0,0
  ID1 enable debug
  ID0 disable debug
  IT,50,30,100,1000,5000,90

  CR1 send node data
  CA2,20,1,4,500,0,0,1024,0,1,0,0 set remote alarm digital
  CA2,20,1,4,500,0,0,1024,1,255,0,0 set remote alarm analog
  CA2,20,1,4,-999 reset remote alarm
  CR0 read sensors
  CR3 read actuators
  CR5 read Send all data ON serial
  CW2,4,1,0,0 //set actuator
  CT2,31,5,2017,13,46 //set time
  CM2,12.4,57,1024,2 //set current meteo //ext node, temp, ext hum, ext press, meteo
  CF2,1,2,3,4 //set meteo forecast //ext node, day1, day2, day3, day4
  CG2,10,20,11,21,12,22,13,23 //set meteo forecast temperature //tmin1, tmax1, tmin2, tmax2, tmin3, tmax3, tmin4, tmax4
*/

//------------------------------------------------------
//----------------- Declare functions ------------------
//------------------------------------------------------
void setLED();
void readSerialData(int x);
uint32_t getNodeByAddress(uint32_t Lsb, uint32_t Msb); //return node number
void setNodeStatus(int n, int sts); // node index
int getNodeType(int node); // node number
void setErrorLed(int n, int sts); // node index
void setStatusLed(int node, int sts); // node number
void updActuator(int node, int actuator, int value); // node number
int getNodeStatus(int node); // node number
int getNodeIndex(int node); // node number
int sendRemoteCommand(int node); // node number
boolean checkSerial();
void updateNodeStatus();
int getData();
void initialize();
void resetTable(byte x);
void initTable(byte x, int cnt);
void initXbeeAddressTable(int cnt);

void sendDataOnSerial(int x);
void sendAllData();

int debug = 0;

#include <XBee.h>  //http://code.google.com/p/xbee-arduino/     Modified per http://arduino.cc/forum/index.php/topic,111354.0.html
#include "Timer.h"
#include <Scheduler.h>

#define SERIAL_BUFFER_SIZE 2048
#define BAUD_RATE_XBEE 115200  // Baud for Xbee serial
#define BAUD_RATE 115200       // Baud for serial

#define SENSOR 0
#define ACTUATOR 1
#define METHOD 2

#define DEBUG 88
#define INIT 73
#define SENSORS 83
#define NODES 78
#define METHODS 77
#define ACTUATORS 65
#define TUNING 84
#define SWDEBUG 68
#define XBEEADDR 88
#define COMMAND 67
#define READ 82
#define WRITE 87
#define SETALARM 65
#define SETTIME 84
#define METEO 77
#define FORECAST 70
#define FORECASTEMP 71

// define params status for Xbee transmission
#define receivedOK 1
#define noResponse 0
#define associateOK 888
#define associateNotOK 889
#define dataError 2
#define xbeeError 3
#define nodeStatusOk 1
#define nodeStatusNotOk 0

// pins for the service LEDs
#define STSLed 21
#define DATLed 4
#define ERRLed 12
#define SERLed 13

//Service Led status
#define ON HIGH
#define OFF LOW

// define node types
#define nLocal 1 //Node for local sensors
#define nXbee 2 //Xbee node

int TIMEt0 = 1000; //send data on serial every n millisec
Timer t0; //timer to schedule the update Sensor and Actuator data

int delayMainLoop = 5;
int delayXbee = 5; //set the delay to receive response from xbee nodes
int delayXbeeS = 1; //set the delay to receive actuators data after sensors
int delayXbeeAfterSent = 10;
int timeoutXbeeResponse = 5; //Xbee nodes timeout. Returns error after this interval
const unsigned long nodeTimeOut = 6000UL;
const unsigned long receiveResponseTimeout = 2000UL; //receive response after a command is sent
int readInt[1024]; //variable to receive data from serial

//define array to store node,sensor,actuators data
#define NUMCOLS 5 //num column arrays. Don't change this value unless you know what are doing.
#define NUMROWS 256 //num rows arrays. Supports 256 Sensors and 256 Actuators. This value can be increased if needed
#define NUMNODS 64 //num rows arrays, Supports 64 Nodes. This value can be increased if needed

//define the array to store Sensors data values
//col 0=node 1=sensor 2=value 3=alarm 4=spare
long aDataTable[NUMROWS][NUMCOLS];
//define the array to store Nodes status values
//col 0=node 1=type (0 = Xbee) 2=spare 3=signal level 4=status 5=last error 6 = #of error 7= NI
int aNodeTable[NUMNODS][8]; //nodes tables
unsigned long aNodeLastUpdate[NUMNODS]; //last time a node was live
//define the array to store Actuators data values
//col 0=node 1=Actuator 2=status 3=value 4=0 actuator, 99 method
long aActuTable[NUMROWS][NUMCOLS];
//col 0=node 1=Actuator 2=method 3=value 4=0 spare
int aMethTable[NUMROWS][NUMCOLS];

const int initString = 0xFFFE;

//Enable/Disable data transmition on serial. false=Disable
boolean procStarted = false;

//------------------------------------------------------
//--- XBee ---------------------------------------------
//------------------------------------------------------
//uncomment the Xbee type used (S2 preferred)
//#define S1 1 //Xbee series 1
#define S2 1 //Xbee series 2

// portability macro
#ifdef S1
#define TX_RESPONSE TX_STATUS_RESPONSE
#define RX_RESPONSE RX_64_RESPONSE
#define ZB_TX_UNKNOWN 0xBF
#define request(addr64, payload, sizeofPayload) Tx64Request tx = Tx64Request(addr64, payload, sizeofPayload)
#define getStatusTx() txStatus.getStatus()
#define TXStatusResponse(txStatus) xbee.getResponse().getTxStatusResponse(txStatus)
TxStatusResponse txStatus = TxStatusResponse();
Rx64Response rx = Rx64Response();
#define RXStatusResponse(rx) xbee.getResponse().getRx64Response(rx)
#define getApiId() xbee.getResponse().getApiId()
#define getRssi() rx.getRssi()
ModemStatusResponse msr = ModemStatusResponse();
#define getModemStatusResponse() xbee.getResponse().getModemStatusResponse(msr)
#define getStatusModem() msr.getStatus()
#endif

#ifdef S2
#define TX_RESPONSE ZB_TX_STATUS_RESPONSE
#define RX_RESPONSE ZB_RX_RESPONSE
#define ZB_TX_UNKNOWN 0xBF
#define request(addr64, payload, sizeofPayload) ZBTxRequest tx = ZBTxRequest(addr64, payload, sizeofPayload)
#define getStatusTx() txStatus.getDeliveryStatus()
#define TXStatusResponse(txStatus)  xbee.getResponse().getZBTxStatusResponse(txStatus)
ZBTxStatusResponse txStatus = ZBTxStatusResponse();
#define setFrameId() tx.setFrameId(88)
ZBRxResponse rx = ZBRxResponse();
#define RXStatusResponse(rx) xbee.getResponse().getZBRxResponse(rx)
#define getApiId() xbee.getResponse().getApiId()
#define getFrameIdRx() txStatus.getFrameId()
#define getFrameIdTx() tx.getFrameId()
#define getRssi() 100
ModemStatusResponse msr = ModemStatusResponse();
#define getModemStatusResponse() xbee.getResponse().getModemStatusResponse(msr)
#define getStatusModem() msr.getStatus()
#endif

//Xbee object instances
XBee xbee = XBee();
#define NUM_DATA_PTS  12 // Number of integers (data points) to upload. Can't exceed 100 bytes or 50 integers unless you change MAX_FRAME_DATA_SIZE in XBee.h (Library Xbee)
#define NUM_RCV_VAL   24 // Number of received Sensor values from a remote node
#define NUM_ACT_VAL   30 // Number of received Actuator values from a remote node
#define NUM_DATA_VAL   3 // numero valori per sensore
#define NUM_BYTE_ARR   3 // Number of bytes of the array used to store long integers in the payload
uint8_t payload[NUM_DATA_PTS * NUM_BYTE_ARR];
long xbeeData[NUM_DATA_PTS];  // Array to hold integers that will be sent to other xbee
#define RX_MAX_VAL 30 //rxData array size to store received data from node. Must be the same of endnodes
//remote Xbee node address
XBeeAddress64 RCV_ADDR = XBeeAddress64(0xFF, 0xFF);
uint32_t aXbeeAddressTable[NUMNODS][2]; //0=High 1=Low
uint8_t signalStrength;
//------------------------------------------------------
//------------------------------------------------------

void setup() {

  Scheduler.startLoop(loop1); //start loop 1. Read data from remote nodes
  Scheduler.startLoop(loop2); //start loop 2. Update status of remote nodes

  // initialize serial:
  Serial.begin(BAUD_RATE); //serial
  Serial1.begin(BAUD_RATE_XBEE); //xbee serial
  // xbee start serial
  xbee.setSerial(Serial1);

  // make the pins outputs:
  pinMode(STSLed, OUTPUT);
  pinMode(DATLed, OUTPUT);
  pinMode(ERRLed, OUTPUT);
  pinMode(SERLed, OUTPUT);

  for (int i = 20; i < 54; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  setLED(STSLed, OFF);
  setLED(DATLed, OFF);
  setLED(ERRLed, OFF);
  setLED(SERLed, OFF);
  initialize();

  t0.every(TIMEt0, sendAllData); //set time interval to read data from remote nodes millis
}

// ---------- Main loop ------------- //
// ---------- Main loop ------------- //
// ---------- Main loop ------------- //
void loop() {
  getSerialData();
  t0.update();
  delay(10);
}

void loop1() {
  getData();
  setLED(SERLed, OFF);
  delay(50);
}

void loop2() {
  updateNodeStatus();
  delay(50);
  yield();
}
// -------- End Mail loop ----------- //
// -------- End Mail loop ----------- //
// -------- End Mail loop ----------- //

//-------------------------------------------------
// Functions
//-------------------------------------------------
void getSerialData() {
  int ret; //variable to return the response from xbee
  int retd; //variable return the response from xbee data received
  byte inChar; //store received bytes fron serial
  int intNum = 0;
  int nodeTemp;
  int nodeType;

  setLED(DATLed, OFF);
  // if there's any serial available, read it:
  if (Serial.available() > 0) {
    setLED(SERLed, ON);
    delay(10);
    inChar = Serial.read();  //read the first byte from the serial buffer
    //------------------------------------------------
    switch (inChar) {
      case DEBUG:  // X debug
        for (int i = 0; i < 5; i++) {
          if (aNodeTable[i][0] != initString) {
            Serial.print("Node ");
            Serial.print(aNodeTable[i][0]);
            Serial.print(" Errors ");
            Serial.print(aNodeTable[i][(NUMCOLS + 2) - 1]);
            Serial.print(" Last error ");
            Serial.println(aNodeTable[i][(NUMCOLS + 1) - 1]);
          }
        }
        break;

      case INIT:  // I init = Set common Arrays (sensors,nodes,actuators,addresses)
        inChar = Serial.read();  //read the first byte from the serial buffer
        //es. IS4,1,2,3,4
        if (inChar == SENSORS) {  // S sensors
          procStarted = false; //stop sending data on serial
          intNum = Serial.parseInt();
          readSerialData(intNum);
          initTable(inChar, intNum);
          Serial.println("CX1");
        }
        else if (inChar == NODES) {  // N nodes
          procStarted = false; //stop sending data on serial
          intNum = Serial.parseInt();
          readSerialData(intNum);
          initTable(inChar, intNum);
          Serial.println("CX1");
        }
        else if (inChar == METHODS) {  // M methods
          procStarted = false; //stop sending data on serial
          intNum = Serial.parseInt();
          readSerialData(intNum);
          initTable(inChar, intNum);
          Serial.println("CX1");
        }
        else if (inChar == ACTUATORS) {  // A Actuators
          procStarted = false; //stop sending data on serial
          intNum = Serial.parseInt();
          readSerialData(intNum);
          initTable(inChar, intNum);
          Serial.println("CX1");
        }
        // IT,50,30,100
        else if (inChar == TUNING) {  // T tuning Delay Variables
          delayXbee = Serial.parseInt(); //set the delay to receive response from xbee nodes
          delayXbeeS = Serial.parseInt(); //set the delay to receive actuators data after sensors
          delayXbeeAfterSent = Serial.parseInt();
          timeoutXbeeResponse = Serial.parseInt();
          delayMainLoop = Serial.parseInt();
          TIMEt0 = Serial.parseInt();
          Serial.println("CX1");
        }
        // IT,1
        else if (inChar == SWDEBUG) {  // D debug on/off
          debug = Serial.parseInt(); // enable disable debug; IT1=enable IT0=disable
          Serial.println("CX1");
        }
        // es. IX2,node,integer Low, integer High,node,integer Low, integer High  (indirizzi Xbee in decimale)
        else if (inChar == XBEEADDR) {  // X Xbee addresses
          intNum = Serial.parseInt();
          initXbeeAddressTable(intNum);
          Serial.println("CX1");
        }
        break;
      //------------------------------------------------
      // Fine sezione inizializzazione
      //------------------------------------------------

      //---------------------------------------
      //  Commands
      // es. CR0 return all remote sensors data
      // es. CR1 return all remote node data
      // es. CR2 return all Xbee node addresses
      // es. CR3 return all remote actuator data
      // es. CW1,5,123,0,0,0 set the value 123 on node 1, pin 5, Digital output, no dimmer,  no timer
      //---------------------------------------
      case COMMAND:  // C command
        inChar = Serial.read();  //read the second byte on serial
        if (inChar == READ) {  // R read
          intNum = Serial.parseInt();
          sendDataOnSerial(intNum);
        }
        else if (inChar == WRITE) {  // W write --da definire
          // es. CW1,5,123,0,0,0 set the value 123 on node 1, pin 5, Digital output, no dimmer,  no timer
          nodeTemp = Serial.parseInt(); //node
          xbeeData[0] = 1; //write
          xbeeData[1] = Serial.parseInt(); //actuator
          xbeeData[2] = Serial.parseInt(); //value
          xbeeData[3] = Serial.parseInt(); //output type
          xbeeData[4] = Serial.parseInt(); //timer time
          xbeeData[5] = Serial.parseInt(); //dimmer time
          xbeeData[6] = 0; //spare
          xbeeData[7] = 0; //spare
          xbeeData[8] = 0; //spare
          xbeeData[9] = 0; //spare
          xbeeData[10] = 0; //spare
          xbeeData[11] = 0; //spare
          nodeType = getNodeType(nodeTemp);
          if (nodeType == nXbee) {   // If it is a Xbee node
            ret = sendRemoteCommand(nodeTemp); //send command to the remote node
            if (ret == 0) { //if 0=Ok command sent
              Serial.println("CX1");  //send on serial to confirm the command is sent
              updActuator(nodeTemp, xbeeData[1], xbeeData[2]);
            }
            else {
              Serial.println("CX0");  //send on serial to confirm the command is NOT sent
            }
          }
          else if (nodeType == nLocal) //if it the local node (for sensors and actuator directly connected to th HUB)
          {
            //insert code here
            Serial.println("CX1");  //send on serial to confirm the command is sent
          }
          // insert here other node types
        }
        else if (inChar == SETALARM) {  // A set alarm
          /*
            2,20,1,4,500,0,0,1024,0,1,0,0 set remote alarm digital
            2,20,1,4,500,0,0,1024,1,255,0,0 set remote alarm analog
            2,20,1,4,-999,0,0,0,0,0,0,0 reset remote alarm
          */
          nodeTemp = Serial.parseInt(); //node
          xbeeData[0] = 2; //write
          xbeeData[1] = Serial.parseInt(); //sensor
          xbeeData[2] = Serial.parseInt(); //value
          xbeeData[3] = Serial.parseInt(); //value
          xbeeData[4] = Serial.parseInt(); //value
          xbeeData[5] = Serial.parseInt(); //value
          xbeeData[6] = Serial.parseInt(); //value
          xbeeData[7] = Serial.parseInt(); //value
          xbeeData[8] = Serial.parseInt(); //value
          xbeeData[9] = Serial.parseInt(); //value
          xbeeData[10] = Serial.parseInt(); //timer time
          xbeeData[11] = Serial.parseInt(); //dimmer time
          nodeType = getNodeType(nodeTemp);
          if (nodeType == nXbee) {   // If it is a Xbee node
            ret = sendRemoteCommand(nodeTemp); //send command to the remote node
            if (ret == 0) { //if 0=Ok command sent
              //aNodeTable[nodeTemp][4] = nodeStatusOk; //setta lo status del nodo 0=ok 1=non raggiungibile
              Serial.println("CX1");   //send on serial to confirm the command is sent
            }
            else
            {
              //aNodeTable[nodeTemp][4] = nodeStatusNotOk; //setta lo status del nodo 0=ok 1=non raggiungibile
              Serial.println("CX0");   //send on serial to confirm the command is NOT sent
            }
          }
          else if (nodeType == nLocal) //if it the local node (for sensors and actuator directly connected to th HUB)
          {
            //insert code here
            Serial.println("CX1");  //send on serial to confirm the command is sent
          }
          // insert here other node types
        }
        else if (inChar == SETTIME) {  // T Set Time
          /*
            2,0,0,9,31,5,2017,13,46 set time
          */
          nodeTemp = Serial.parseInt(); //node
          xbeeData[0] = 3; //time
          xbeeData[1] = Serial.parseInt(); //day
          xbeeData[2] = Serial.parseInt(); //month
          xbeeData[3] = Serial.parseInt(); //year
          xbeeData[4] = Serial.parseInt(); //hour
          xbeeData[5] = Serial.parseInt(); //minutes
          xbeeData[6] = 0; //not used
          xbeeData[7] = 0; //not used
          xbeeData[8] = 0; //not used
          xbeeData[9] = 0; //not used
          xbeeData[10] = 0; //not used
          xbeeData[11] = 0; //not used
          nodeType = getNodeType(nodeTemp);
          if (nodeType == nXbee) {   // If it is a Xbee node
            ret = sendRemoteCommand(nodeTemp); //send command to the remote node
            if (ret == 0) { //if 0=Ok command sent
              //aNodeTable[nodeTemp][4] = nodeStatusOk; //setta lo status del nodo 0=ok 1=non raggiungibile
              Serial.println("CX1");   //send on serial to confirm the command is sent
            }
            else
            {
              //aNodeTable[nodeTemp][4] = nodeStatusNotOk; //setta lo status del nodo 0=ok 1=non raggiungibile
              Serial.println("CX0");   //send on serial to confirm the command is NOT sent
            }
          }
          else if (nodeType == nLocal) //if it the local node (for sensors and actuator directly connected to th HUB)
          {
            //insert code here
            Serial.println("CX1");  //send on serial to confirm the command is sent
          }
          // insert here other node types
        }
        else if (inChar == METEO) {  // M meteo
          /*
            2,123,55,1024,2 set meteo
          */
          nodeTemp = Serial.parseInt(); //node
          xbeeData[0] = 4; //meteo
          xbeeData[1] = Serial.parseInt(); //ext temp
          xbeeData[2] = Serial.parseInt(); //ext humidity
          xbeeData[3] = Serial.parseInt(); //ext pressure
          xbeeData[4] = Serial.parseInt(); //meteo
          xbeeData[5] = 0; //not used
          xbeeData[6] = 0; //not used
          xbeeData[7] = 0; //not used
          xbeeData[8] = 0; //not used
          xbeeData[9] = 0; //not used
          xbeeData[10] = 0; //not used
          xbeeData[11] = 0; //not used
          nodeType = getNodeType(nodeTemp);
          if (nodeType == nXbee) {   // If it is a Xbee node
            ret = sendRemoteCommand(nodeTemp); //send command to the remote node
            if (ret == 0) { //if 0=Ok command sent
              //aNodeTable[nodeTemp][4] = nodeStatusOk; //setta lo status del nodo 0=ok 1=non raggiungibile
              Serial.println("CX1");   //send on serial to confirm the command is sent
            }
            else
            {
              //aNodeTable[nodeTemp][4] = nodeStatusNotOk; //setta lo status del nodo 0=ok 1=non raggiungibile
              Serial.println("CX0");   //send on serial to confirm the command is NOT sent
            }
          }
          else if (nodeType == nLocal) //if it the local node (for sensors and actuator directly connected to th HUB)
          {
            //insert code here
            Serial.println("CX1");  //send on serial to confirm the command is sent
          }
          // insert here other node types
        }
        else if (inChar == FORECAST) {  // F meteo forecast
          /*
            2,123,55,1024,2 set meteo
          */
          nodeTemp = Serial.parseInt(); //node
          xbeeData[0] = 5; //meteo forecast
          xbeeData[1] = Serial.parseInt(); //day1
          xbeeData[2] = Serial.parseInt(); //day2
          xbeeData[3] = Serial.parseInt(); //day3
          xbeeData[4] = Serial.parseInt(); //day4
          xbeeData[5] = 0; //not used
          xbeeData[6] = 0; //not used
          xbeeData[7] = 0; //not used
          xbeeData[8] = 0; //not used
          xbeeData[9] = 0; //not used
          xbeeData[10] = 0; //not used
          xbeeData[11] = 0; //not used
          nodeType = getNodeType(nodeTemp);
          if (nodeType == nXbee) {   // If it is a Xbee node
            ret = sendRemoteCommand(nodeTemp); //send command to the remote node
            if (ret == 0) { //if 0=Ok command sent
              //aNodeTable[nodeTemp][4] = nodeStatusOk; //setta lo status del nodo 0=ok 1=non raggiungibile
              Serial.println("CX1");   //send on serial to confirm the command is sent
            }
            else
            {
              //aNodeTable[nodeTemp][4] = nodeStatusNotOk; //setta lo status del nodo 0=ok 1=non raggiungibile
              Serial.println("CX0");   //send on serial to confirm the command is NOT sent
            }
          }
          else if (nodeType == nLocal) //if it the local node (for sensors and actuator directly connected to th HUB)
          {
            //insert code here
            Serial.println("CX1");  //send on serial to confirm the command is sent
          }
          // insert here other node types
        }  
        else if (inChar == FORECASTEMP) {  // G meteo forecast tempurature
          /*
            2,123,55,1024,2 set meteo
          */
          nodeTemp = Serial.parseInt(); //node
          xbeeData[0] = 6; //meteo forecast temperature
          xbeeData[1] = Serial.parseInt(); //day1 min
          xbeeData[2] = Serial.parseInt(); //day1 max
          xbeeData[3] = Serial.parseInt(); //day2 min
          xbeeData[4] = Serial.parseInt(); //day2 max
          xbeeData[5] = Serial.parseInt(); //day3 min
          xbeeData[6] = Serial.parseInt(); //day3 max
          xbeeData[7] = Serial.parseInt(); //day4 min
          xbeeData[8] = Serial.parseInt(); //day4 max
          xbeeData[9] = 0; //not used
          xbeeData[10] = 0; //not used
          xbeeData[11] = 0; //not used
          nodeType = getNodeType(nodeTemp);
          if (nodeType == nXbee) {   // If it is a Xbee node
            ret = sendRemoteCommand(nodeTemp); //send command to the remote node
            if (ret == 0) { //if 0=Ok command sent
              //aNodeTable[nodeTemp][4] = nodeStatusOk; //setta lo status del nodo 0=ok 1=non raggiungibile
              Serial.println("CX1");   //send on serial to confirm the command is sent
            }
            else
            {
              //aNodeTable[nodeTemp][4] = nodeStatusNotOk; //setta lo status del nodo 0=ok 1=non raggiungibile
              Serial.println("CX0");   //send on serial to confirm the command is NOT sent
            }
          }
          else if (nodeType == nLocal) //if it the local node (for sensors and actuator directly connected to th HUB)
          {
            //insert code here
            Serial.println("CX1");  //send on serial to confirm the command is sent
          }
          // insert here other node types
        }   
        setLED(SERLed, OFF);
        break;
    }
    //------------------------------------------------
    // End Commands section
    //------------------------------------------------
  }
} // getSerialData()

void sendAllData()
{
  if (procStarted == true)
    sendDataOnSerial(5);
}


//-----------------------------------------------
//send all sensor data in aDataTable array if x=0
//send all node data in aNodeTable array if x=1
//send all actuators data in aActuTable array if x=3
//send all methods data in aMethTable array if x=9
//-----------------------------------------------
void sendDataOnSerial(int x) {
  String stringToSend;
  if (x == 0) {
    //Serial.print("DS,");
    for (int i = 0; i < NUMROWS; i++)
    {
      if (aDataTable[i][0] != initString)
      {
        for (int y = 0; y < NUMCOLS; y++) {
          Serial.print(aDataTable[i][y], DEC);
          Serial.print(",");
        }
      }
      else
      {
        Serial.println('\r\n');
        break;
      }
    }
  }
  if (x == 1) {
    //Serial.print("DN,");
    for (int i = 0; i < NUMNODS; i++)
    {
      if (aNodeTable[i][0] != initString)
      {
        for (int y = 0; y < NUMCOLS; y++) {
          Serial.print(aNodeTable[i][y], DEC);
          Serial.print(",");
        }
      }
      else
      {
        Serial.println('\r\n');
        break;
      }
    }
  }
  if (x == 2) {
    uint32_t trailingString = 0x0;
    //Serial.print("DA,");
    for (int i = 0; i < NUMNODS; i++)
    {
      if (aXbeeAddressTable[i][1] != trailingString)
      {
        Serial.print(aXbeeAddressTable[i][0], HEX);
        Serial.print(aXbeeAddressTable[i][1], HEX);
        Serial.print(",");
      }
    }
  }
  if (x == 3) {
    //Serial.print("DS,");
    for (int i = 0; i < NUMROWS; i++)
    {
      if (aActuTable[i][0] != initString)
      {
        for (int y = 0; y < NUMCOLS; y++) {
          Serial.print(aActuTable[i][y], DEC);
          Serial.print(",");
        }
      }
      else
      {
        Serial.println('\r\n');
        break;
      }
    }
  }
  if (x == 5) { //send nodes,sensors,actuators,methods
    //-----------------------------------------------------
    //-----------------------------------------------------
    //Nodes
    for (int i = 0; i < NUMNODS; i++)
    {
      if (aNodeTable[i][0] != initString)
      {
        stringToSend.concat("1,"); //prefix for nodes
        for (int y = 0; y < NUMCOLS; y++) {
          stringToSend.concat(aNodeTable[i][y]);
          stringToSend.concat(",");
        }
      }
      else
      {
        break;
      }
    }
    //sensors
    for (int i = 0; i < NUMROWS; i++)
    {
      if (aDataTable[i][0] != initString)
      {
        stringToSend.concat("0,"); //prefix for nodes
        for (int y = 0; y < NUMCOLS; y++) {
          stringToSend.concat(aDataTable[i][y]);
          stringToSend.concat(",");
        }
      }
      else
      {
        break;
      }
    }
    //actuators
    for (int i = 0; i < NUMROWS; i++)
    {
      if (aActuTable[i][0] != initString)
      {
        stringToSend.concat("3,"); //prefix for nodes
        for (int y = 0; y < NUMCOLS; y++) {
          stringToSend.concat(aActuTable[i][y]);
          stringToSend.concat(",");
        }
      }
      else
      {
        break;
      }
    }
    //methods
    for (int i = 0; i < NUMROWS; i++)
    {
      if (aMethTable[i][0] != initString)
      {
        stringToSend.concat("9,"); //prefix for methods
        for (int y = 0; y < NUMCOLS; y++) {
          stringToSend.concat(aMethTable[i][y]);
          stringToSend.concat(",");
        }
      }
      else
      {
        break;
      }
    }
    Serial.print(stringToSend);
    Serial.println('\r\n');
  }  //end IF = 5
}// sendDataOnSerial

//-------------------------------------------------
//Init functions
//-------------------------------------------------

//-------------------------------------------------
// update Xbee address table ArrayIndex = Xbee node
//-------------------------------------------------
void initXbeeAddressTable(int cnt) {
  uint32_t inNode;
  uint32_t inAddrL;
  uint32_t inAddrH;
  for (int i = 0; i < cnt; i++) {
    inNode = Serial.parseInt();
    inAddrH = Serial.parseInt();
    inAddrL = Serial.parseInt();
    aXbeeAddressTable[inNode][1] = inAddrL;
    aXbeeAddressTable[inNode][0] = inAddrH;
    //aXbeeAddressTable[i][0] = inAddrH;
    //aXbeeAddressTable[i][1] = inAddrL;
    //aXbeeAddressTableIndex[i] = inNode;
  }
}

//-----------------------------------------
// set data from Serial into the data tables
//-----------------------------------------
void initTable(byte x, int cnt) {
  int idx = 0; //row num
  int i = 0;
  int y = 0;

  resetTable(x);
  while (i < cnt) {
    y = 0; //reset column counter
    for (y = 0; y < NUMCOLS; y++) {
      if (x == 83) { // S sensors
        aDataTable[idx][y] = readInt[i];
      }
      if (x == 78) { // N nodes
        aNodeTable[idx][y] = readInt[i];
      }
      if (x == 77) { // M methods
        aMethTable[idx][y] = readInt[i];
      }
      if (x == 65) { // A Actuators
        aActuTable[idx][y] = readInt[i];
      }
      i++;
    }
    idx++;
  }

  if (x == 78) procStarted = true; //Enable data transmition on Serial

} // end intTable

void resetTable(byte x) {
  switch (x)
  {
    case SENSORS:
      for (int i = 0; i < NUMROWS; i++) {
        aDataTable[i][0] = initString;
        for (int y = 1; y < NUMCOLS; y++) {
          aDataTable[i][y] = 0;
        }
      }
      break;
    case NODES:
      for (int i = 0; i < NUMNODS; i++) {
        aNodeTable[i][0] = initString;
        for (int y = 1; y < NUMCOLS; y++) {
          aNodeTable[i][y] = 0;
        }
      }
      break;
    case  ACTUATORS:
      for (int i = 0; i < NUMROWS; i++) {
        aActuTable[i][0] = initString;
        for (int y = 1; y < NUMCOLS; y++) {
          aActuTable[i][y] = 0;
        }
      }
      break;
    case METHODS:
      for (int i = 0; i < NUMROWS; i++) {
        aMethTable[i][0] = initString;
        for (int y = 1; y < NUMCOLS; y++) {
          aMethTable[i][y] = 0;
        }
      }
      break;
  }
} // end resetTable

//------------------
// initialize arrays
//------------------
void initialize() {
  //inizializza array dei dati
  for (int i = 0; i < NUMROWS; i++) {
    aDataTable[i][0] = initString;
    aActuTable[i][0] = initString;
    aMethTable[i][0] = initString;
  }
  for (int i = 0; i < NUMNODS; i++) {
    aNodeTable[i][0] = initString;
    aNodeTable[i][(NUMCOLS + 2) - 1] = 0; //set counter for num of transmission errors
    aXbeeAddressTable[i][0] = 0x0;
    aXbeeAddressTable[i][1] = 0x0;
    aNodeLastUpdate[i] = 1UL;
  }
  setLED(DATLed, ON);
  procStarted = false; //Disable data transmition on Serial
  delay(1000);
}

//---------------------------------------------------
// inizio sezione Xbee ------------------------------
//---------------------------------------------------
int getData()
//---------------------------------------------------
// Read available data on Xbee serial
//---------------------------------------------------
{
  int nStatus = 999;
  uint8_t option;            // Should return zero, not sure how to use this
  uint8_t dataLength;        // number of bytes of data being sent to xbee
  static int16_t RxData[RX_MAX_VAL + 1]; // Array to hold data received
  //  uint16_t Tx_Id;    //  s of transmitter (MY ID)
  bool responseFromReadPacket = xbee.readPacket(500);
  if (responseFromReadPacket)
  {
    int resXbee = getApiId();
    // got a Rx packet
    if (resXbee == RX_RESPONSE)
    {
      RXStatusResponse(rx);
      option = rx.getOption();
      dataLength = rx.getDataLength();
      RCV_ADDR = rx.getRemoteAddress64(); // Serial of Tx (remember MY is set as a hex number.  Useful if you have multiple transmitters
      signalStrength = getRssi();

      for (int i = 0; i < dataLength; i = i + 2)
      {
        RxData[i / 2]  = rx.getData(i) << 8;
        RxData[i / 2] |= rx.getData(i + 1);

        if (debug == 1) {
          Serial.print(RxData[i / 2]);
          Serial.print("-");
        }
      }

      if (debug == 1) {
        Serial.println("");
      }

      //Look for the node number from an Xbee address
      int node = getNodeIndex(getNodeByAddress(RCV_ADDR.getLsb(), RCV_ADDR.getMsb()));

      if (debug == 1) {
        Serial.print("Node: ");
        Serial.println(node);
        Serial.print("Node numberX: ");
        Serial.println(getNodeNumber(node));       
      }

      if (node != 99999) //if the node number has been found
      {     
        aNodeTable[node][3] = signalStrength;
        aNodeTable[node][4] = nodeStatusOk;
        aNodeLastUpdate[node] = millis();

        switch (RxData[0]) {

        case SENSOR: //update the Sensors Array with remote data
          for (int i = 1; i < dataLength / 2; i = i + NUM_DATA_VAL ) { // loop on received data
            for (int t = 0; t < NUMROWS; t++) { //loop su sensors array
              if (RxData[i] == aDataTable[t][1] and aDataTable[t][0] == getNodeNumber(node))
              {
                aDataTable[t][2] = RxData[i + 1];
                aDataTable[t][4] = RxData[i + 2];
                break;
              }
            }
          }
          break;
        case ACTUATOR: //update the Actuators Array with remote data
          for (int i = 1; i < dataLength / 2; i = i + NUM_DATA_VAL ) { // loop on received data
            for (int t = 0; t < NUMROWS; t++) { //loop su actuarors array
              if (RxData[i] == aActuTable[t][1] and aActuTable[t][0] == getNodeNumber(node))
              {            
                //updActuator(getNodeNumber(node), RxData[i], RxData[i + 2]);
                aActuTable[t][2] = RxData[i + 1]; //update Actuator status
                aActuTable[t][3] = RxData[i + 2]; //update Actuator value
                break;
              }
            }
          }
          break;
        case METHOD: //update the Methods Array with remote data
          for (int i = 1; i < dataLength / 2; i = i + NUM_DATA_VAL ) { // loop on received data
            for (int t = 0; t < NUMROWS; t++) { //loop su actuarors array
              if (RxData[i] == aMethTable[t][1] and RxData[i + 1] == aMethTable[t][2] and aMethTable[t][0] == getNodeNumber(node))
              {
                aMethTable[t][3] = RxData[i + 2]; //update Actuator value
                break;
              }
            }
          }
          break;
        }
        nStatus = receivedOK;
 
        if (debug == 1) {
          Serial.print(RxData[0]);
          Serial.print(" - RX - Response available - Frame id: ");
          Serial.print(resXbee, HEX);
          Serial.print(" - RX Status: ");
          Serial.print(getStatusTx(), HEX);
          Serial.print(" - Nodo: ");
          Serial.print(node, DEC);
          Serial.print(" - datalenght: ");
          Serial.print(dataLength, DEC);
          Serial.print(" ");
          Serial.print(RCV_ADDR.getMsb(), HEX);
          Serial.print(RCV_ADDR.getLsb(), HEX);
          Serial.print(" ");
          Serial.println(rx.getRemoteAddress16(), HEX);
        }

      } //End if the node is found
    }
    else // if the response in not RX_64_RESPONSE
    {
      nStatus = noResponse;
    }
  }
  else // no response available, then get the error
  { // Got something, but not a packet
    nStatus = xbee.getResponse().getErrorCode();
  }
  return nStatus;
}

//---------------------------------------------------
// inizio sezione Xbee ------------------------------
//---------------------------------------------------
int sendRemoteCommand(int node) // n=node
{
  unsigned long receiveResponseInitTime = millis();
  boolean readPacketResponse; //store the response of xbee.readPacket(timeout)
  //if the node is inactive returns immediately and not execute the command
  if (getNodeStatus(node) ==  nodeStatusNotOk) return nodeStatusNotOk;
  // if the node is active goes ahead

  int nStatus = 999; //set node status to no response
  int x;
  XBeeAddress64 addr64 = XBeeAddress64(aXbeeAddressTable[node][0], aXbeeAddressTable[node][1]);

  // break down integers into two bytes and build the payload
  // break down integers into n (NUM_BYTE_ARR) bytes and build the payload
  for (int i = 0; i < NUM_DATA_PTS; i++)
  {
    for (int k = 0; k < NUM_BYTE_ARR; k++)
    {
      if (k == 0) {
        payload[(i * NUM_BYTE_ARR) + (NUM_BYTE_ARR - 1)] = xbeeData[i] & 0xff;
      }
      else
      {
        payload[(i * NUM_BYTE_ARR) + (NUM_BYTE_ARR - 1 - k)] = xbeeData[i] >> 8 * k & 0xff;
      }
    }
  }
  /*
    for (int i = 0; i < NUM_DATA_PTS; i++)
    {
    payload[i * 2] = xbeeData[i] >> 8 & 0xff; // High byte - shift bits 8 places, 0xff masks off the upper 8 bits
    payload[(i * 2) + 1] = xbeeData[i] & 0xff;  // low byte, just mask off the upper 8 bits
    }

  */
  uint8_t frameid = 8;
  request(addr64, payload, sizeof(payload));
  setFrameId();
  uint16_t senderLongAddress;

  //  serialFlush(); //clean the xbee serial buffer before sending data
  xbee.send(tx);
  if (debug == 1) {
    Serial.print("trasmesso - ");
  }
  //        delay(10);
  receiveResponseInitTime = millis() + receiveResponseTimeout; // set the init time to count the timeout
  while (receiveResponseInitTime >= millis()) { // && readPacketResponse == false //);
    if (xbee.readPacket(timeoutXbeeResponse)) {
      if (getApiId() == TX_RESPONSE) { //  && getFrameIdTx() == getFrameIdRx()
        aNodeLastUpdate[getNodeIndex(node)] = millis();
        break;
      }
    }
  }
  //if (xbee.getResponse().isAvailable())
  //{
  if (debug == 1) {
    Serial.print(" response available ");
    Serial.println(getApiId(), HEX);
  }
  if (getApiId() == TX_RESPONSE)
  {
    TXStatusResponse(txStatus);
    senderLongAddress = txStatus.getRemoteAddress();
    // get the delivery status, 0 = OK, 1 = Error, 2 = Invalid Command, 3 = Invalid Parameter
    if (getStatusTx() == SUCCESS)
    {
      // success.  time to celebrate
      nStatus = getStatusTx();
      setErrorLed(node, OFF);
      setLED(ERRLed, OFF);
    }
    else if (getStatusTx() == ZB_TX_UNKNOWN)
    {
      setErrorLed(node, ON);
      setLED(ERRLed, ON);
      // the remote XBee did not receive our packet. is it powered on?
    }
    else if (getStatusTx() == NETWORK_ACK_FAILURE || getStatusTx() == ADDRESS_NOT_FOUND) //21 24
    {
      setErrorLed(node, ON);
      setLED(ERRLed, ON);
      //nStatus = "INACTIVE";// the remote XBee did not receive our packet. is it powered on?
    }
    else
    {
      setErrorLed(node, ON);
      setLED(ERRLed, ON);
      // the remote XBee did not receive our packet. is it powered on?
    }
    nStatus = getStatusTx();
  }
  else if (x == MODEM_STATUS_RESPONSE) {
    getModemStatusResponse();
    // the local XBee sends this response on certain events, like association/dissociation
    if (getStatusModem() == ASSOCIATED) {
      // yay this is great.  flash led
      nStatus = associateOK; // associated but no data transmitted
    } else if (getStatusModem() == DISASSOCIATED) {
      // this is awful.. flash led to show our discontent
      nStatus = associateNotOK;
    }
  }
  else if (getApiId() == 0) //0
  {
    nStatus = 999; //generic or unknown error
    setErrorLed(node, ON);
    setLED(ERRLed, ON);
  }

  if (debug == 1) {
    Serial.print("TX Node: ");
    Serial.print(node);
    Serial.print(" - Frame id: ");
    Serial.print(getApiId(), HEX);
    Serial.print(" - Remote address: ");
    Serial.print(senderLongAddress, HEX);
    Serial.print(" - TX Status: ");
    Serial.print(getStatusTx(), HEX);
    Serial.print(" - z: ");
    Serial.println(nStatus, HEX);
  }
  return nStatus; // return the transmition status 0=Ok 1=not responding
} // sendRemoteCommand()

//----------------------------------------------------------//
// Common functions                                         //
//----------------------------------------------------------//
boolean checkSerial() {
  boolean response;
  if (Serial1.available() > 0) response = true;
  else response = false;
  return response;
}
//----------------------------------------------------------//
int getNodeIndex(int node)
{
  int nodeIndex = 99999;
  for (int i = 0; i < NUMNODS; i++)
  {
    if (aNodeTable[i][0] == node)
    {
      nodeIndex = i;
      break;
    }
  }
  return nodeIndex;
}

//----------------------------------------------------------//
int getNodeNumber(int n)
{
  return aNodeTable[n][0];
}

//----------------------------------------------------------//
/* check and update nodes status */
/* if last update >= millis()+timeout */
void updateNodeStatus()
{
  for (int i = 0; i < NUMNODS; i++)
  {
    if (aNodeTable[i][1] != nLocal) {
      if (aNodeTable[i][0] != initString)
      {
        if (aNodeLastUpdate[i] >= (millis() - nodeTimeOut)) {
          //node active
          setNodeStatus(i, nodeStatusOk);
        }
        else //if false
        {
          //node inactive
          setNodeStatus(i, nodeStatusNotOk);
        }
      }
      else
      {
        break;
      }
    }
  }
}
//----------------------------------------------------------//
int getNodeStatus(int node) {
  return aNodeTable[getNodeIndex(node)][4];
}
//----------------------------------------------------------//
void updActuator(int node, int actuator, int value) {
  for (int i = 0; i < NUMROWS; i++) {
    if (aActuTable[i][0] == node && aActuTable[i][1] == actuator)
    {
      if (value = OFF) {
        aActuTable[i][2] = OFF;
      }
      else
      {
        aActuTable[i][2] = ON;
      }
      aActuTable[i][3] = value;
      break;
    }
  }
}
//----------------------------------------------------------//
void setStatusLed(int node, int sts) // es. (1,0)
{
  digitalWrite(22 + ((getNodeIndex(node) - 1) * 2), sts);
}

void setErrorLed(int node, int sts) // es. (1,0)
{
  digitalWrite(23 + ((getNodeIndex(node) - 1) * 2), sts);
}
//----------------------------------------------------------//
int getNodeType(int node)
{
  int nodeType;
  for (int i = 0; i < NUMNODS; i++)
  {
    //if node found
    if (aNodeTable[i][0] == node)
    {
      //get the type
      nodeType = aNodeTable[i][1];
      break;
    }
  }
  return nodeType;
}
//----------------------------------------------------------//
void setNodeStatus(int n, int sts) {

  if (sts == nodeStatusOk) {
    setLED(DATLed, ON);
    aNodeTable[n][4] = nodeStatusOk; //set the node status 1=ok 0=not responding
    setErrorLed(aNodeTable[n][0], OFF);
    setStatusLed(aNodeTable[n][0], ON);
    setLED(ERRLed, OFF);
  }
  else
  {
    aNodeTable[n][4] = nodeStatusNotOk; //setta lo status del nodo 1=ok 0=non raggiungibile
    //aNodeTable[c][(NUMCOLS+2)-1] = aNodeTable[c][(NUMCOLS+2)-1] + 1; //increase error counter
    aNodeTable[n][(NUMCOLS + 1) - 1] = sts; //set the last error code
    setErrorLed(aNodeTable[n][0], ON);
    setStatusLed(aNodeTable[n][0], OFF);
    setLED(ERRLed, ON);
  }
}
//----------------------------------------------------------//
void setLED(int pin, int sts)
{
  digitalWrite(pin, sts);
}
//----------------------------------------------------------//
uint32_t getNodeByAddress(uint32_t Lsb, uint32_t Msb)
{
  uint32_t node = 99999;
  for (int i = 0; i < NUMROWS; i++)
  { // loop su array con i dati ricevuti
    //if node found
    if (Lsb == aXbeeAddressTable[i][1] and Msb == aXbeeAddressTable[i][0])
    {
      node = i;
      break;
    }
  }
  return node;
}
//----------------------------------------------------------//
//------------------------
// read data from Serial
//------------------------
void readSerialData(int x)  {
  while (Serial.available() > 0) {
    for (int i = 0; i < x; i++) {
      //read all integer available on serial
      readInt[i] = Serial.parseInt();
    }
  }
}
//----------------------------------------------------------//
