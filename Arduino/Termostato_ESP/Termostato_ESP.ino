#include <Timer.h>
#include <SoftwareSerial.h>
#include <Nextion.h>
#include <EEPROM.h>
#include <pitches.h>
#include <Wire.h>
#include <RTClib.h>
#include <AM232X.h>
#include <OneWire.h>
#include <Arduino.h>
#include <JSON_Decoder.h>
#include <OpenWeather.h>

// Choose library to load
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#else // ESP32
#include <WiFi.h>
#endif
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// MQTT ------------------------------------------------------------ //
#define mqtt_server "192.168.178.44"
//#define mqtt_user "homeassistant"
//#define mqtt_password "Saeseetai3kee0Siemeeve7oth2ru3eeQuoh4joo7ohyusaiCh7quoob8tahYieN"

#define mqtt_user "mqtt_user"
#define mqtt_password "topona"

#define humidity_topic "termostato/intumid"
#define temperature_topic "termostato/inttemp"
#define stat_topic "termostato/status"
#define mode_topic "termostato/mode"
#define prog_topic "termostato/program"
#define temp_topic "termostato/settemp"
#define range_topic "termostato/range"
#define fire_topic "termostato/fire"

WiFiClient espClient;
PubSubClient client(espClient);

// ------------------------------------------------------------ //

//#define INIT_EEPROM 0
//#define INIT_WIFI 0

#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15

#define NEXTION_RX D5
#define NEXTION_TX D6
#define PIN_RELE D7
#define PIN_TONE D0
#define LED_IR D8
#define ONE_WIRE_BUS 2 //D4

// ------------------------------------------------------------ //

// =====================================================
// ========= User configured stuff starts here =========
// Further configuration settings can be found in the
// OpenWeather library "User_Setup.h" file

// -----------------------------------------------------------------
// ---- Wifi section ----*/

#ifdef INIT_WIFI
char wifiParams_ssid[32]  = {""};
char wifiParams_passcode[64]  = {""};
#else
char wifiParams_ssid[32];
char wifiParams_passcode[64];
#endif

