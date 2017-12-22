void initScreen() {
  HMISerial.setComponentValue("gStatus", g_sts);
  HMISerial.setComponentValue("gMode", g_mode);
  HMISerial.setComponentValue("gProg", g_prog);
  HMISerial.setComponentValue("gLink", g_link);
  HMISerial.sendCommand("vis pLink,0");
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


    //write the timeline into the EPROM -- only once
     for (int i = 1; i < sizeof(timeline); i++) {
          EEPROM.write(i, timeline[i]);
     }



    for (int i = 0; i < 8; i++) {
      EEPROM.write(200 + i, i+10); //ranges idx 0 to 7 8, 8 values
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

  for (byte i = 1; i < TIMELINE_ARRAY; i++) {
    timeline[i] = EEPROM.read(i);

    actuators[getActuatorId(PINNODE)][1] = g_sts;
    actuators[getActuatorId(PINNODE)][3] = g_sts;
  }

  f_refreshRange();
  //save weekly days
  for (byte i = 0; i < 7; i++) {
    weekdays[i][DAILY] = EEPROM.read(210 + (10 * DAILY) + i);
  }
  for (byte i = 0; i < 7; i++) {
    weekdays[i][WDAYS] = EEPROM.read(210 + (10 * WDAYS) + i);
  }
  for (byte i = 0; i < 7; i++) {
    weekdays[i][WEND] = EEPROM.read(210 + (10 * WEND) + i);
  }
}

void f_adjust()
{
  byte Int = HMISerial.getComponentValue("gAdjInt");
  byte Dec = HMISerial.getComponentValue("gAdjDec");
  t_adjust = (Int * 10) + Dec ;
  t_adjust = t_adjust / 10;
  //pBit();
}

void f_extPre()
{
  getScreenTouch();
  HMISerial.setComponentValue("gExtPre", getInt(p_extPre));
}

void f_intHum()
{
  getScreenTouch();
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
  HMISerial.setComponentValue("gIntInt", getInt(t_intTemp));
  HMISerial.setComponentValue("gIntDec", getDec(t_intTemp));
  HMISerial.setComponentValue("t", getTemp());
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
  //pBit();
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
  //pBit();
  f_timeline();
}

void f_sts()
{
  //getScreenTouch();
  g_sts = HMISerial.getComponentValue("gStatus");
  actuators[getActuatorId(PINNODE)][1] = g_sts;
  actuators[getActuatorId(PINNODE)][3] = g_sts;
  if (g_sts == ON) {
    f_timeline();
  }
  else
  {
    g_fire = OFF;
    f_fire;
  }

}

void f_fire()
{
  getScreenTouch();
  digitalWrite(PIN_RELE, g_fire);
  HMISerial.setComponentValue("gFire", g_fire);
}

byte f_weekday(byte w) {
  //set Monday as first day of the week
  if (w == 0)
    w = 7;
  return w;
}

void f_battery()
{
  getScreenTouch();
  byte pic = 75;
  if (analogRead(pinBattery) >= 700)
    pic = 79;
  else if (analogRead(pinBattery) < 700 && analogRead(pinBattery) >= 500)
    pic = 78;
  else if (analogRead(pinBattery) < 500 && analogRead(pinBattery) >= 300)
    pic = 77;
  else if (analogRead(pinBattery) < 300 && analogRead(pinBattery) >= 100)
    pic = 76;
  else if (analogRead(pinBattery) < 100)
    pic = 75;

  HMISerial.setComponentPic("pBatt", pic);
}


void f_savetime()
{
  getScreenTouch();
  int p_yr = HMISerial.getComponentValue("p_year");
  uint8_t p_mo = HMISerial.getComponentValue("p_month");
  uint8_t p_dy = HMISerial.getComponentValue("p_day");
  uint8_t p_ho = HMISerial.getComponentValue("p_hour");
  uint8_t p_mi = HMISerial.getComponentValue("p_minute");
  rtc.adjust(DateTime(p_yr, p_mo, p_dy, p_ho, p_mi, 0));
  f_time;
  setSaved("t4");
}

void f_refreshtime()
{
  getScreenTouch();
  DateTime now = rtc.now();
  HMISerial.setComponentValue("p_year", now.year());
  HMISerial.setComponentValue("p_month", now.month());
  HMISerial.setComponentValue("p_day", now.day());
  HMISerial.setComponentValue("p_hour", now.hour());
  HMISerial.setComponentValue("p_minute", now.minute());
}

void f_time()
{
  char buffer[10] = {0};
  getScreenTouch();
  DateTime now = rtc.now();
  HMISerial.setComponentValue("t_dd", f_weekday(now.dayOfTheWeek()));
  HMISerial.setComponentValue("t_hh", now.hour());
  HMISerial.setComponentValue("t_mm", now.minute());

  //date
  memset(buffer, 0, sizeof(buffer));
  char xtemp[10];
  itoa(now.day(), xtemp, 10);
  strcat ( buffer, xtemp);
  strcat ( buffer, "/");
  memset(xtemp, 0, sizeof(xtemp));
  itoa(now.month(), xtemp, 10);
  strcat ( buffer, xtemp);
  strcat ( buffer, "/");
  memset(xtemp, 0, sizeof(xtemp));
  itoa(now.year(), xtemp, 10);
  strcat ( buffer, xtemp);
  HMISerial.setComponentText("t_date", buffer);
  f_timeline();
}

void f_meteo(int meteo) {
  getScreenTouch();
  HMISerial.setComponentValue("gMeteo", meteo);
}

void f_meteo_forecast() {
  getScreenTouch();
  for (int i = 1; i < 5; i++) {
    g_meteof = RxData[1];
    String label = "pD" + String(i);
    HMISerial.setComponentValue(label, RxData[i]);
  }
}

void f_meteo_forecast_temp() {
  getScreenTouch();
  byte cnt = 0;
  for (int i = 1; i < 5; i++) {
    cnt = (i * 2) - 2;
    if (RxData[cnt + 1] >= 9000) //if negative value
      RxData[cnt + 1] = (RxData[cnt + 1] - 9000) * -1;
    String label = "t" + String(i) + "min";
    HMISerial.setComponentText(label, String(RxData[cnt + 1]));
    if (RxData[cnt + 2] >= 9000)//if negative value
      RxData[cnt + 2] = (RxData[cnt + 2] - 9000) * -1;
    label = "t" + String(i) + "max";
    HMISerial.setComponentText(label, String(RxData[cnt + 2]));
  }
}



void f_timeline() {
  DateTime now = rtc.now();
  byte idx = 0;
  if (g_cpage == 0) {

    idx = getTimelineId(); // get the current timeline

    if (g_mode == AUTO || g_mode == AUTOPROG) {
      long int current_time = (now.hour() * 3600L) + (now.minute() * 60L);
      int cnt = 0;
      String tlx = "";
      for (int i = 1; i <= TIMELINE_SEGMENTS; i++) {
        //long int current_range = 1800L * i;
        unsigned int p = (TIMELINE_SEGMENTS * idx) + i;
        if (getCurrentTime(current_time, i)) {
          // if (current_time >= (current_range - 1800L) &&  current_time < current_range) {
          tlx = String(timeline[p] + 4) + tlx; //uses light timeline icons (+4)
        }
        else
        {
          tlx = String(timeline[p]) + tlx;
        }
        if (cnt == 7) {
          if (i >= 1 && i <= 8) {
            HMISerial.setComponentText("tl1", tlx);
          }
          else if (i >= 9 && i <= 16) {
            HMISerial.setComponentText("tl2", tlx);
          }
          else if (i >= 17 && i <= 24) {
            HMISerial.setComponentText("tl3", tlx);
          }
          else if (i >= 25 && i <= 32) {
            HMISerial.setComponentText("tl4", tlx);
          }
          else if (i >= 33 && i <= 40) {
            HMISerial.setComponentText("tl5", tlx);
          }
          else if (i >= 41 && i <= 48) {
            HMISerial.setComponentText("tl6", tlx);
          }
          cnt = 0;
          tlx = "";
        } else {
          cnt++;
        }
      }
    }
    //HMISerial.sendCommand("timeline.en=1");
  }
}

void f_refreshTimeline() {
  //delay(10);
  byte idx = 0;
  int pic = HMISerial.pickComponentPic("pProgram");
  if (pic == 40) {//daily
    idx = 0;
  }
  else if (pic == 41) {//working days
    idx = 1;
  }
  else if (pic == 42) {//weekend
    idx = 2;
  }

  int cnt = 0;
  String tlx = "";

  for (int i = 1; i <= TIMELINE_SEGMENTS; i++) {
    unsigned int p = (TIMELINE_SEGMENTS * idx) + i;
    tlx = String(timeline[p]) + tlx;
    if (cnt == 7) {
      if (i >= 1 && i <= 8) {
        for (int x = 0; x < 5; x++) {
          HMISerial.setComponentText("tlx1", tlx);
        }
      }
      else if (i >= 9 && i <= 16) {
        for (int x = 0; x < 5; x++) {
          HMISerial.setComponentText("tlx2", tlx);
        }
      }
      else if (i >= 17 && i <= 24) {
        for (int x = 0; x < 5; x++) {
          HMISerial.setComponentText("tlx3", tlx);
        }
      }
      else if (i >= 25 && i <= 32) {
        for (int x = 0; x < 5; x++) {
          HMISerial.setComponentText("tlx4", tlx);
        }
      }
      else if (i >= 33 && i <= 40) {
        for (int x = 0; x < 5; x++) {
          HMISerial.setComponentText("tlx5", tlx);
        }
      }
      else if (i >= 41 && i <= 48) {
        for (int x = 0; x < 5; x++) {
          HMISerial.setComponentText("tlx6", tlx);
        }
      }
      cnt = 0;
      tlx = "";
    }
    else {
      cnt++;
    }
  }
  //send weekly days
  tlx = "";
  for (int i = 6; i >= 0; i--) {
    tlx = String(weekdays[i][idx]) + tlx;
  }
  //try to write tlx until it successes
  for (int x = 0; x < 50; x++) {
    HMISerial.setComponentText("tld", tlx);
  }
  //delay(10);
  HMISerial.sendCommand("timelinex.en=1");

}

void f_saveTimeline() {
  unsigned int idx = 0;
  char buffer[10] = {0};
  int pic = HMISerial.pickComponentPic("pProgram");

  if (pic == 40) //daily
    idx = 0;
  else if (pic == 41) //working days
    idx = 1;
  else if (pic == 42) //weekend
    idx = 2;
  //save time line

  int cnt = 1 + (TIMELINE_SEGMENTS * idx);
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
    weekdays[i][idx]  = val;
  }
  setSaved("t1");
}

