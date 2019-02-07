void refreshScreen() {

  if (isDimmerStarted == false) { //if dimmer is not started
    String setValue;
    Serial.println("refresh group");
    setValue = "stg0.val=" + String(aLights[0].status); //status
    sendCommand(setValue.c_str());
    setValue = "vlg0.val=" + String(aLights[0].value); //value
    sendCommand(setValue.c_str());

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
    setValue = "t_icons.en=1";
    sendCommand(setValue.c_str());
    setValue = "t_select_type.en=1";
    sendCommand(setValue.c_str());
  }
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
  //Serial.println(url);
  return url;
}

/* event callbacks */
static void handleData(void* arg, AsyncClient* client, void *data, size_t len) {
  uint8_t * d = (uint8_t*)data;
  for (size_t i = 0; i < len; i++) {
    if (d[i] == 167 || d[i]  == 63) {
      loadString = true;
      line = "";
    }
    else if (d[i]  == 35) {
      loadString = false;
    }
    if (loadString == true) {
      line = line + String(char(d[i]));
    }
  }

  if (loadString == false)
  {
    line = line.substring(line.indexOf("§") + 2);
    String command = line.substring(0, line.indexOf(","));

    if (command == "statuslight") {
      getStatusLightResult();
    }
    else if (command == "listlight") {
      getLightsResult();
    }
    else if (command == "listgroup") {
      getGroupsResult();
    }
  }
}

void onConnect(void* arg, AsyncClient* client) {
  Serial.printf("\n client has been connected to %s on port %d \n", SERVER_HOST_NAME, TCP_PORT);
}

void execUrl(String url) {
  // This will send the request to the server

  while (client->freeable()) {
    wifiOff();
    Serial.print('.');
    delay(500);
    client->connect(SERVER_HOST_NAME, TCP_PORT);
    wifiOn();
    nex.poll();
  }

  String line = "GET " + url + " HTTP/1.1\r\n";
  line += "Host: " + String(SERVER_HOST_NAME) + ":" + String(TCP_PORT) + "\r\n";
  line += "Connection: keep-alive\r\n\r\n";

  char message[line.length() + 1];
  strcpy(message, line.c_str());
  client->write(message);
  nex.poll();
}

void sendCommand(const char* cmd) {
  while (nextionSerial.available()) {
    nextionSerial.read();
  }//end while
  nextionSerial.print(cmd);
  nextionSerial.write(0xFF);
  nextionSerial.write(0xFF);
  nextionSerial.write(0xFF);
}//end sendCommand

void setText(String page, String obj, String val) {
  String cmd = page + "." + obj + ".txt=" + String('"') + val + String('"');
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
  aLights[0].id = String(c_group_id);
}

int getGroupID() {
  EEPROM.get(180, c_group_id);
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
  disableTask(); //stop timers
  String url = createUrl(tradfriParams_ip, tradfriParams_key, "0", "0", "listgroup", 0);
  execUrl(url);
  nex.poll();
}

void getGroupsResult() {
  char *str = (char*)line.c_str();
  Serial.println(line);

  int cnt = countCommas(line);

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
  nex.poll();
  enableTask();  //restart timers
}

void getLights() {
  // get available lights assigned to the selected group from gateway
  String url = createUrl(tradfriParams_ip, tradfriParams_key, aLights[0].id, "0", "listlight", 0);
  execUrl(url);
  nex.poll();
}

void getLightsResult() {
  char *str = (char*)line.c_str();
  Serial.println(line);

  clearLightsArrays();

  int cnt = countCommas(line);
  String arr[cnt]; //6 values per light
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
  //read received lights from gateway
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

  //pgMain.show();
  sendCommand("page Main");

  nex.poll();
  calculateGroupStatus();
  nex.poll();
  refreshScreen();
  nex.poll();

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
  }\
  execUrl(url);
  nex.poll();
  calculateGroupStatus();
  nex.poll();
}

void configGroup() {
  aGroups[c_group][0].toCharArray(c_group_id, 10);
  aLights[0].id = aGroups[c_group][0];
  putGroupID();
  getLights();
  nex.poll();
}


void getStatusLight() {
  String url = "/dh/readFile.php";
  execUrl(url);
}

void getStatusLightResult() {
  char *str = (char*)line.c_str();
  Serial.println(line);

  int cnt = countCommas(line);
  String arr[cnt]; //max 10 lights. 6 values per light

  char *p = strtok(str, ",");
  int index = 0;

  while (p != nullptr && index < cnt) {
    arr[index++] = String(p);
    p = strtok(NULL, ",");
  }

  int idx = -1;
  //read received data from gateway
  for (int i = 1; i < index; i = i + 4) {
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
  nex.poll();
  calculateGroupStatus();
  nex.poll();
  refreshScreen();
  nex.poll();
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
  nex.poll();
}
