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


#endif // CONFIG_H
