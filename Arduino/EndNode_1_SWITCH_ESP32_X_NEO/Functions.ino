<<<<<<< HEAD
void refreshScreen() {
  String setValue;

  for (int i = 0; i < NUM_LIGHTS; i++) {
    //I don't use the library as this is faster
    //[x][0]=id [x][1]=status [x][2]=value [x][3]=color
    setValue = "vt" + String(i + 1) + ".val=" + aLights[i][2].toInt(); //type
    sendCommand(setValue.c_str());
    setValue = "st" + String(i + 1) + ".val=" + int(aLightsStatus[i][1]); //status
    sendCommand(setValue.c_str());
    setValue = "vl" + String(i + 1) + ".val=" + int(aLightsStatus[i][2]); //value
    sendCommand(setValue.c_str());
    setValue = "cx" + String(i + 1) + ".val=" + int(aLightsStatus[i][3]); //color
  }

  t_icons.enable();
  t_select_type.enable();
  //pBit();
}

void clearLightsArrays(int type) {
  if (type == 0) {
    for (int i = 1; i < NUM_LIGHTS; i++)
    {
      aLights[i][0] = "";
      aLights[i][1] = "";
      aLights[i][2] = "";
      aLights[i][3] = "";
    }
  }
  else if (type == 1) {
    for (int i = 1; i < NUM_LIGHTS; i++)
    {
      aLightsStatus[i][0] = NOLIGHT;
      aLightsStatus[i][1] = 999;
      aLightsStatus[i][2] = 0;
      aLightsStatus[i][3] = 0;
    }
  }
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
    // aLights[getPinIdx(pin)][2] = sts; //status
    //aLights[getPinIdx(pin)][3] = val; //status
    //aLights[getPinIdx(pin)][4] = convertColor(color); //status
    refreshScreen();
  }
}

/*
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
*/

void setLightGroup(uint8_t group) {
  c_group = group; //current group
}

void pBit() {
  tone(PIN_TONE, 1500, 30);
  noTone;
}

void pTic() {
  tone(PIN_TONE, NOTE_C3, 30);
  noTone;
}


String createUrl(char* ip, char* key, String group, String light, String cmd, int value) {
  String url = "/dh/runScript.php?";
  url = url + "ip=";
  url = url + ip;
  url = url + "&key=";
  url = url + key;
  url = url + "&group=";
  url = url + group;
  url = url + "&light=";
  url = url + light;
  url = url + "&command=";
  url = url + cmd;
  url = url + "&value=";
  url = url + value;
  Serial.print(url);
  return url;
}

