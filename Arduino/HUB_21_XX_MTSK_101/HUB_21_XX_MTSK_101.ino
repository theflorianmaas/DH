/*
  DH HUB
  Platform: Arduino 101 MULTITASKING
  Purpose: Collect data from/to end nodes and link from/to the main database
           Receive data from end nodes
           Send commands to end nodes
           Send data to the main database (on Serial)

  Author: Franco Parenti

  Rel. 21 XX
  27/10/2019

  Before compiling:
  Set the Serial Buffer to 2048 in the Arduino IDE
  /home/user/.arduino15/packages/Intel/hardware/arc32/2.0.4/cores/arduino/RingBuffer.h

  Command Examples (on Serial)
  IX2,1,1286656,1085167784,2,1286656,1085167843
  IX1,1,1286656,1085167843
  IS10,1,18,0,0,0,1,19,0,0
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
  CD2,10,1,23,0 //Setup smartlight configuration (all device type)
  CE2,10,0,1,2,3,4,5,0,0,0,0 setup smartlight configuration group MOODs
  CC7,1,1,1,183 //smartlight command

*/

//------------------------------------------------------
//----------------- Declare functions ------------------
//------------------------------------------------------
void setLED();
void readSerialData(int x);
int getNodeByAddress(uint32_t Lsb, uint32_t Msb); //return node number
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
void processRx();
void resetTable(byte x);
void initTable(byte x, int cnt);
void initXbeeAddressTable(int cnt);

void sendDataOnSerial(int x);
void sendAllData();

int debug = 0;

#include <XBee.h>  //http://code.google.com/p/xbee-arduino/     Modified per http://arduino.cc/forum/index.php/topic,111354.0.html
#include "Timer.h"
//#include <Scheduler.h>
#include <QueueList.h>
//#include "Nextion.h"

//#define SERIAL_BUFFER_SIZE 4096
#define BAUD_RATE_XBEE 115200  // Baud for Xbee serial
#define BAUD_RATE 115200       // Baud for serial

#define SENSOR 0
#define ACTUATOR 1
#define METHOD 2
#define SMCMD 99

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
#define SMSETUP 68
#define SMGROUP 69


// define params status for Xbee transmission
#define receivedOK 1
#define sentOK     0
#define noResponse 0
#define dataError 2
#define xbeeError 3
#define nodeStatusOk 1
#define nodeStatusNotOk 0

// pins for the service LEDs
#define STSLed 11
#define DATLed 11
#define ERRLed 11
#define SERLed 11
#define xbeeLed 13

//Service Led status
#define ON HIGH
#define OFF LOW

// define node types
#define nLocal 1 //Node for local sensors
#define nXbee 2 //Xbee node

int TIMEt0 = 1000; //send data on serial every n millisec
Timer t0; //timer to schedule the update Sensor and Actuator data

const unsigned long nodeTimeOut = 5000UL; //Xbee nodes timeout. Returns error after this interval
const unsigned long receiveResponseTimeout = 2000L; //receive response after a command is sent
unsigned long startResponseWaitingTime;
int readInt[1024]; //variable to receive data from serial

//define array to store node,sensor,actuators data
#define NUMCOLS 5 //num column arrays. Don't change this value unless you know what are doing.
#define NUMROWS 64 //num rows arrays. Supports 72 Sensors and 72 Actuators. This value can be increased if needed
#define NUMNODS 16 //num rows arrays, Supports 64 Nodes. This value can be increased if needed

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

//array for smartlight received command
//int aSMCommand[5];

#define INITSTRING 0xFFFE
#define NODE_NOT_FOUND 9999

//Enable/Disable data transmition on serial. false=Disable
boolean procStarted = false;

//------------------------------------------------------
//--- XBee ---------------------------------------------
//------------------------------------------------------

#define TX_RESPONSE ZB_TX_STATUS_RESPONSE
#define RX_RESPONSE ZB_RX_RESPONSE
#define ZB_TX_UNKNOWN 0xBF
#define request(addr64, payload, sizeofPayload) ZBTxRequest tx = ZBTxRequest(addr64, payload, sizeofPayload)
#define getStatusTx() txStatus.getDeliveryStatus()
#define TXStatusResponse(txStatus)  xbee.getResponse().getZBTxStatusResponse(txStatus)
ZBTxStatusResponse txStatus = ZBTxStatusResponse();
#define setFrameId() tx.setFrameId(xbee.getNextFrameId())
ZBRxResponse rx = ZBRxResponse();
#define RXStatusResponse(rx) xbee.getResponse().getZBRxResponse(rx)
#define getApiId() xbee.getResponse().getApiId()
#define getFrameIdRx() txStatus.getFrameId()
FrameIdResponse frame = FrameIdResponse();
#define getFrameIdTx() frame.getFrameId()
#define getRssi() 100
ModemStatusResponse msr = ModemStatusResponse();
#define getModemStatusResponse() xbee.getResponse().getModemStatusResponse(msr)
#define getStatusModem() msr.getStatus()

