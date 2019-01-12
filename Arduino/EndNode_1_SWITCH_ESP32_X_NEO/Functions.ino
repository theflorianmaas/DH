void refreshScreen() {

  String setValue;
  Serial.println("refrrsh group");
  setValue = "stg0.val=" + String(aLights[0].status); //status
  sendCommand(setValue.c_str());
  setValue = "vlg0.val=" + String(aLights[0].value); //value
  sendCommand(setValue.c_str());
  Serial.println("refrrsh light");

  for (int i = 1; i < NUM_LIGHTS; i++) {
    //I don't use the library as this is faster
    //[x][0]=id [x][1]=status [x][2]=value [x][3]=color
    setValue = "vt" + String(i) + ".val=" + aLights[i].type.toInt(); //type
    sendCommand(setValue.c_str());
    setValue = "st" + String(i) + ".val=" + int(aLights[i].status); //status
    sendCommand(setValue.c_str());
    setValue = "vl" + String(i) + ".val=" + int(aLights[i].value); //value
    sendCommand(setValue.c_str());
    setValue = "cx" + String(i) + ".val=" + int(aLights[i].color); //color
  }
  t_icons.enable();
  t_select_type.enable();
}

void clearLightsArrays() {
  for (int i = 1; i < NUM_LIGHTS; i++)
  {
    aLights[i].id = "";
    aLights[i].name = "";
    aLights[i].type = "";
    aLights[i].status = NOLIGHT;
    aLights[i].value = 0;
    aLights[i].color = 0;
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
  Serial.println(url);
  return url;
}

String execUrl(String url, unsigned long timeout) {
  // This will send the request to the server
  if (!http.connect(host, httpPort)) {
    Serial.println("connection failed");
  }
  String line;
  http.print(String("GET ") + url + " HTTP/1.1\r\n" +
             "Host: " + host + "\r\n" +
             "Connection: keep-alive\r\n\r\n");

  if (timeout == 0) { // no timeout return immediately
    return "end";
  }
  else { //timeout, wait for the response

    unsigned long iniTime = millis();
    while (http.available() == 0) {
      if (millis() - iniTime > timeout) {
        Serial.println(">>> Client Timeout !");
        http.stop();
        return line;
      }
    }
    // Read all the lines of the reply from server and print them to Serial
    while (http.available()) {
      line = http.readStringUntil('#');
    }
    //http.stop();
    String linex = line.substring(line.indexOf("§") + 2);
    return linex;
  }
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
    cmd = "vis gx" + String(i) + ",1";
    sendCommand(cmd.c_str());
    cmd = "t" + String(i) + ".txt=" + String('"') + aGroups[i][1] + String('"');
    Serial.println(aGroups[i][1]);
    Serial.println(cmd);
    sendCommand(cmd.c_str());
    if (i == c_group) {
      cmd = "gx" + String(i) + ".val=1";
      sendCommand(cmd.c_str());
    }
  }
}//setGroupList

void putGroupID() {
  EEPROM.put(180, c_group_id);
  EEPROM.commit();
  Serial.println(c_group_id);
  aLights[0].id = String(c_group_id);
}

int getGroupID() {
  EEPROM.get(180, c_group_id);
  Serial.println(c_group_id);
  aLights[0].id = String(c_group_id);
}

int countCommas(String result) {
  //counts the number of values in the result
  int cnt = 0;
  Serial.println(result.length());
  for (int i = 0; i < result.length(); i++) {
    if (result[i] == ',') {
      cnt++;
    }
  }
  return cnt;
}

//-----------------------------------
// Read data from gateway
//------------------------------------
void getGroups() {

  String url = createUrl(tradfriParams_ip, tradfriParams_key, "0", "0", "listgroup", 0);
  String result = execUrl(url, 5000);
  char *str = (char*)result.c_str();
  Serial.println(result);

  int cnt = countCommas(result);

  String arr[cnt]; //max 10 groups. 4 values per group
  //arr[0] = command
  //arr[1] = group id
  //arr[2] = group name
  //arr[3] = not used
  //arr[4] = not used
  //...

  char *p = strtok(str, ",");
  int index = 0;

  while (p != nullptr && index < 20) {
    arr[index++] = String(p);
    p = strtok(NULL, ",");
  }

  // memset(aGroups, 0, sizeof(aGroups)); //reset group array
  int idx = 0;
  //if (String(arr[0]) == String("listgroup")) {
  //read received groups from gateway
  for (int i = 1; i < index; i = i + 4) {
    aGroups[idx][0] = arr[i];
    aGroups[idx][1] = arr[i + 1];
    if (aGroups[idx][0] == aLights[0].id) {
      c_group = idx;
    }
    idx++;
  }
  // }
  setGroupList(idx);
}


