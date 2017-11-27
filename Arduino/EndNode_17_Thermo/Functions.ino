void initScreen() {
  HMISerial.setComponentValue("gStatus", g_sts);
  HMISerial.setComponentValue("gMode", g_mode);
  HMISerial.setComponentValue("gProg", g_prog);
  HMISerial.setComponentValue("gLink", g_link);
  HMISerial.sendCommand("vis pLink,1");
}

void initTimeLine() {
  /*
    for (int i = 1; i < 7; i++) {
      EEPROM.write(210 + i, 0); //ranges idx 0 to 7 8, 8 values
      //210-217 = working days monday-sunday
    }
    for (int i = 1; i < 7; i++) {
      EEPROM.write(220 + i, 0); //ranges idx 0 to 7 8, 8 values
      //220-227 = saturday
    }
    for (int i = 1; i < 7; i++) {
      EEPROM.write(230 + i, 0); //ranges idx 0 to 7 8, 8 values
      //230-237 = sunday
    }
  */

  /*
      for (int i = 1; i < 49; i++) {
        EEPROM.write(i, 1);
        EEPROM.write(i + 49, 1);
        EEPROM.write(i + 97, 1);
      }
  */

  for (int i = 1; i < 145; i++) {
    timeline[i] = EEPROM.read(i);
    //timeline[i]=1;
  }

  for (int i = 0; i < 8; i++) {
    range [1][0] = EEPROM.read( 200 );
    range [1][1] = EEPROM.read( 201 );
    range [2][0] = EEPROM.read( 202 );
    range [2][1] = EEPROM.read( 203 );
    range [3][0] = EEPROM.read( 204 );
    range [3][1] = EEPROM.read( 205 );
    range [4][0] = EEPROM.read( 206 );
    range [4][1] = EEPROM.read( 207 );
  }

  //save weekly days
  for (int i = 0; i < 8; i++) {
    weekdays[i + 1][DAILY] = EEPROM.read(210 + (10 * DAILY) + i);
  }
  for (int i = 0; i < 8; i++) {
    weekdays[i + 1][WDAYS] = EEPROM.read(210 + (10 * WDAYS) + i);
  }
  for (int i = 0; i < 8; i++) {
    weekdays[i + 1][WEND] = EEPROM.read(210 + (10 * WEND) + i);
  }



  /*
    for (int i = 1; i < 8; i++) {
    EEPROM.write(200 + i, 0); //ranges idx 0 to 7 8, 8 values
    //0 = r1 from
    //1 = r1 to
    //2 = r2 from
    //3 = r2 to
    //4 = r3 from
    //5 = r3 to
    //6 = r4 from
    //7 = r4 to
    }
  */
  /*


    }
  */
}

void f_adjust()
{
  int Int = HMISerial.getComponentValue("gAdjInt");
  int Dec = HMISerial.getComponentValue("gAdjDec");
  t_adjust = (Int * 10) + Dec ;
  t_adjust = t_adjust / 10;
  pBit();
}

void f_extPre()
{
  getScreenTouch();
  HMISerial.setComponentValue("gExtPre", getInt(p_extPre));
}

void f_intHum()
{
  getScreenTouch();
  //float ihum = sensors[getSensorId(PIN_IHUM)][1] / 100;
  HMISerial.setComponentValue("gIntHum", getInt(h_intHum));
}

void f_extHum()
{
  getScreenTouch();
  HMISerial.setComponentValue("gExtHum", getInt(h_extHum));
}

void f_indoor()
{
  getScreenTouch();
  // float itemp = float(sensors[getSensorId(PIN_ITEM)][1])/100;
  HMISerial.setComponentValue("gIntInt", getInt(t_intTemp));
  HMISerial.setComponentValue("gIntDec", getDec(t_intTemp));
}

void f_outdoor()
{
  getScreenTouch();
  HMISerial.setComponentValue("gExtInt", getInt(t_extTemp));
  HMISerial.setComponentValue("gExtDec", getDec(t_extTemp));
}