//Xbee object instances
XBee xbee = XBee();
#define NUM_DATA_PTS  12 // Number of integers (data points) to upload. Can't exceed 100 bytes or 50 integers unless you change MAX_FRAME_DATA_SIZE in XBee.h (Library Xbee)
#define NUM_RCV_VAL   24 // Number of received Sensor values from a remote node
#define NUM_ACT_VAL   30 // Number of received Actuator values from a remote node
#define NUM_DATA_VAL   3 // numero valori per sensore
#define NUM_BYTE_ARR   3 // Number of bytes of the array used to store long integers in the payload
uint8_t payload[NUM_DATA_PTS * NUM_BYTE_ARR];
long xbeeData[NUM_DATA_PTS];  // Array to hold integers that will be sent to other xbee
//remote Xbee node address
XBeeAddress64 RCV_ADDR = XBeeAddress64(0xFF, 0xFF);
uint32_t aXbeeAddressTable[NUMNODS][2]; //0=High 1=Low
uint8_t signalStrength;
QueueList <String> qRXResponse; //queue to collect the RX responses
QueueList <uint8_t> qTXResponse; //queue to collect the TX responses
QueueList <int> qCommands; //queue to collect the commands to send out

char buffer[10] = {0};

#include <SoftwareSerial.h>

SoftwareSerial xbeeSerial(2, 3); // RX, TX


//------------------------------------------------------
//------------------------------------------------------

void setup() {

  // initialize serial:
  Serial.begin(BAUD_RATE); //serial
  //xbeeSerial.begin(57600);
  Serial1.begin(BAUD_RATE_XBEE); //xbee serial
  // xbee start serial
  xbee.setSerial(Serial1);

  // make the pins outputs:
  pinMode(STSLed, OUTPUT);
  pinMode(DATLed, OUTPUT);
  pinMode(ERRLed, OUTPUT);
  pinMode(SERLed, OUTPUT);
  pinMode(xbeeLed, OUTPUT);

/*
  for (int i = 20; i < 54; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
*/

  setLED(STSLed, OFF);
  setLED(DATLed, OFF);
  setLED(ERRLed, OFF);
  setLED(SERLed, OFF);
  setLED(xbeeLed, OFF);
  initialize();

  t0.every(TIMEt0, sendAllData); //set time interval to read data from remote nodes millis
  //t0.every(500, sendRemoteCommand); //set time interval to read data from remote nodes millis
  //t0.every(300, getXbeeData); //set time interval to read data from remote nodes millis
  //t0.every(300, readXbeeData); //set time interval to read data from remote nodes millis
  //t0.every(500, updateNodeStatus); //set time interval to read data from remote nodes millis


  //  Scheduler.startLoop(loop4); //start loop 3. Update status of remote nodes
  //  Scheduler.startLoop(loop3); //start loop 3. Apply data from xbee nodes
  //  Scheduler.startLoop(loop2); //start loop 2. Read data and responses from Xbee nodes
  //  Scheduler.startLoop(loop1); //start loop 1. Execute commands

}

// ---------- Main loop ------------- //
// ---------- Main loop ------------- //
// ---------- Main loop ------------- //
void loop() {
  getSerialData(); 
  if (procStarted) {
    t0.update();
    loop1();
    loop2();
    loop3();
    loop4();
  }

  setLED(xbeeLed, OFF);
  //delay(20);
}

void loop1() {
  sendRemoteCommand();
  //setLED(SERLed, OFF);
  //delay(20);
}

void loop2() {
  getXbeeData();
  //setLED(SERLed, OFF);
  //delay(20);
}

void loop3() {
  readXbeeData();
  //delay(20);
}

void loop4() {
  updateNodeStatus();
  //delay(20);
}

// -------- End Mail loop ----------- //
// -------- End Mail loop ----------- //
// -------- End Mail loop ----------- //

