void initScreen() {
  //  HMISerial.setComponentValue("gStatus", g_sts);
  //  HMISerial.setComponentValue("gMode", g_mode);
  //  HMISerial.setComponentValue("gProg", g_prog);
  //  HMISerial.setComponentValue("gLink", g_link);
  //  HMISerial.sendCommand("vis pLink,0");
}

void refreshScreen() {

  boolean outout;
  for (int c = 0; c < 5; c++) {
    HMISerial.setComponentValue("st1", 0);
    Serial.println(outout);
  }
  String label;

  for (int i = 1; i < NUM_LIGHTS; i++) {
    HMISerial.setComponentValue("vt" + String(i), int(aLights[i][1]));
    HMISerial.setComponentValue("st" + String(i), int(aLights[i][2]));
    HMISerial.setComponentValue("vl" + String(i), int(aLights[i][3]));
    HMISerial.setComponentValue("cx" + String(i), int(aLights[i][4]));
  }
  HMISerial.sendCommand("t_icons.en=1");
  HMISerial.sendCommand("t_select_type.en=1");
  pBit();
}

byte convertColor(byte color) {
  byte ret = 1;
  switch (color) {
    case 1:
      ret = 1;
      break;
    case 2:
      ret = 2;
      break;
    case 3:
      ret = 3;
      break;
    case 19:
      ret = 2;
      break;
    case 7:
      ret = 3;
      break;
    case 11:
      ret = 4;
      break;
    case 16:
      ret = 5;
      break;
    case 18:
      ret = 6;
      break;
  }
  return ret;

}


void execLightCommand(byte pin, byte sts, int value, int color) {
  byte val = map(value, 0, 254, 0, 100);
  aLights[getPinIdx(pin)][2] = sts; //status
  aLights[getPinIdx(pin)][3] = val; //status
  aLights[getPinIdx(pin)][4] = convertColor(color); //status
  refreshScreen();
}

void setLightConfig(byte pin, byte type, byte mode) {
  //mode 0=insert 2=delete 1=update
  byte idx;
  switch (mode) {
    case 0: //insert
      if (getPinIdx(pin) == -1) {//if the pin not exist
        idx = getFreePinIdx(); //search for the first vacant light
        if (idx != -1) {
          aLights[idx][0] = pin; //pin
          aLights[idx][1] = type; //type
          aLights[idx][2] = OFF; //status
          aLights[idx][3] = 0; //value
          aLights[idx][4] = 1; //color
          //[x][0]=pin [x][1]=type [x][2]=status [x][3]=value [x][4]=color
        }
      }
      else {//if exists update
        idx = getPinIdx(pin); //search for the light to update
        aLights[idx][1] = type; //type
        aLights[idx][2] = OFF; //status
        aLights[idx][3] = 0; //value
        aLights[idx][4] = 1; //color
      }
      break;
    case 1: //delete
      idx = getPinIdx(pin); //search for the light to delete
      if (idx != -1) {
        aLights[idx][0] = NOLIGHT; //pin
        aLights[idx][2] = 999; //status = not visible
      }
      break;
    case 2: //update
      idx = getPinIdx(pin); //search for the light to update
      if (idx != -1) {
        aLights[idx][1] = type; //type
        aLights[idx][2] = OFF; //status
        aLights[idx][3] = 0; //value
        aLights[idx][4] = 1; //color
      }
      break;
  }
  pBit();
  refreshScreen();
}

void setLightGroup(long int RxData[1]) {
  aGroups[0][1] = RxData[2]; //mood1
  aGroups[0][2] = RxData[3]; //mood2
  aGroups[0][3] = RxData[4]; //mood3
  aGroups[0][4] = RxData[5]; //mood4
  aGroups[0][5] = RxData[6]; //mood5
  aGroups[0][6] = RxData[7]; //mood6
  aGroups[0][7] = RxData[8]; //mood7
  aGroups[0][8] = RxData[9]; //mood8
  aGroups[0][9] = RxData[10]; //mood9
}

void pBit() {
  //byte pic;
  //if (g_bit == false) {
  //  pic = 55;
  //  g_bit = true;
  //}
  //else if (g_bit == true) {
  //  pic = 54;
  //  g_bit = false;
  // }
  //HMISerial.setComponentPic("pBit", pic);
  tone(9, 1500, 30);
  noTone;
  //delay(10);
}

void getScreenTouch() {

  uint8_t* touch = HMISerial.listen(); //check for message
  if (touch[0] != 0) {
    //Serial.print(touch[0],HEX);
    //Serial.print("-");
    //Serial.println(touch[1],HEX);
    //Serial.println(touch[2],HEX);
    //if (touch[0] == NEX_RET_EVENT_TOUCH_HEAD) { // if a message is received...
    pBit();
  }
}

boolean isTouch(uint8_t* touch, uint8_t code) {

  boolean res = false;
  if (touch[0] == code || touch[1] == code || touch[2] == code || touch[3] == code || touch[4] == code || touch[5] == code) {
    res = true;
  }
  return res;
}