void f_prog()
{
  //getScreenTouch();
  g_prog = HMISerial.getComponentValue("gProg");
  pBit();
  f_timeline();
}

void f_mode()
{
  //getScreenTouch();
  g_mode = HMISerial.getComponentValue("gMode");
  if (g_mode == HUB) {
    g_fire = OFF;
  }
  else if (g_mode == MANUAL) {
    f_adjust();
  }
  pBit();
  f_timeline();
}

void f_sts()
{
  //getScreenTouch();
  g_sts = HMISerial.getComponentValue("gStatus");
  actuators[getActuatorId(PINNODE)][1] = g_sts;
  actuators[getActuatorId(PINNODE)][3] = g_sts;
  pBit();
  f_timeline();
}

void f_fire()
{
  getScreenTouch();
  HMISerial.setComponentValue("gFire", g_fire);
  digitalWrite(8, g_fire);
}

int f_weekday(int w) {
  //set Monday as first day of the week
  if (w == 1)
    w = 7;
  else
    w -= 1;
  return w;
}

void f_time()
{
  getScreenTouch();
  HMISerial.setComponentValue("t_dd", f_weekday(weekday()));
  HMISerial.setComponentValue("t_hh", hour());
  HMISerial.setComponentValue("t_mm", minute());

  //date
  memset(buffer, 0, sizeof(buffer));
  char xtemp[10];
  itoa(day(), xtemp, 10);
  strcat ( buffer, xtemp);
  strcat ( buffer, "/");
  memset(xtemp, 0, sizeof(xtemp));
  itoa(month(), xtemp, 10);
  strcat ( buffer, xtemp);
  strcat ( buffer, "/");
  memset(xtemp, 0, sizeof(xtemp));
  itoa(year(), xtemp, 10);
  strcat ( buffer, xtemp);
  HMISerial.setComponentText("t_date", buffer);
  f_timeline();
}

void f_meteo(int meteo) {
  getScreenTouch();
  HMISerial.setComponentValue("gMeteo", meteo);
}

void f_timeline() {
  delay(5);
  int idx = 0;
  if (g_cpage == 0) {

    idx = getTimelineId(); // get the current timeline

    if (g_mode == AUTO || g_mode == AUTOPROG) {
      long int current_time = (hour() * 3600L) + (minute() * 60L);
      int cnt = 0;
      String tlx = "";
      for (int i = 0; i < 48; i++) {
        //long int current_range = 1800L * i;
        unsigned int p = (48 * idx) + i;
        if (getCurrentTime(current_time, i)) {
          // if (current_time >= (current_range - 1800L) &&  current_time < current_range) {
          tlx = String(timeline[p] + 4) + tlx;
        }
        else
        {
          tlx = String(timeline[p]) + tlx;
        }
        if (cnt == 7) {
          if (i >= 0 && i <= 7) {
            //Serial.println(tlx);
            HMISerial.setComponentText("tl1", tlx);
          }
          else if (i >= 8 && i <= 15) {
            HMISerial.setComponentText("tl2", tlx);
          }
          else if (i >= 16 && i <= 23) {
            HMISerial.setComponentText("tl3", tlx);
          }
          else if (i >= 24 && i <= 31) {
            HMISerial.setComponentText("tl4", tlx);
          }
          else if (i >= 32 && i <= 39) {
            HMISerial.setComponentText("tl5", tlx);
          }
          else if (i >= 40 && i <= 47) {
            HMISerial.setComponentText("tl6", tlx);
          }
          cnt = 0;
          tlx = "";
        } else {
          cnt++;
        }
      }
    }
    HMISerial.sendCommand("timeline.en=1");
  }
}