void f_refreshRange()
{
  range [0][0] = EEPROM.read( 200 );
  range [0][1] = EEPROM.read( 201 );
  range [1][0] = EEPROM.read( 202 );
  range [1][1] = EEPROM.read( 203 );
  range [2][0] = EEPROM.read( 204 );
  range [2][1] = EEPROM.read( 205 );
  range [3][0] = EEPROM.read( 206 );
  range [3][1] = EEPROM.read( 207 );

  HMISerial.setComponentValue("r0", range[0][0]);
  HMISerial.setComponentValue("r1", range[0][1]);
  HMISerial.setComponentValue("r2", range[1][0]);
  HMISerial.setComponentValue("r3", range[1][1]);
  HMISerial.setComponentValue("r4", range[2][0]);
  HMISerial.setComponentValue("r5", range[2][1]);
  HMISerial.setComponentValue("r6", range[3][0]);
  HMISerial.setComponentValue("r7", range[3][1]);

}

void f_saveRange()
{
  char buffer[10] = {0};
  for (int i = 0; i < 8; i++) {
    memset(buffer, 0, sizeof(buffer));
    utoa(i, buffer, 10);
    String cmd;
    cmd += "r";
    cmd += buffer;
    int res = HMISerial.getComponentValue(cmd.c_str());
    EEPROM.write( 200 + i, res);

  }
  setSaved("t4");
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
  f_battery();
  f_fire();
  f_meteo(g_meteo);
  //meteo led. Turn on if tomorrow will be rainy, snowwy, stormy
  if (g_meteof == RAIN || g_meteof == STORM || g_meteof == SNOW || g_meteof == CHANCE_SNOW || g_meteof == CHANCE_RAIN || g_meteof == CHANCE_STORM)
    analogWrite(IR_EMITER, 10);
  else
    analogWrite(IR_EMITER, 0);
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

  if (touch[0] == NEX_RET_EVENT_TOUCH_HEAD) { // if a message is received...
    pBit();
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
    pBit();
    f_sts();
  }
  else if (isTouch(touch, NEX_RET_MODE))
  {
    pBit();
    f_mode();
  }
  else if (isTouch(touch, NEX_RET_PROG))
  {
    pBit();
    f_prog();
  }
  else if (isTouch(touch, NEX_RET_INCR) || isTouch(touch, NEX_RET_DECR))
  {
    pBit();
    f_adjust();
  }
  else if (isTouch(touch, NEX_RET_SSAV))
  {
    pBit();
    f_saveTimeline();
  }
  else if (isTouch(touch, NEX_RET_SPRO))
  {
    pBit();
    f_refreshTimeline();
  }
  else if (isTouch(touch, NEX_RET_RSAV))
  {
    pBit();
    f_saveRange();
  }
  else if (isTouch(touch, NEX_RET_TSAV)) //save time
  {
    pBit();
    f_savetime();
  }
  else if (isTouch(touch, NEX_RET_REST))
  {
    pBit();
    //reset();
  }
  else if (touch[0] == NEX_RET_CURRENT_PAGE_ID_HEAD) {
    pBit();
    g_cpage = touch[1];
    if (g_cpage == PAGE_MAIN)  //page 0
      f_refreshMain();
    else if (g_cpage == PAGE_RANGES) //page 2
      f_refreshRange();
    else if (g_cpage == PAGE_PROGRAMS) //page 3
      f_refreshTimeline();
    else if (g_cpage == PAGE_TIME) //page 6
      f_refreshtime();
  }
}

