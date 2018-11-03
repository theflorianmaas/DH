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
  //pBit();
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
  if (isDimmerStarted == true && pin == c_light) { //if dimmer started do not update the current light
    pBit(); //do nothing
  }
  else  {
  byte val = map(value, 0, 254, 0, 100);
  aLights[getPinIdx(pin)][2] = sts; //status
  aLights[getPinIdx(pin)][3] = val; //status
  aLights[getPinIdx(pin)][4] = convertColor(color); //status
  refreshScreen();
  }
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
  //pBit();
  refreshScreen();
}

void setLightGroup(uint8_t group) { 
  c_group = group; //current group
}

void pBit() {
  tone(PIN_TONE, 1500, 30);
  noTone;
}

void getScreenTouch() {
  uint8_t* touch = HMISerial.listen(); //check for message
  byte cmd;
  if (touch[0] != 0) {
     pBit();
     //Serial.println(touch[0]);
     //Serial.println(touch[1]);
     //Serial.println(touch[2]);
    if (touch[2] == NEX_RET_GROUP || touch[2] == NEX_RET_LIGHT1 || touch[2] == NEX_RET_LIGHT2 || touch[2] == NEX_RET_LIGHT3 || touch[2] == NEX_RET_LIGHT4 || touch[2] == NEX_RET_LIGHT5 || touch[2] == NEX_RET_LIGHT6)
    {
      switch (touch[2]) {
        case NEX_RET_GROUP:
          c_light = 0;
          break;
        case NEX_RET_LIGHT1:
          c_light = 1;
          break;
        case NEX_RET_LIGHT2:
          c_light = 2;
          break;
        case NEX_RET_LIGHT3:
          c_light = 3;
          break;
        case NEX_RET_LIGHT4:
          c_light = 4;
          break;
        case NEX_RET_LIGHT5:
          c_light = 5;
          break;
        case NEX_RET_LIGHT6:
          c_light = 6;
          break;
      }
    }
    else if (touch[2] == NEX_RET_STRDIM)
      isDimmerStarted = true;
    else if (touch[2] == NEX_SEL_GROUP) //selected default group
      setLightGroup(touch[3]);
    else if (touch[2] == NEX_CON_WIFI) //selected default group
      connectWIFI();
    else {
      if (touch[2] == NEX_RET_OFF) {
        isDimmerStarted = false;
      }
      sendCommand(touch[2]);
    }
  }
}