// Static IP details...
IPAddress ip(192, 168, 178, 224);
IPAddress gateway(192, 168, 178, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

// ---- Meteo section ----*/

struct FORECAST {
  int forecast_day_1;
  int forecast_day_2;
  int forecast_day_3;
  int forecast_day_4;
  int t1min;
  int t1max;
  int t2min;
  int t2max;
  int t3min;
  int t3max;
  int t4min;
  int t4max;
};

FORECAST forecast = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//METEO meteoUrl = {"http://api.openweathermap.org/data/2.5/forecast?units=metric", "http://api.openweathermap.org/data/2.5/weather?units=metric", "fb3d3d91b39bfacebbdf5ce445277a38", "6542285"};
//int phases[] =  {7, 7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0};
//const char* host = "api.openweathermap.org";

int meteoIcons[][2] = {{200, 72}, {201, 64}, {202, 64}, {210, 64}, {211, 64}, {212, 64}, {221, 64}, {230, 64}, {231, 64}, {232, 64}, {300, 80}, {301, 80}, {302, 80}, {310, 80}, {311, 80}, {312, 80}, {313, 80}, {314, 80}, {321, 80}, {500, 80}, {501, 63}, {502, 63}, {503, 63}, {504, 63}, {511, 66}, {520, 80}, {521, 63}, {522, 63}, {531, 71}, {600, 69}, {601, 66}, {602, 66}, {611, 66}, {612, 65}, {613, 66}, {615, 81}, {616, 81}, {620, 70}, {621, 66}, {622, 66}, {701, 68}, {711, 68}, {721, 67}, {731, 68}, {741, 68}, {751, 68}, {761, 68}, {762, 68}, {771, 68}, {781, 68}, {800, 57}, {801, 58}, {802, 59}, {803, 60}, {804, 61}};

// OpenWeather API Details, replace x's with your API key
String api_key = ""; // Obtain this from your OpenWeather account

// Set both your longitude and latitude to at least 4 decimal places
String latitude =  "43.77925"; // 90.0000 to -90.0000 negative for Southern hemisphere
String longitude = "11.24626"; // 180.000 to -180.000 negative for West

String units = "metric";  // or "imperial"
String language = "it";   // See notes tab

OW_Weather ow; // Weather forecast library instance

// ------------------------------------------------------------ //
//-----------------------------------------------------------
#define PINNODE 0
//-----------------------------------------------------------
// reserved pins
//-----------------------------------------------------------
// Data wire is plugged into port 2 on the Arduino

const int pinBattery PROGMEM = A0;
#define PINt 24 //virtual pin for temperature
#define PINh 25 //virtual pin for humidity

//-----------------------------------------------------------
// Nextion Display
//-----------------------------------------------------------
//-----------------------------------------------------------
#define ON  1
#define OFF 0

#define AUTO 0
#define AUTOPROG 1
#define MANUAL 2
#define HUB 3

String modes[4] = {"Automatico","Programma","Manuale","Remoto"};
String progs[3] = {"Giornaliero","Feriale","Festivo"};

void mqtt_update(int idx = 0);

#define DAILY 0
#define WDAYS 1
#define WEND  2

#define TMONOFF    0
#define TMMODE     31
#define TMPROG     32
#define TMFIRE     33
#define TMADJUST   34

#define TMAUTO     0
#define TMAUTOPROG 1
#define TMMANUAL   2
#define TMHUB      3
#define TMDAILY    0
#define TMWDAYS    1
#define TMWEND     2

#define UNKNOWN     56
#define SUNNY       57
#define SCATT_CLOUD 58
#define PARTLY_SUNNY 59
#define CLOUDY      60
#define PARTLY_CLOUDY 61
#define OVERTCAST   62
#define RAIN   63
#define LIGHTRAIN   80
#define STORM 64
#define FLURRIES 65
#define SNOW 66
#define SNOWRAIN 81
#define HAZE 67
#define FOG 68
#define CHANCE_SNOW 69
#define CHANCE_FLURRY 70
#define CHANCE_RAIN 71
#define CHANCE_STORM 72

uint8_t g_sts = ON;
uint8_t g_mode = AUTO;
uint8_t g_prog = DAILY;
uint8_t g_fire = OFF;
uint8_t g_cpage = 0;
boolean g_bit = false;
uint8_t g_meteo = UNKNOWN;
uint8_t g_link = OFF;
uint8_t c_timeline; //current timeline value (temp range)

float t_intTemp = 0;
float t_extTemp = 0;
float t_adjust = 19.0;
float h_intHum = 0;
float h_extHum = 0;
float p_extPre = 0;

//--------------------------------------------//
// field labels
//--------------------------------------------//
#define NEX_RET_CMD_FINISHED            (0x01)
#define NEX_RET_EVENT_LAUNCHED          (0x88)
#define NEX_RET_EVENT_UPGRADED          (0x89)
#define NEX_RET_EVENT_TOUCH_HEAD            (0x65)
#define NEX_RET_EVENT_POSITION_HEAD         (0x67)
#define NEX_RET_EVENT_SLEEP_POSITION_HEAD   (0x68)
#define NEX_RET_CURRENT_PAGE_ID_HEAD        (0x66)
#define NEX_RET_STRING_HEAD                 (0x70)
#define NEX_RET_NUMBER_HEAD                 (0x71)
#define NEX_RET_INVALID_CMD             (0x00)
#define NEX_RET_INVALID_COMPONENT_ID    (0x02)
#define NEX_RET_INVALID_PAGE_ID         (0x03)
#define NEX_RET_INVALID_PICTURE_ID      (0x04)
#define NEX_RET_INVALID_FONT_ID         (0x05)
#define NEX_RET_INVALID_BAUD            (0x11)
#define NEX_RET_INVALID_VARIABLE        (0x1A)
#define NEX_RET_INVALID_OPERATION       (0x1B)

//Codes received from Nextion screen on serial
#define NEX_RET_ONFF  0x20 //on/off
#define NEX_RET_MODE  0x21 //mode
#define NEX_RET_PROG  0x22 //program
#define NEX_RET_INCR  0x23 //manual temp increase
#define NEX_RET_DECR  0x24 //manual temp decrease
#define NEX_RET_SSAV  0x40 //Save
#define NEX_RET_SPRO  0x41 //Save timeline
#define NEX_RET_RSAV  0x50 //Save range
#define NEX_RET_REST  0x48 //reset
#define NEX_RET_TSAV  0x60 //Save time
#define NEX_RET_TWIF  0x99 //Test WiFi

#define PAGE_MAIN 0
#define PAGE_SETTINGS 1
#define PAGE_RANGES 2
#define PAGE_PROGRAMS 3
#define PAGE_DEBUG 4
#define PAGE_TIME 6
#define PAGE_METEO 7


#define IN 1
#define OUT 0

#define TIMEt0 10000 //update sensor data
#define TIMEt1 3000 //send data to coordinator
#define TIMEt2 60000 //set time
#define TIMEt3 5000 // check fire
#define TIMEt4 600000 // meteo

Timer t0; //timer to schedule the sensors and actuators values update

SoftwareSerial nextion(NEXTION_RX, NEXTION_TX);// Nextion TX to pin 11 and RX to pin 10 of Arduino
Nextion HMISerial(nextion, 57600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps

#define TIMELINE_SEGMENTS 48 //timeline ticks. One every 30mins = 48
#define TIMELINE_ARRAY   145 //48* Daily+Working days+Week end +1

#ifdef INIT_EEPROM
byte timeline[TIMELINE_ARRAY] = {9, 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 1};
#else
byte timeline[TIMELINE_ARRAY]; 
#endif
byte range[4][2];
byte weekdays[7][3];

#define BAUD_RATE     115200  // Baud for both Xbee and serial monitor

RTC_DS1307 rtc; //Real time clock instance
AM232X AM2322;  //Temp+Hum sensor instance
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire ds(ONE_WIRE_BUS);

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_IR, OUTPUT);
  pinMode(PIN_RELE, OUTPUT);
  pinMode(PIN_TONE, OUTPUT);
  EEPROM.begin(512);

  Serial.begin(BAUD_RATE);
  Wire.begin(D2, D1);
  Wire.setClock(40000);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  HMISerial.init();

  Serial.println();
  Serial.println();
  Serial.println();

  delay(1000);

  play();
  initScreen();
#ifdef INIT_WIFI
  putWifiParams();
#endif
  getWifiParams();
  wifiTryConnect();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  mqtt_reconnect();
  timeClient.begin();
  timeClient.setTimeOffset(3600);
  initTimeLine();
  updatePinValues(); // read sensor once
  f_refreshMain();
  f_time();
  startTasks();
  getWeather();
}