void pBit() {
  byte pic;
  if (g_bit == false) {
    pic = 55;
    g_bit = true;
  }
  else if (g_bit == true) {
    pic = 54;
    g_bit = false;
  }
  HMISerial.setComponentPic("pBit", pic);
  tone(9, 1500, 30);
  noTone;
  //delay(10);
}

boolean isTouch(uint8_t* touch, uint8_t code) {

  boolean res = false;
  if (touch[0] == code || touch[1] == code || touch[2] == code || touch[3] == code || touch[4] == code || touch[5] == code) {
    res = true;
  }
  return res;
}

/*
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
*/

void checkFire() {

  switch (g_sts) {
    case OFF:
      HMISerial.setComponentValue("gFire", OFF);
      g_fire = OFF;
      break;

    case ON:
      if (g_mode == AUTO || g_mode == AUTOPROG)  {
        float t_max = int(range[timeline[c_timeline] - 1][1]); // max temp
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
  f_fire();
  //return res;
}

int getTimelineId() {
  int idx = 0;
  DateTime now = rtc.now();
  if (g_mode == AUTO) { //mode Auto
    //search the weekday to set the right timeline
    for (int i = 210; i < 240; i++) { //EEPROM address for the first weekly days
      if (f_weekday(now.dayOfTheWeek()) ==  EEPROM.read(i)) {
        if (i >= 210 && i <= 216) {
          idx = DAILY;
        }
        else if (i >= 220 && i <= 226) {
          idx = WDAYS;
        }
        else if (i >= 230 && i <= 236) {
          idx = WEND;
        }
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
    c_timeline = (TIMELINE_SEGMENTS * getTimelineId()) + i;
  }
  else {
    res = false;
  }
  return res;
}

void setSaved(String field) {
  HMISerial.setComponentText(field, "Saved");
  delay(1500);
  HMISerial.setComponentText(field, "");
}