void getLights() {
  // get available lights assigned to the selected group from gateway
  String url = createUrl(tradfriParams_ip, tradfriParams_key, aLights[0].id, "0", "listlight", 0);
  String result = execUrl(url, 5000);
  char *str = (char*)result.c_str();
  Serial.println(result);

  clearLightsArrays();

  int cnt = countCommas(result);
  String arr[cnt]; //max 10 lights. 6 values per light
  //arr[0] = command
  //arr[1] = light id
  //arr[2] = type
  //arr[3] = status
  //arr[4] = value
  //...

  char *p = strtok(str, ",");

  int index = 0;
  while (p != nullptr && index < cnt) {
    arr[index++] = String(p);
    p = strtok(NULL, ",");
  }

  int idx = 1;
  //read received groups from gateway
  for (int i = 1; i < index; i = i + 6) {
    if (arr[i] != "") {
      aLights[idx].id = arr[i];
      aLights[idx].name = arr[i + 1];
      aLights[idx].type = arr[i + 2];
      aLights[idx].status = convStatus(arr[i + 3]);
      aLights[idx].value = arr[i + 4].toInt();
      aLights[idx].color = convertColor(arr[i + 5].toInt());
      idx++;
    }
  }
  // }
  pgMain.show();
  calculateGroupStatus();
  refreshScreen();
}


void setLight() {
  String url;
  int val = map(aLights[c_light].value, 0, 100, 0, 254);
  if (c_light == 0) {
    url = createUrl(tradfriParams_ip, tradfriParams_key, aLights[0].id, "0", "setdimmer", val);
  }
  else //it is a light
  {
    url = createUrl(tradfriParams_ip, tradfriParams_key, "0", aLights[c_light].id, "setdimmer", val);
  }
  execUrl(url, 0);
}


void getStatusLight() {

  String url = "/dh/readFile.php";
  //String url = createUrl(tradfriParams_ip, tradfriParams_key, aLights[0].id, "0", "statuslight", 0);
  String result = execUrl(url, 3000);
  char *str = (char*)result.c_str();
  Serial.println(result);

  int cnt = countCommas(result);
  String arr[cnt]; //max 10 lights. 6 values per light

  char *p = strtok(str, ",");
  int index = 0;

  while (p != nullptr && index < cnt) {
    arr[index++] = String(p);
    p = strtok(NULL, ",");
  }

  int idx = -1;
  //read received data from gateway
  for (int i = 0; i < index; i = i + 4) {
    //get light index
    idx = -1;
    for (int x = 1; x < NUM_LIGHTS; x++) {
      if (arr[i] == aLights[x].id) {
        idx = x;
        break; //found light idx
      }
    }
    if (idx != -1) { //if light found
      //set the light values
      aLights[idx].status = convStatus(arr[i + 1]);
      aLights[idx].value = map(arr[i + 2].toInt(), 0, 254, 0, 100);
      aLights[idx].color = convertColor(arr[i + 3].toInt());
    }
  }
  calculateGroupStatus();
  refreshScreen();
}

void calculateGroupStatus() {

  if (aLights[1].status == 1 || aLights[2].status == 1 || aLights[3].status == 1 || aLights[4].status == 1 || aLights[5].status == 1 || aLights[6].status == 1) {
    aLights[0].status = 1;
    int totalLightValue = 0;
    int numLights = 0;
    for (int i = 1; i < NUM_LIGHTS; i++) {
      if (aLights[i].status != 999) {
        totalLightValue = totalLightValue + aLights[i].value;
        numLights++;
      }
    }
    aLights[0].value = int(totalLightValue / numLights);
  }
  else {
    aLights[0].status = 0;
    aLights[0].value = 0;
  }
  Serial.print("status ");
  Serial.println(aLights[0].status);
  Serial.print("value ");
  Serial.println(aLights[0].value);

}