void f_refreshTimeline() {
  unsigned int idx = 0;
  int pic = HMISerial.pickComponentPic("pProgram");
  if (pic == 40) //daily
    idx = 0;
  else if (pic == 41) //working days
    idx = 1;
  else if (pic == 42) //weekend
    idx = 2;
  else
    idx = 0;

  int cnt = 0;
  String tlx = "";
  for (int i = 0; i < 48; i++) {
    unsigned int p = (48 * idx) + i;
    if (timeline[p] == 0)
      timeline[p] = 1;
    tlx = String(timeline[p]) + tlx;
    if (cnt == 7) {
      if (i >= 0 && i <= 7) {
        HMISerial.setComponentText("tlx1", tlx);
      }
      else if (i >= 8 && i <= 15) {
        HMISerial.setComponentText("tlx2", tlx);
      }
      else if (i >= 16 && i <= 23) {
        HMISerial.setComponentText("tlx3", tlx);
      }
      else if (i >= 24 && i <= 31) {
        HMISerial.setComponentText("tlx4", tlx);
      }
      else if (i >= 32 && i <= 39) {
        HMISerial.setComponentText("tlx5", tlx);
      }
      else if (i >= 40 && i <= 47) {
        HMISerial.setComponentText("tlx6", tlx);
      }
      cnt = 0;
      tlx = "";
    } else {
      cnt++;
    }
  }
  //send weekly days
  tlx = "";
  for (int i = 6; i >= 0; i--) {
    tlx = String(weekdays[i + 1][idx]) + tlx;
    HMISerial.setComponentText("tld", tlx);
  }
  HMISerial.sendCommand("timelinex.en=1");
}

void f_saveTimeline() {
  unsigned int idx = 0;
  char buffer[10] = {0};

  if (HMISerial.pickComponentPic("pProgram") == 40) //daily
    idx = 0;
  else if (HMISerial.pickComponentPic("pProgram") == 41) //working days
    idx = 1;
  else if (HMISerial.pickComponentPic("pProgram") == 42) //weekend
    idx = 2;
  //save time line

  int cnt = 0 + 48 * idx;
  for (int c = 1; c < 7; c++) {
    String segment = "tlx" + String(c);
    String x = HMISerial.getComponentText(segment);
    delay(5);
    for (int i = 7; i >= 0; i--) {
      int val = x.substring(i, i + 1).toInt();
      EEPROM.write(cnt, val);
      timeline[cnt] = val;
      cnt++;
    }
  }

  String x = HMISerial.getComponentText("tld");
  for (int i = 6; i >= 0; i--) {
    int val = x.substring(i, i + 1).toInt();
    EEPROM.write( 210 + (10 * idx) + i, val);
    weekdays[i + 1][idx]  = val;
  }
  HMISerial.setComponentText("t1", "Saved");
  delay(2000);
  HMISerial.setComponentText("t1", "");
}

void f_refreshRange()
{
  HMISerial.setComponentValue("r0", range [1][0]);
  HMISerial.setComponentValue("r1", range [1][1]);
  HMISerial.setComponentValue("r2", range [2][0]);
  HMISerial.setComponentValue("r3", range [2][1]);
  HMISerial.setComponentValue("r4", range [3][0]);
  HMISerial.setComponentValue("r5", range [3][1]);
  HMISerial.setComponentValue("r6", range [4][0]);
  HMISerial.setComponentValue("r7", range [4][1]);
}

void f_saveRange()
{
  for (int i = 0; i < 8; i++) {
    memset(buffer, 0, sizeof(buffer));
    utoa(i, buffer, 10);
    String cmd;
    cmd += "r";
    cmd += buffer;
    HMISerial.getComponentValue(cmd.c_str());
    EEPROM.write( 200 + i, res);
  }
  range [1][0] = EEPROM.read( 200 );
  range [1][1] = EEPROM.read( 201 );
  range [2][0] = EEPROM.read( 202 );
  range [2][1] = EEPROM.read( 203 );
  range [3][0] = EEPROM.read( 204 );
  range [3][1] = EEPROM.read( 205 );
  range [4][0] = EEPROM.read( 206 );
  range [4][1] = EEPROM.read( 207 );

  HMISerial.setComponentText("t4", "Saved");
  delay(1500);
  HMISerial.setComponentText("t4", "");
}