void loop() {
  // put your main code here, to run repeatedly:
  t0.update();
  //if (!client.connected()) {
  //mqtt_reconnect();
  //}
  client.loop();
  getScreenTouch();

}

void updatePinValues()
{
  //Serial.println(getTemp());
  int status = AM2322.read();
  h_intHum = AM2322.getHumidity();
  t_intTemp = AM2322.getTemperature();

  // am2322.readTemperatureAndHumidity(t_intTemp, h_intHum);
  t_intTemp = t_intTemp - 2.5; //compensate components heating

  //Serial.println(t_intTemp);
  getScreenTouch();
  checkFire();
  mqtt_update(3);
  f_refreshMain();


} // updatePinValues()

void play()
{

  // notes in the melody:
  int melody[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
  };

  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  uint8_t noteDurations[] = {
    4, 8, 8, 4, 4, 4, 4, 4
  };

  for (uint8_t thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(PIN_TONE, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(PIN_TONE);

  }
}

void startTasks() {
  // t0.every(TIMEt1, sendSensorData);
  t0.every(TIMEt0, updatePinValues);
  t0.every(TIMEt2, f_time);
  t0.every(TIMEt2, mqtt_reconnect);
  t0.every(TIMEt3, checkFire);
  t0.every(TIMEt4, getWeather);
}


boolean wifiTryConnect() {
  //se la pagina corrente non è wifi legge i parametri da eeprom
  Serial.print("Provo connessione ");
  //prova la connessione
  WiFi.mode(WIFI_STA);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.config(ip, dns, gateway, subnet); //If you need Internet Access You should Add DNS also...
  WiFi.begin((char*)wifiParams_ssid, (char*)wifiParams_passcode);
  //unsigned long timeout = millis() + 5000;
  WiFi.reconnect();
  while (WiFi.status() != WL_CONNECTED) {
    wifiOff();
    delay(500);
    Serial.print(".");
    wifiOn();
    //nex.poll();
  }
  if (WiFi.status() == WL_CONNECTED) { //connessione attiva
    //se connesso aggiorna variabile wifi su schermo a 1 altrimenti 0
    //iwifi.show();
    //vwifi.setValue(ON);
    putWifiParams(); //scrive valori su eeprom
    Serial.print("Connected IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    //vwifi.setValue(OFF);
    Serial.println("Non connesso");
    wifiCheckConnect();
    return false;
  }
}

void wifiCheckConnect() {
  if (WiFi.status() != WL_CONNECTED) { //connessione attiva
    wifiOff();
    Serial.println("Non connesso");
    WiFi.reconnect();
    //nex.poll();
    if (WiFi.status() == WL_CONNECTED) {
      //wifiRed();
      wifiOn();
      Serial.println("riconnesso WIFI");
    }
  }
}

void wifiOn() {
  String cmd = "vis i_wifi,1";
  //digitalWrite(LED_GREEN, HIGH);
  //sendCommand(cmd.c_str());
  // nex.poll();
}

void wifiOff() {
  String cmd = "vis i_wifi,0";
  //digitalWrite(LED_GREEN, LOW);
  //sendCommand(cmd.c_str());
  //nex.poll();
}

void putWifiParams() {
  EEPROM.put(0, wifiParams_ssid);
  EEPROM.put(33, wifiParams_passcode);
  EEPROM.commit();
  setText("Wifi", "t_ssid", String(wifiParams_ssid));
  setText("Wifi", "t_pass", String(wifiParams_passcode));
}

void getWifiParams() {
  EEPROM.get(0, wifiParams_ssid);
  EEPROM.get(33, wifiParams_passcode);
  setText("Wifi", "t_ssid", String(wifiParams_ssid));
  setText("Wifi", "t_pass", String(wifiParams_passcode));
}
