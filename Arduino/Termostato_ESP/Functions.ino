void initScreen() {
  HMISerial.setComponentValue("gStatus", g_sts);
  HMISerial.setComponentValue("gMode", g_mode);
  HMISerial.setComponentValue("gProg", g_prog);
  HMISerial.setComponentValue("gLink", g_link);
  HMISerial.sendCommand("vis pLink,0");
}

void initTimeLine() {
  int idx = 100 + TIMELINE_ARRAY;
  Serial.println(idx);
#ifdef INIT_EEPROM

  //write the timeline into the EPROM -- only once
  for (int i = 100; i < idx; i++) {
    EEPROM.put(i, timeline[i - 100]);
    EEPROM.commit();
    Serial.print(EEPROM.read(i));
  }
  for (int i = 1; i < 7; i++) {
    EEPROM.put(250 + i, 0); //ranges idx 0 to 7 8, 8 values
    //210-217 = working days monday-sunday
  }
  for (int i = 1; i < 7; i++) {
    EEPROM.put(260 + i, 0); //ranges idx 0 to 7 8, 8 values
    //220-227 = saturday
  }
  for (int i = 1; i < 7; i++) {
    EEPROM.put(270 + i, 0); //ranges idx 0 to 7 8, 8 values
    //230-237 = sunday
  }

  for (int i = 0; i < 8; i++) {
    EEPROM.put(280 + i, i + 10); //ranges idx 0 to 7 8, 8 values
    //0 = r1 from
    //1 = r1 to
    //2 = r2 from
    //3 = r2 to
    //4 = r3 from
    //5 = r3 to
    //6 = r4 from
    //7 = r4 to
  }
  EEPROM.commit();
#endif

  for (byte i = 100; i < idx; i++) {
    EEPROM.get(i,  timeline[i - 100]);
    Serial.print( timeline[i - 100]);
  }

  Serial.println("");

  f_refreshRange();
  //save weekly days
  for (byte i = 0; i < 7; i++) {
    EEPROM.get(250 + (10 * DAILY) + i, weekdays[i][DAILY]);
  }
  for (byte i = 0; i < 7; i++) {
    EEPROM.get(250 + (10 * WDAYS) + i, weekdays[i][WDAYS] );
  }
  for (byte i = 0; i < 7; i++) {
    EEPROM.get(250 + (10 * WEND) + i, weekdays[i][WEND]);
  }
}

void f_refreshRange()
{
  EEPROM.get( 280, range [0][0]);
  EEPROM.get( 281, range [0][1]);
  EEPROM.get( 282, range [1][0]);
  EEPROM.get( 283, range [1][1]);
  EEPROM.get( 284, range [2][0]);
  EEPROM.get( 285, range [2][1]);
  EEPROM.get( 286, range [3][0]);
  EEPROM.get( 287, range [3][1]);

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
    EEPROM.put( 280 + i, res);
    EEPROM.commit();

  }
  setSaved("t4");
}


void setSaved(String field) {
  HMISerial.setComponentText(field, "Saved");
  delay(1500);
  HMISerial.setComponentText(field, "");
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
  }
  else if (isTouch(touch, NEX_RET_TWIF))
  {
    pBit();
    //inserire codice qui
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
  tone(PIN_TONE, 1500, 30);
  noTone;
}

boolean isTouch(uint8_t* touch, uint8_t code) {

  boolean res = false;
  if (touch[0] == code || touch[1] == code || touch[2] == code || touch[3] == code || touch[4] == code || touch[5] == code) {
    res = true;
    //Serial.println(code);
  }
  return res;
}

void f_sts()
{
  getScreenTouch();
  while (1) {
    g_sts = HMISerial.getComponentValue("gStatus");
    if (g_mode <= 1) {
      break;
    }
  }
  mqtt_update(4);
  if (g_sts == ON) {
    f_timeline();
  }
  else
  {
    g_fire = OFF;
    f_fire;
  }
}

void f_set_sts()
{
  getScreenTouch();
  HMISerial.setComponentValue("gStatus", g_sts);
  delay(10);
  HMISerial.setComponentValue("gStatus", g_sts);
  delay(10);
  HMISerial.setComponentValue("gStatus", g_sts);
  if (g_sts == ON) {
    f_timeline();
  }
  else
  {
    g_fire = OFF;
    f_fire;
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

    /*
      else {

      for (int i = 0; i < TIMELINE_SEGMENTS; i++) {
        String pic = "p" + String(i + 100);
        Serial.println(pic);
        HMISerial.setComponentPic(pic, 25);
      }

      }

      // HMISerial.sendCommand("timeline.en=1");
    */
  }
}