unsigned int getRange(int value) {
  switch (value) {
    case 45:
      return 1;
      break;
    case 46:
      return 2;
      break;
    case 47:
      return 3;
      break;
    case 48:
      return 4;
      break;
  }
}

void setTimelineProgram(uint8_t idx, uint8_t value) {
  uint32_t pic;
  switch (value) {
    case 1:
      pic = 45;
      break;
    case 2:
      pic = 46;
      break;
    case 3:
      pic = 47;
      break;
    case 4:
      pic = 48;
      break;
  }
  char buffer[10] = {0};
  utoa(idx, buffer, 10);
  String cmd;
  cmd += "p";
  cmd += buffer;
  HMISerial.setComponentPic(cmd.c_str(), pic);
}

void setTimeline(uint8_t idx, uint8_t value, uint8_t current) {
  uint32_t pic;
  if (current == 0) {
    switch (value) {
      case 0:
        pic = 28;
        break;
      case 1:
        pic = 28;
        break;
      case 2:
        pic = 29;
        break;
      case 3:
        pic = 30;
        break;
      case 4:
        pic = 31;
        break;
    }
  }
  else if (current == 1) {
    switch (value) {
      case 0:
        pic = 32;
        break;
      case 1:
        pic = 32;
        break;
      case 2:
        pic = 33;
        break;
      case 3:
        pic = 34;
        break;
      case 4:
        pic = 35;
        break;
    }
  }
  char buffer[10] = {0};
  utoa(idx, buffer, 10);
  String cmd;
  cmd += "p";
  cmd += buffer;
  HMISerial.setComponentPic(cmd.c_str(), pic);
}

void f_refreshMain() {
  f_indoor();
  f_outdoor();
  f_intHum();
  f_extHum();
  f_extPre();
  f_fire();
  f_meteo(g_meteo);
}

int getInt(float x) {
  return (int) x;
}

int getDec(float x)
{
  float temp = x - (long)(x);
  int p = 1;
  for (int i = 0; i < 1; i++) p *= 10;
  int DecimalPart = abs(p * temp);
  return DecimalPart; //return the decimal
}

void getScreenTouch() {

  uint8_t* touch = HMISerial.listen(); //check for message

  /*
    if (touch[0] != 0) {
      Serial.println(touch[0]);
      Serial.println(touch[1]);
      Serial.println(touch[2]);
      Serial.println(touch[3]);
      Serial.println(touch[4]);
    }
  */
  if (touch[0] == NEX_RET_EVENT_TOUCH_HEAD) { // if a message is received...
    if (touch[1] == 0 && touch[2] == 84) { //page 0 pSettings icon
      g_cpage = 1;
    }
    else if (touch[1] == 2 && touch[2] == 1) { //page 2 back (timeline)
      g_cpage = 1;
    }
    else if (touch[1] == 3 && touch[2] == 1) { //page 2 back (timeline)
      g_cpage = 1;
    }
    else if (touch[1] == 2 && touch[2] == 1) { //page 3 back (timeline)
      g_cpage = 1;
    }
    else if (touch[1] == 1 && touch[3] == 3) { //page 1 back
      g_cpage = 0;
    }
  }
  else if (isTouch(touch, NEX_RET_ONFF))
  {
    switch (g_sts) {
      case OFF:
        g_sts = ON;
        break;
      case ON:
        g_sts = OFF;
        break;
    }
    f_sts();
  }
  else if (isTouch(touch, NEX_RET_MODE))
  {
    f_mode();
  }
  else if (isTouch(touch, NEX_RET_PROG))
  {
    f_prog();
  }
  else if (isTouch(touch, NEX_RET_INCR) || isTouch(touch, NEX_RET_DECR))
  {
    f_adjust();
  }
  else if (isTouch(touch, NEX_RET_SSAV))
  {
    f_saveTimeline();
  }
  else if (isTouch(touch, NEX_RET_SPRO))
  {
    f_refreshTimeline();
  }
  else if (isTouch(touch, NEX_RET_RSAV))
  {
    f_saveRange();
  }
  else if (isTouch(touch, NEX_RET_REST))
  {
    //    reset();
  }
  else if (touch[0] == NEX_RET_CURRENT_PAGE_ID_HEAD) {
    g_cpage = touch[1];
    if (g_cpage == PAGE_MAIN)  //page 0
      f_refreshMain();
    else if (g_cpage == PAGE_RANGES) //page 2
      f_refreshRange();
    else if (g_cpage == PAGE_PROGRAMS) //page 3
      f_refreshTimeline();
    //Serial.println(touch[1]);
  }
}