//-------------------------------------------------
// Functions
//-------------------------------------------------
//---------------------------------------------------
// inizio sezione Xbee ------------------------------
//---------------------------------------------------
void getXbeeData()
//---------------------------------------------------
// Read all available data on Xbee serial and store in queues
//---------------------------------------------------
{
  int nStatus = 999;
  uint8_t option;            // Should return zero, not sure how to use this
  uint8_t dataLength;        // number of bytes of data being sent to xbee

  int node = 0;
  //  uint16_t Tx_Id;    //  s of transmitter (MY ID)
  bool responseFromReadPacket = xbee.readPacket(100);
  RXStatusResponse(rx);
  option = rx.getOption();
  dataLength = rx.getDataLength();
  RCV_ADDR = rx.getRemoteAddress64();
  signalStrength = getRssi();

  if (responseFromReadPacket)
  {
    int resXbee = getApiId();
    // got a Rx packet
    if (resXbee == RX_RESPONSE)
    {

      if (debug == 1) {
        setLED(xbeeLed, ON);
        Serial.print("RX response available ");
        Serial.println(getApiId(), HEX);
      }
      int16_t RxData[(dataLength / 2) + 1];
      nStatus = receivedOK;
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
        Serial.println("Fine loop");
      }

      //Look for the node number from an Xbee address
      int nodeid = getNodeByAddress(RCV_ADDR.getLsb(), RCV_ADDR.getMsb());
      node = getNodeIndex(nodeid);
     
      if (debug == 1) {
        Serial.print("NodeX: ");
        Serial.println(nodeid);
        Serial.println(node);
        Serial.println(RCV_ADDR.getLsb(), DEC);
        Serial.print(RCV_ADDR.getMsb(), DEC);
        Serial.print("Node numberX: ");
        Serial.println(getNodeNumber(node));
      }
     
      if (node != NODE_NOT_FOUND) //if the node number has been found
      {
        aNodeTable[node][3] = signalStrength;
        aNodeLastUpdate[node] = millis();
        String str = createString(node, RxData, sizeof(RxData) / 2);
        qRXResponse.push(str);
      }
      
    }
    else if (resXbee == TX_RESPONSE)// if the response in not RX_64_RESPONSE
    {
      
      if (debug == 1) {
        Serial.print("TX response available ");
        Serial.println(getApiId(), HEX);
      }
      xbee.getResponse().getZBTxStatusResponse(txStatus);
      //TXStatusResponse(txStatus); //dà errore verificare
      // get the delivery status, 0 = OK, 1 = Error, 2 = Invalid Command, 3 = Invalid Parameter
      if (getStatusTx() == SUCCESS)
      {
        if (!qTXResponse.isEmpty()) {
          qTXResponse.pop();
          Serial.println("CX1");
        }
      }
      else
      {
        //an invalid packet is received
        setErrorLed(node, ON);
        setLED(ERRLed, ON);
      }
      //check if there is an expired response
      if (startResponseWaitingTime + receiveResponseTimeout < millis()) { //if response timeout expired
        if (!qTXResponse.isEmpty()) {
          qTXResponse.pop();
          Serial.println("CX0");
        }
      }    
    }
    else
    {
      // no valid packet recieved
      setLED(ERRLed, ON);
    }

  }

}

//---------------------------------------------------
void readXbeeData()  {

  while (!qRXResponse.isEmpty())
  {
    String x = qRXResponse.pop();
    int valueCnt = parseIntRxCount(x);
    int16_t aRxData[valueCnt];
    parseIntRx(x, aRxData);
    int node = aRxData[0];
    switch (aRxData[1]) {
      case SENSOR: //update the Sensors Array with remote data
        for (int i = 2; i < valueCnt; i = i + NUM_DATA_VAL ) { // loop on received data
          for (int t = 0; t < NUMROWS; t++) { //loop su sensors array
            if (aRxData[i] == aDataTable[t][1] and aDataTable[t][0] == getNodeNumber(node))
            {
              aDataTable[t][2] = aRxData[i + 1];
              aDataTable[t][4] = aRxData[i + 2];
              break;
            }
          }
        }
        break;
      case ACTUATOR: //update the Actuators Array with remote data
        for (int i = 2; i < valueCnt; i = i + NUM_DATA_VAL ) { // loop on received data
          for (int t = 0; t < NUMROWS; t++) { //loop su actuarors array
            if (aRxData[i] == aActuTable[t][1] and aActuTable[t][0] == getNodeNumber(node))
            {
              //updActuator(getNodeNumber(node), RxData[i], RxData[i + 2]);
              aActuTable[t][2] = aRxData[i + 1]; //update Actuator status
              aActuTable[t][3] = aRxData[i + 2]; //update Actuator value
              break;
            }
          }
        }
        break;
      case METHOD: //update the Methods Array with remote data
        for (int i = 2; i < valueCnt; i = i + NUM_DATA_VAL ) { // loop on received data
          for (int t = 0; t < NUMROWS; t++) { //loop su actuarors array
            if (aRxData[i] == aMethTable[t][1] and aRxData[i + 1] == aMethTable[t][2] and aMethTable[t][0] == getNodeNumber(node))
            {
              aMethTable[t][3] = aRxData[i + 2]; //update Actuator value
              break;
            }
          }
        }
        break;
        /*
      case SMCMD: //smartlight command
        aSMCommand[0] = getNodeNumber(node);
        aSMCommand[1] = aRxData[2]; //pin
        aSMCommand[2] = aRxData[3]; //status
        aSMCommand[3] = aRxData[4]; //value
        aSMCommand[4] = aRxData[5]; //color/mood
        sendCommandOnSerial();
        break;
        */
    }
  } //End while
}

//---------------------------------------------------
// inizio sezione Xbee ------------------------------
//---------------------------------------------------
void sendRemoteCommand() // n=node
{
  if (!qCommands.isEmpty()) { //if there is a command to send
    int node = qCommands.pop(); //get the node id
    //boolean readPacketResponse; //store the response of xbee.readPacket(timeout)
    //if the node is inactive returns immediately and not execute the command
    if (getNodeStatus(node) ==  nodeStatusOk) { //if the node is active execute the command

      XBeeAddress64 addr64 = XBeeAddress64(aXbeeAddressTable[node][0], aXbeeAddressTable[node][1]); //node Xbee Address

      // Create the payload
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
      request(addr64, payload, sizeof(payload));
      xbee.send(tx); //send the command
      startResponseWaitingTime = millis();
      qTXResponse.push(node);
    }
  }
} // sendRemoteCommand()