void f_refreshTimeline() {

  if (g_mode == AUTOPROG) {
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
      unsigned int p = (1 + (TIMELINE_SEGMENTS * idx)) + i;
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
      EEPROM.put(cnt + 100, val);
      timeline[cnt] = val;
      cnt++;
    }
  }

  String x = HMISerial.getComponentText("tld");
  for (int i = 6; i >= 0; i--) {
    int val = x.substring(i, i + 1).toInt();
    EEPROM.put( 250 + (10 * idx) + i, val);
    weekdays[i][idx]  = val;
  }
  EEPROM.commit();
  setSaved("t1");
}

void f_set_adjust()
{
  getScreenTouch();
  byte Int = int(t_adjust);
  byte Dec = (t_adjust - int(t_adjust)) * 10;
  HMISerial.setComponentValue("gAdjInt", Int);
  HMISerial.setComponentValue("gAdjDec", Dec);
  delay(10);
  HMISerial.setComponentValue("gAdjInt", Int);
  HMISerial.setComponentValue("gAdjDec", Dec);
  delay(10);
  HMISerial.setComponentValue("gAdjInt", Int);
  HMISerial.setComponentValue("gAdjDec", Dec);
  f_timeline();
}

void f_adjust()
{
  byte Int = HMISerial.getComponentValue("gAdjInt");
  byte Dec = HMISerial.getComponentValue("gAdjDec");
  t_adjust = (Int * 10) + Dec ;
  t_adjust = t_adjust / 10;
  mqtt_update(6);
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
}

void f_outdoor()
{
  getScreenTouch();
  HMISerial.setComponentValue("gExtInt", getInt(t_extTemp));
  HMISerial.setComponentValue("gExtDec", getDec(t_extTemp));
}

void f_set_prog()
{
  getScreenTouch();
  HMISerial.setComponentValue("gProg", g_prog);
  delay(10);
  HMISerial.setComponentValue("gProg", g_prog);
  delay(10);
  HMISerial.setComponentValue("gProg", g_prog);
  f_timeline();
}

void f_prog()
{
  getScreenTouch();
  while (1) {
    g_prog = HMISerial.getComponentValue("gProg");
    if (g_mode <= 2) {
      break;
    }
  }
  mqtt_update(2);
  f_timeline();
}

void f_set_mode()
{
  getScreenTouch();
  HMISerial.setComponentValue("gMode", g_mode);
  delay(10);
  HMISerial.setComponentValue("gMode", g_mode);
  delay(10);
  HMISerial.setComponentValue("gMode", g_mode);
  f_timeline();
}