void pBit() {
  if (g_bit == false) {
    HMISerial.setComponentPic("pBit", 55);
    g_bit = true;
  }
  else if (g_bit == true) {
    HMISerial.setComponentPic("pBit", 54);
    g_bit = false;
  }
}

boolean isTouch(uint8_t* touch, uint8_t code) {

  boolean res = false;
  if (touch[0] == code || touch[1] == code || touch[2] == code || touch[3] == code || touch[4] == code || touch[5] == code) {
    res = true;
  }
  return res;
}

void debug(String x, int idx)
{
  //idx 0-7
  if (g_cpage == PAGE_DEBUG) {
    switch (idx) {
      case 0:
        HMISerial.setComponentText("t0", x);
        break;
      case 1:
        HMISerial.setComponentText("t1", x);
        break;
      case 2:
        HMISerial.setComponentText("t2", x);
        break;
      case 3:
        HMISerial.setComponentText("t3", x);
        break;
      case 4:
        HMISerial.setComponentText("t4", x);
        break;
      case 5:
        HMISerial.setComponentText("t5", x);
        break;
      case 6:
        HMISerial.setComponentText("t6", x);
        break;
      case 7:
        HMISerial.setComponentText("t7", x);
        break;
    }
    String row = "t" + idx;
    HMISerial.setComponentText(row, x);
  }
}

void checkFire() {

  switch (g_sts) {
    case OFF:
      HMISerial.setComponentValue("gFire", OFF);
      break;

    case ON:
      if (g_mode == AUTO || g_mode == AUTOPROG)  {
        float t_max = int(range[timeline[c_timeline]][1]/10);// max temp
        if (t_intTemp < t_max) {
          g_fire = ON;
        }
        else {
          g_fire = OFF;
        }
      }
      else if (g_mode == MANUAL) {
        if (t_intTemp < t_adjust) {
          g_fire = ON;
        }
        else {
          g_fire = OFF;
        }
      }
      else if (g_mode == HUB) {
        // do nothing
      }
      break;
  }
  return res;
}

int getTimelineId() {
  int idx = 0;
  if (g_mode == AUTO) { //mode Auto
    //search the weekday to set the right timeline
    for (int i = 210; i < 240; i++) { //EEPROM address for the first weekly days
      if (f_weekday(weekday()) ==  EEPROM.read(i)) {
        if (i >= 210 && i <= 219)
          idx = DAILY;
        else if (i >= 220 && i <= 229)
          idx = WDAYS;
        else if (i >= 230 && i <= 239)
          idx = WEND;
      }
    }
  }
  else if (g_mode == AUTOPROG) { //semi auto
    idx = g_prog;
  }
  return idx;
}

boolean getCurrentTime(long int current_time, int i) {
  boolean res;
  long int current_range = 1800L * i;
  if (current_time >= (current_range - 1800L) &&  current_time < current_range) {
    res = true;
    c_timeline = (48 * getTimelineId()) + i;
  }
  else {
    res = false;
  }
  return res;
}


