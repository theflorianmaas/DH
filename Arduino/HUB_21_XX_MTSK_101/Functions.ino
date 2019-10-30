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
int getNodeIndex(int nodeid)
{
  int nodeIndex;
  if (nodeid == NODE_NOT_FOUND) {
    nodeIndex = NODE_NOT_FOUND;
  }
  else
  {
    for (int i = 0; i < NUMNODS; i++)
    {
      if (aNodeTable[i][0] == nodeid)
      {
        nodeIndex = i;
        break;
      }
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
      if (aNodeTable[i][0] != INITSTRING)
      {
        if (aNodeLastUpdate[i] + nodeTimeOut >= millis()) {
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
      if (value == OFF) {
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
  digitalWrite(3 + ((getNodeIndex(node) - 1) * 2), sts);
}

void setErrorLed(int node, int sts) // es. (1,0)
{
  digitalWrite(4 + ((getNodeIndex(node) - 1) * 2), sts);
}
//----------------------------------------------------------//
int getNodeType(int node)
{
  int nodeType = 9999;
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
    //aNodeLastUpdate[n] = millis();
    setErrorLed(aNodeTable[n][0], OFF);
    setStatusLed(aNodeTable[n][0], ON);
    setLED(ERRLed, OFF);
  }
  else
  {
    aNodeTable[n][4] = nodeStatusNotOk; //setta lo status del nodo 1=ok 0=non raggiungibile
    aNodeTable[n][6] = aNodeTable[n][6] + 1; //increase error counter
    aNodeTable[n][5] = sts; //set the last error code
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
int getNodeByAddress(uint32_t Lsb, uint32_t Msb)
{
  int node = NODE_NOT_FOUND;
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
  for (int i = 0; i < x; i++) {
    //read all integer available on serial
    readInt[i] = Serial.parseInt();
  }
}
//----------------------------------------------------------//

/*
void sendCommandOnSerial() {
  String stringToSend;
  //-----------------------------------------------------
  //-----------------------------------------------------
  stringToSend.concat("99,"); //prefix for sm commands
  for (int y = 0; y < 5; y++) {
    stringToSend.concat(String(aSMCommand[y]));
    stringToSend.concat(",");
  }
  Serial.print(stringToSend);
  Serial.println("\r\n");
}// sendCommandOnSerial
*/

String createString(int node, int16_t aRxData[], int a_size) {
  String separator = ",";
  String res = String(node);
  for (int i = 0; i < a_size; i++)
  {
    res = res + separator;
    res = res + String(aRxData[i]);
  }
  return res;
}

int parseIntRxCount(String data) {
  String separator = ",";
  int index;
  index = 0;
  int inc = data.length() - 1;
  for (int i = 0; i < inc; i++) //count the number of occurences
  { //Walk through the text one letter at a time
    if (String(data[i]) == separator)
      index++;
  }
  return index + 1;
}

void parseIntRx(String data, int16_t aRxData[]) {
  String dataPart;      //variable to hole the return text
  String separator = ",";
  int index;
  index = 0;
  int idx = 0;
  int idxNext = 0;
  int cnt = 0;

  idx = 0;
  idxNext = -1;
  cnt = 0;
  int inc = data.length() - 1;
  for (int i = 0; i < inc; i++)
  { //Walk through the text one letter at a time
    idx = idxNext + 1;
    idxNext = data.indexOf(separator, idx);
    if (idxNext != -1)  {
      dataPart = data.substring(idx, idxNext);
      aRxData[cnt] = dataPart.toInt();
      cnt++;
    }
    else {
      dataPart = data.substring(idx, data.length());
      aRxData[cnt] = dataPart.toInt();
      break;
    }
  }
}

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
        aDataTable[i][0] = INITSTRING;
        for (int y = 1; y < NUMCOLS; y++) {
          aDataTable[i][y] = 0;
        }
      }
      break;
    case NODES:
      for (int i = 0; i < NUMNODS; i++) {
        aNodeTable[i][0] = INITSTRING;
        for (int y = 1; y < NUMCOLS; y++) {
          aNodeTable[i][y] = 0;
        }
      }
      break;
    case  ACTUATORS:
      for (int i = 0; i < NUMROWS; i++) {
        aActuTable[i][0] = INITSTRING;
        for (int y = 1; y < NUMCOLS; y++) {
          aActuTable[i][y] = 0;
        }
      }
      break;
    case METHODS:
      for (int i = 0; i < NUMROWS; i++) {
        aMethTable[i][0] = INITSTRING;
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
    aDataTable[i][0] = INITSTRING;
    aActuTable[i][0] = INITSTRING;
    aMethTable[i][0] = INITSTRING;
  }
  for (int i = 0; i < NUMNODS; i++) {
    aNodeTable[i][0] = INITSTRING;
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
//---------------------------------------------------
//-----------------------------------------------
//send all sensor data in aDataTable array if x=0
//send all node data in aNodeTable array if x=1
//send all actuators data in aActuTable array if x=3
//send all methods data in aMethTable array if x=9
//send all data if x=5
//-----------------------------------------------
void sendDataOnSerial(int x) {
  String stringToSend;
  if (x == 0) {
    //Serial.print("DS,");
    for (int i = 0; i < NUMROWS; i++)
    {
      if (aDataTable[i][0] != INITSTRING)
      {
        for (int y = 0; y < NUMCOLS; y++) {
          Serial.print(aDataTable[i][y], DEC);
          Serial.print(",");
        }
      }
      else
      {
        Serial.print("\r\n");
        break;
      }
    }
  }
  if (x == 1) {
    //Serial.print("DN,");
    for (int i = 0; i < NUMNODS; i++)
    {
      if (aNodeTable[i][0] != INITSTRING)
      {
        for (int y = 0; y < NUMCOLS; y++) {
          Serial.print(aNodeTable[i][y], DEC);
          Serial.print(",");
        }
      }
      else
      {
        Serial.print("\r\n");
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
      if (aActuTable[i][0] != INITSTRING)
      {
        for (int y = 0; y < NUMCOLS; y++) {
          Serial.print(aActuTable[i][y], DEC);
          Serial.print(",");
        }
      }
      else
      {
        Serial.print("\r\n");
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
      if (aNodeTable[i][0] != INITSTRING)
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
      if (aDataTable[i][0] != INITSTRING)
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
      if (aActuTable[i][0] != INITSTRING)
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
      if (aMethTable[i][0] != INITSTRING)
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
    Serial.println(stringToSend);
    //Serial.println("\r\n");
  }  //end IF = 5
}// sendDataOnSerial

//-------------------------------------------------
// Functions
//-------------------------------------------------
void getSerialData() {
  byte inChar; //store received bytes fron serial
  int intNum = 0;
  int nodeTemp;
  int nodeType = 0;

  setLED(DATLed, OFF);
  // if there's any serial available, read it:
  if (Serial.available() > 0) {
    setLED(13, ON);
    delay(10);
    inChar = Serial.read();  //read the first byte from the serial buffer
    //------------------------------------------------
    switch (inChar) {
      case DEBUG:  // X debug
        for (int i = 0; i < 5; i++) {
          if (aNodeTable[i][0] != INITSTRING) {
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
        // IT,500
        else if (inChar == TUNING) {  // T tuning Delay Variables
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
          nodeType = getNodeType(nodeTemp);
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
        }
        else if (inChar == COMMAND) {  // smartlight Command
          /*
            10,1,1,123,9
          */
          nodeTemp = Serial.parseInt(); //node
          xbeeData[0] = 7; //smartlight command
          xbeeData[1] = Serial.parseInt(); //pin
          xbeeData[2] = Serial.parseInt(); //status
          xbeeData[3] = Serial.parseInt(); //value
          xbeeData[4] = Serial.parseInt(); //color
          xbeeData[5] = 0; //not used
          xbeeData[6] = 0; //not used
          xbeeData[7] = 0; //not used
          xbeeData[8] = 0; //not used
          xbeeData[9] = 0; //not used
          xbeeData[10] = 0; //not used
          xbeeData[11] = 0; //not used
          nodeType = getNodeType(nodeTemp);
        }
        else if (inChar == SMSETUP) {  // D Setup smartlight configuration (all device type)
          /*
            10,1,23 Setup smartlight configuration (all device type)
          */
          nodeTemp = Serial.parseInt(); //node
          xbeeData[0] = 8; //Setup smartlight configuration (all device type)
          xbeeData[1] = Serial.parseInt(); //pin
          xbeeData[2] = Serial.parseInt(); //type
          xbeeData[3] = Serial.parseInt(); //mode
          xbeeData[4] = 0; //not used
          xbeeData[5] = 0; //not used
          xbeeData[6] = 0; //not used
          xbeeData[7] = 0; //not used
          xbeeData[8] = 0; //not used
          xbeeData[9] = 0; //not used
          xbeeData[10] = 0; //not used
          xbeeData[11] = 0; //not used
          nodeType = getNodeType(nodeTemp);
        }
        else if (inChar == SMGROUP) {  // E setup smartlight configuration group MOODs
          /*
            10,0,1,2,3,4,5,6,7,8,9,10 setup smartlight configuration group MOODs
          */
          nodeTemp = Serial.parseInt(); //node
          xbeeData[0] = 9; //setup smartlight configuration group MOODs
          xbeeData[1] = Serial.parseInt(); //pin
          xbeeData[2] = Serial.parseInt(); //mood1
          xbeeData[3] = Serial.parseInt(); //mood2
          xbeeData[4] = Serial.parseInt(); //mood3
          xbeeData[5] = Serial.parseInt(); //mood4
          xbeeData[6] = Serial.parseInt(); //mood5
          xbeeData[7] = Serial.parseInt(); //mood6
          xbeeData[8] = Serial.parseInt(); //mood7
          xbeeData[9] = Serial.parseInt(); //mood8
          xbeeData[10] = Serial.parseInt(); //mood9
          xbeeData[11] = Serial.parseInt(); //mood10
          nodeType = getNodeType(nodeTemp);
        }
        //COMMON SECTION TO EXECUTE THE COMMAND
        if (nodeType == nXbee) {   // If it is a Xbee node
          qCommands.push(nodeTemp);
        }
        else if (nodeType == nLocal) //if it the local node (for sensors and actuator directly connected to th HUB)
        {
          //insert code here
          Serial.println("CX1");  //send on serial to confirm the command is sent
        }
        // insert here other node types

        setLED(SERLed, OFF);
        //END COMMON SECTION
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