void f_mode()
{
  getScreenTouch();
  while (1) {
    g_mode = HMISerial.getComponentValue("gMode");
    if (g_mode <= 3) {
      break;
    }
  }
  if (g_mode == HUB) {
    g_fire = OFF;
  }
  else if (g_mode == MANUAL) {
    f_adjust();
  }
  mqtt_update(1);
  //pBit();
  f_timeline();
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
  timeClient.update();
  Serial.println(timeClient.getYear());
  Serial.println(timeClient.getMonth());
  rtc.adjust(DateTime(timeClient.getYear(), timeClient.getMonth(), timeClient.getDay(), timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds()));

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

void f_refreshMain() {
  f_indoor();
  f_outdoor();
  f_intHum();
  f_extHum();
  f_extPre();
  f_battery();
  f_fire();
  f_meteo(g_meteo);
  f_meteo_forecast();
}

void f_meteo(int meteo) {
  getScreenTouch();
  //int icon = getMeteoIcon(meteo);
  //Serial.println(icon);
  HMISerial.setComponentValue("gMeteo", meteo);
}

void f_meteo_forecast() {
  getScreenTouch();
  HMISerial.setComponentValue("pD1", forecast.forecast_day_1);
  HMISerial.setComponentValue("pD2", forecast.forecast_day_2);
  HMISerial.setComponentValue("pD3", forecast.forecast_day_3);
  HMISerial.setComponentValue("pD4", forecast.forecast_day_4);

  HMISerial.setComponentText("t1min", String(forecast.t1min));
  HMISerial.setComponentText("t1max", String(forecast.t1max));
  HMISerial.setComponentText("t2min", String(forecast.t2min));
  HMISerial.setComponentText("t2max", String(forecast.t2max));
  HMISerial.setComponentText("t3min", String(forecast.t3min));
  HMISerial.setComponentText("t3max", String(forecast.t3max));
  HMISerial.setComponentText("t4min", String(forecast.t4min));
  HMISerial.setComponentText("t4max", String(forecast.t4max));


  if (forecast.forecast_day_1 == RAIN || forecast.forecast_day_1 == LIGHTRAIN || forecast.forecast_day_1 == SNOWRAIN || forecast.forecast_day_1 == STORM || forecast.forecast_day_1 == SNOW || forecast.forecast_day_1 == CHANCE_SNOW || forecast.forecast_day_1 == CHANCE_RAIN || forecast.forecast_day_1 == CHANCE_STORM) {
    //meteo led. Turn on if tomorrow will be rainy, snowwy, stormy
    analogWrite(LED_IR, 10);
  } else {
    analogWrite(LED_IR, 0);
  }
}

int getTimelineId() {
  int idx = 0;
  int x;
  DateTime now = rtc.now();
  if (g_mode == AUTO) { //mode Auto
    //search the weekday to set the right timeline
    for (int i = 0xD2; i < 0xF0; i++) { //EEPROM address for the first weekly days
      EEPROM.get(i, x);
      if (f_weekday(now.dayOfTheWeek()) == x)  {
        if (i >= 250 && i < 260) {
          idx = DAILY;
        }
        else if (i >= 260 && i < 270) {
          idx = WDAYS;
        }
        else if (i >= 270 && i < 280) {
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

void setText(String page, String obj, String val) {
  String cmd = page + "." + obj + ".txt=" + String('"') + val + String('"');
  HMISerial.sendCommand(cmd.c_str());
}//setText

int getMeteoIcon(int code) {
  for (int i = 0; i < 55; i++) {
    if (meteoIcons[i][0] == code) {
      return meteoIcons[i][1];
    }
  }
  return UNKNOWN;
}


void getWeather() {
  getScreenTouch();
  OW_current *current = new OW_current;
  OW_hourly *hourly = new OW_hourly;
  OW_daily  *daily = new OW_daily;

  if (WiFi.status() == WL_CONNECTED) {

    // Serial.print("\nRequesting weather information from OpenWeather... ");
    boolean res = ow.getForecast(current, hourly, daily, api_key, latitude, longitude, units, language);
    if (res) {
      getScreenTouch();
      t_extTemp = current->temp;
      h_extHum = current->humidity;
      String wcode = String(current->id);
      p_extPre = current->pressure;
      g_meteo = getMeteoIcon(wcode.toInt());
      Serial.println(wcode);

      wcode = String(daily->id[1]);
      forecast.forecast_day_1 = getMeteoIcon(wcode.toInt());
      wcode = String(daily->id[2]);
      forecast.forecast_day_2 = getMeteoIcon(wcode.toInt());
      wcode = String(daily->id[3]);
      forecast.forecast_day_3 = getMeteoIcon(wcode.toInt());
      wcode = String(daily->id[4]);
      forecast.forecast_day_4 = getMeteoIcon(wcode.toInt());

      forecast.t1min = int(daily->temp_min[1]);
      forecast.t1max = int(daily->temp_max[1]);
      forecast.t2min = int(daily->temp_min[2]);
      forecast.t2max = int(daily->temp_max[2]);
      forecast.t3min = int(daily->temp_min[3]);
      forecast.t3max = int(daily->temp_max[3]);
      forecast.t4min = int(daily->temp_min[4]);
      forecast.t4max = int(daily->temp_max[4]);


      Serial.println("------------------------------------------------------------");

      Serial.print("id               : "); Serial.println(current->id);
      Serial.print("temp             : "); Serial.println(current->temp);
      Serial.print("pressure         : "); Serial.println(current->pressure);
      Serial.print("humidity         : "); Serial.println(current->humidity);
      Serial.print("id               : "); Serial.println(daily->id[1]);
      Serial.print("temp.min         : "); Serial.println(daily->temp_min[1]);
      Serial.print("temp.max         : "); Serial.println(daily->temp_max[1]);
      Serial.print("id               : "); Serial.println(daily->id[2]);
      Serial.print("temp.min         : "); Serial.println(daily->temp_min[2]);
      Serial.print("temp.max         : "); Serial.println(daily->temp_max[2]);
      Serial.print("id               : "); Serial.println(daily->id[2]);
      Serial.print("temp.min         : "); Serial.println(daily->temp_min[2]);
      Serial.print("temp.max         : "); Serial.println(daily->temp_max[2]);
      Serial.print("id               : "); Serial.println(daily->id[2]);
      Serial.print("temp.min         : "); Serial.println(daily->temp_min[2]);
      Serial.print("temp.max         : "); Serial.println(daily->temp_max[2]);

    }
    else {
      Serial.println("Parse error");
      getScreenTouch();
      wifiCheckConnect();
    }
  } else {
    getScreenTouch();
    wifiCheckConnect();
  }
}

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
        if (t_adjust > t_intTemp) {
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
  mqtt_update(5);
  f_fire();
}
