#ifndef CONFIG_H
#define CONFIG_H

#define SERVER_HOST_NAME "192.168.1.33"
#define TCP_PORT 90
#define DNS_PORT 53

//-----------------------------------------------------------
#define PINNODE 0
//-----------------------------------------------------------
// reserved pins
//-----------------------------------------------------------
#define NEXTION_RX 5 //D2
#define NEXTION_TX 4 //D1
#define PIN_RELE   2 //D4
#define PIN_TONE   12 //D6
#define LED_RED   0 //D3
#define LED_GREEN 16 //D0
#define LED_IR 13 //D7

//-----------------------------------------------------------
// Nextion Display
//-----------------------------------------------------------
//-----------------------------------------------------------
#define ON  1
#define OFF 0
#define PAGE_MAIN           0
#define PAGE_AC             1
#define PAGE_TV             2
#define PAGE_SETTINGS       3
#define PAGE_OPTIONS        4
#define PAGE_CONFIG_LIGHTS  5
#define PAGE_WIFI           6

#define SWITCH_MODE_HWSW    0
#define SWITCH_MODE_SW      1

//Service Led status
#define ON  HIGH
#define OFF LOW

#define NOLIGHT 999

// Output type for Actuator
#define DIGITAL       0
#define ANALOG        1
#define SERVO         2
#define TONE          4
#define HVAC          5
#define TV            6
#define LEDRGB        7
#define THERMOSTAT    9

#define IN 1
#define OUT 0

#define TIMEt0 1000 //update lights data
#define TIMEt1 5000 //wifi connection check

#define BAUD_RATE   115200  // Baud for both Xbee and serial monitor
#define NUM_LIGHTS  7 //max number of lights available (# of lights+1) 

#define TV_SONY      1
#define TV_SAMSUNG   2

#define AC_MIDEA     0

//TV REMOTE
//Actuator methods TV
#define TV0NOFF       12
#define TVVOLUMEUP    13
#define TVVOLUMEDOWN  14
#define TVCHANNELUP   15
#define TVCHANNELDOWN 16
#define TVMUTE        17
#define TVANTENNA     18
#define TVHDMI        19
#define TVBUTRED      20
#define TVBUTGREEN    21
#define TVBUTYELLOW   22
#define TVBUTBLUE     23
#define TVKEYUP       24
#define TVKEYDOWN     25
#define TVKEYLEFT     26
#define TVKEYRIGHT    27
#define TVKEYOK       28
#define TVKEYRETURN   29

//AC commands HVAC
#define ACCOFF      0
#define ACCON       1
#define ACCTEMP     2
#define ACCMODE     3
#define ACCFAN      4
#define ACCSWING    5

//AC methods HVAC
#define ACFAN1        3
#define ACFAN2        4
#define ACFAN3        5
#define ACFANAUTO     6
#define ACMODECOOL    7
#define ACMODEDRY     8
#define ACMODEHEAT    9
#define ACMODEAUTO    10
#define ACTEMPERATURE 11
#define ACSWING       12


#endif // CONFIG_H