String execUrl(String url) {
  // This will send the request to the server

  if (!http.connect(host, httpPort)) {
    Serial.println("connection failed");
  }
  String line;
  http.print(String("GET ") + url + " HTTP/1.1\r\n" +
             "Host: " + host + "\r\n" +
             "Connection: keep-alive\r\n\r\n");
  unsigned long timeout = millis();
  while (http.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      http.stop();
      return line;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (http.available()) {
    line = http.readStringUntil('\r');
  }
  return line;
}


void sendCommand(const char* cmd) {
  while (nextionSerial.available()) {
    nextionSerial.read();
  }//end while
  Serial.println(cmd);
  nextionSerial.print(cmd);
  nextionSerial.write(0xFF);
  nextionSerial.write(0xFF);
  nextionSerial.write(0xFF);
}//end sendCommand

void setText(String page, String obj, String val) {
  String cmd = page + "." + obj + ".txt=" + String('"') + val + String('"');
  // Serial.println(cmd.c_str());
  sendCommand(cmd.c_str());
}//setText


void setGroupList(int idx) {

  for (int i = 0; i < idx; i++) {
    String cmd;
    cmd = "vis t" + String(i) + ",1";
    sendCommand(cmd.c_str());
    cmd = "vis g" + String(i) + ",1";
    sendCommand(cmd.c_str());
    cmd = "t" + String(i) + ".txt=" + String('"') + aGroups[i][1] + String('"');
    Serial.println(aGroups[i][1]);
    Serial.println(cmd);
    sendCommand(cmd.c_str());
    if (i == c_group){
      cmd = "g" + String(i) + ".val=1";
      sendCommand(cmd.c_str());
    }
  }

}//setGroupList

void setLightList(int idx) {

  pgMain.show();
  /*
    for (int i = 0; i < idx; i++) {
     String cmd;
     cmd = "vis t" + String(i) + ",1";
     sendCommand(cmd.c_str());
     cmd = "vis g" + String(i) + ",1";
     sendCommand(cmd.c_str());
     cmd = "t" + String(i) + ".txt=" + String('"') + aGroups[i][1] + String('"');
     Serial.println(aLights[i][1]);
     Serial.println(cmd);
     sendCommand(cmd.c_str());
    }
  */
}//setGroupList


void putGroupID() {
  EEPROM.put(180, c_group_id);
  EEPROM.commit();
  //setText("Tradfri", "t_ip", String(tradfriParams_ip));
}

int getGroupID() {
  EEPROM.get(180, c_group_id);
  //setText("Tradfri", "t_key", String(tradfriParams_key));
}
=======
void refreshScreen() {
  String setValue;
  for (int i = 1; i < NUM_LIGHTS; i++) {
    //I don't use the library as this is faster
    //[x][0]=pin [x][1]=type [x][2]=status [x][3]=value [x][4]=color
    setValue = "vt" + String(i) + ".val=" + int(aLights[i][1]); //type
    sendCommand(setValue.c_str());
    setValue = "st" + String(i) + ".val=" + int(aLights[i][2]); //status
    sendCommand(setValue.c_str());
    setValue = "vl" + String(i) + ".val=" + int(aLights[i][3]); //value
    sendCommand(setValue.c_str());
    setValue = "cx" + String(i) + ".val=" + int(aLights[i][4]); //color
  }
  t_icons.enable();
  t_select_type.enable();
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

void pTic() {
  tone(PIN_TONE, NOTE_C3, 30);
  noTone;
}


String createUrl(char* ip, char* key, int group, int light, String cmd, int value) {
  String url = "/dh/runScript.php?";
  url = url + "ip=";
  url = url + ip;
  url = url + "&key=";
  url = url + key;
  url = url + "&group=";
  url = url + group;
  url = url + "&light=";
  url = url + light;
  url = url + "&command=";
  url = url + cmd;
  url = url + "&value=";
  url = url + value;
  Serial.print(url);
  return url;
}

String execUrl(String url) {
  // This will send the request to the server
  String line;
  http.print(String("GET ") + url + " HTTP/1.1\r\n" +
             "Host: " + host + "\r\n" +
             "Connection: keep-alive\r\n\r\n");
  unsigned long timeout = millis();
  while (http.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      //client.stop();
      return line;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (http.available()) {
    line = http.readStringUntil('\r');
  }
  return line;
}


void sendCommand(const char* cmd) {
  while (nextionSerial.available()) {
    nextionSerial.read();
  }//end while
  Serial.println(cmd);
  nextionSerial.print(cmd);
  nextionSerial.write(0xFF);
  nextionSerial.write(0xFF);
  nextionSerial.write(0xFF);
}//end sendCommand

void setText(String page, String obj, String val) {

  String cmd = page + "." + obj + ".txt=" + String('"') + val + String('"');
  Serial.println(cmd.c_str());
  sendCommand(cmd.c_str());

}//setText


void setGroupList() {

  Serial.println(sizeof(aGroups));
  int numRows = sizeof(aGroups) / 88;
  
  for (int i = 0; i < numRows; i++) {
    String cmd = "Config_Lights.t" + String(i) + ".txt=" + String('"') + aGroups[i][1] + String('"');
    Serial.println(cmd.c_str());
    sendCommand(cmd.c_str());
    cmd = "vis Config_Lights.t" + String(i) + ",1";
    sendCommand(cmd.c_str());
    cmd = "vis Config_Lights.g" + String(i) + ",1";
    sendCommand(cmd.c_str());
  }

}//setGroupList
>>>>>>> 14d8ccc29a2b6d9ebeb959ed77e2d72156646afd
