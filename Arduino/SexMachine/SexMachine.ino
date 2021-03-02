/*
   SEX MACHINE V1.1
   01-03-2020
*/

#include <SoftwareSerial.h>
#include <Nextion.h>
#include <IRremote.h>
#include "timer.h"
#include "pitches.h"

# define RECVPIN 2
# define MOTOR_STEP_PIN 10
# define MOTOR_DIRECTION_PIN 11
# define MOTOR_ENABLE_PIN 12

# define BUZZER_PIN 5

# define POINT_0_PIN 7
# define POINT_1_PIN 8
# define POINT_2_PIN 9
# define POINT_3_PIN 6

# define NORMAL 0
# define SPECIAL_1 1
# define SPECIAL_2 2
# define SPECIAL_3 3
# define SPECIAL_4 4
# define COLPODURO 14
# define ROTATE 15
# define RECIPROCATING 16


//Objects definition
SoftwareSerial nextion(4, 3);// Nextion TX to pin 4 and RX to pin 5 of Arduino
Nextion myNextion(nextion, 57600); //create a Nextion object named myNextion using the nextion serial port @ 57600bps
IRrecv irrecv(RECVPIN); //IR receiver
Timer timer;

//Global variables
decode_results results;
decode_results prev_results;
decode_results prev_results_special3;
int Operation = NORMAL;
int speed = 200;
const int maxspeed = 3000;
const int minspeed = 50;
long angleToRun = 100;
const int segments = 10;
const long timeFrame = 50;
bool isRunning = false;
int Movement;
int period = 32000;
int periodx = 0; //period reciprocating
int periodcd = 0; //period colpo duro
long initTime;
float periodcd_perc = 0.05;
bool phase = true; //anda false = rianda

//intro
int melody[] = {
  NOTE_D4, -8, NOTE_G4, 16, NOTE_C5, -4,
  NOTE_B4, 8, NOTE_G4, -16, NOTE_E4, -16, NOTE_A4, -16,
  NOTE_D5, 2,
};

int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / 100;
int divider = 0, noteDuration = 0;

void setup() {

  pinMode(MOTOR_STEP_PIN, OUTPUT);
  pinMode(MOTOR_DIRECTION_PIN, OUTPUT);
  pinMode(MOTOR_ENABLE_PIN, OUTPUT);
  pinMode(POINT_0_PIN, INPUT);
  pinMode(POINT_1_PIN, INPUT);
  pinMode(POINT_2_PIN, INPUT);
  pinMode(POINT_3_PIN, INPUT);

  // initialize serial communications (for debugging only):
  Serial.begin(9600);
  irrecv.enableIRIn();
  myNextion.init();

  //Set the interval in milliseconds we want the led to blink
  timer.setInterval(period);

  //Set our callback function
  timer.setCallback(t_callback);

  //Start the timer
  timer.stop();

  NXupdMode();
  NXupdStatus();
  NXupdSpeed();
  NXupdAngle();

  playIntro();

  initStepper();

  NXupdMode();
  NXupdStatus();
  NXupdSpeed();
  NXupdAngle();
}

void loop() {
  timer.update();
  readIR();

  if (isRunning) {
    if (Operation == NORMAL) {
    } else if (Operation == RECIPROCATING) {
      reciprocating();
    } else if (Operation == ROTATE) {
      roundabout();
    } else if (Operation == SPECIAL_1) {
      special_1();
    } else if (Operation == SPECIAL_2) {
      special_2();
    }
  }
  if (Operation == SPECIAL_3) {
    special_3();
  }
}



//-------------------------------------------------------------------------//
//  Stepper functions
//-------------------------------------------------------------------------//

void stepper(int _speed) {
  tone(MOTOR_STEP_PIN, _speed);
}

void initStepper() {
  digitalWrite(MOTOR_ENABLE_PIN, HIGH);
  setSpeed(200, false);
  seekPoint(0, true);
}

void start() {
  beep();
  digitalWrite(MOTOR_ENABLE_PIN, HIGH);
}

void stop() {
  noTone(MOTOR_STEP_PIN);
  digitalWrite(MOTOR_ENABLE_PIN, LOW);
  beep();
}

void setSpeed(int newspeed, bool acceleration) {
  int segment;
  long initTime;
  if (acceleration) {
    int ramp = newspeed - speed;
    segment = round(ramp / segments);
    initTime = millis();
    for (int i = 0; i <= segments; i++) {
      while (true) {
        if (millis() >= (initTime + timeFrame * (long)i )) {
          if ((speed + (segment * i)) < 50) {
            stepper(50);
            break;
          } else {
            stepper(speed + (segment * i));
          }
          break;
        }
      }
    }
  } else {
    stepper(newspeed);
  }
  speed = newspeed;
  NXupdSpeed();
}

void seekPoint(int point, bool stopMovement) {
  while (true) {
    if (point == 0) {
      if (digitalRead(POINT_0_PIN)) {
        if (stopMovement)
          stop();
        break;
      }
    } else if (point == 1) {
      if (digitalRead(POINT_1_PIN)) {
        if (stopMovement)
          stop();
        initTime = millis();
        break;
      }
    } else if (point == 2) {
      if (digitalRead(POINT_2_PIN)) {
        if (stopMovement)
          stop();
        break;
      }
    } else if (point == 3) {
      if (digitalRead(POINT_3_PIN)) {
        if (stopMovement)
          stop();
        break;
      }
    }
  }
}

void reciprocating() {
  seekPoint(1, false);
  while (true) {
    timer.update();
    if (digitalRead(POINT_1_PIN)) {
      setSpeed(speed, false);
      path(speed, angleToRun);
      timer.stop();
      timer.setInterval(periodx);
      timer.start();
    }
    readIR();
    if (Operation != RECIPROCATING) {
      break;
    }
  }
  return;
}

void roundabout() {
  // for future development
  readIR();
}
//-------------------------------------------------------------------------//
//  Special programs
//-------------------------------------------------------------------------//

//rollercoster
void special_1() {
  long timeIncrease = 300;
  long elapsedTime = millis();
  long currentTime = millis();
  bool dir = 0;
  if (isRunning) {
    setSpeed(50, true);
    while (true)
    {
      readIR();
      if (Operation != SPECIAL_1) {
        break;
      }
      elapsedTime = millis() - currentTime;
      if (elapsedTime > timeIncrease) {
        readIR();
        if (Operation != SPECIAL_1) {
          break;
        }
        if (dir == 0) {
          if (speed < maxspeed ) {
            setSpeed(speed + 20, true);
          }
          else {
            setSpeed(maxspeed, true);
            dir = 1;
          }
        }
        else if (dir == 1) {
          readIR();
          if (Operation != SPECIAL_1) {
            break;
          }
          if (speed > minspeed ) {
            setSpeed(speed - 20, true);
          }
          else {
            setSpeed(minspeed, true);
            dir = 0;
          }
        }
        currentTime = millis();
      }
    }
    return;
  }
}

//colpo duro
void special_2() {
  float speed_matrix[] = {1.0, 1.0, 2.0, 2.5, 3.0, 3.5, 5.0, 6.0, 2.0, 1.0, 0.2 , 0.1, 0.01};
  int segment;
  seekPoint(0, false);
  phase = true;
  setSpeed(speed, false);
  path(speed, angleToRun);
  while (true) {
    if (digitalRead(POINT_0_PIN)) {
      setSpeed(speed, false);
      path(speed, angleToRun);
      segment = round(periodx / 13);
      if (phase == false) {
        digitalWrite(MOTOR_DIRECTION_PIN, !digitalRead(MOTOR_DIRECTION_PIN));
        phase = true;
      }
      delay(100);
    } else if (digitalRead(POINT_2_PIN)) {
      phase = false;
      digitalWrite(MOTOR_DIRECTION_PIN, !digitalRead(MOTOR_DIRECTION_PIN));
      delay(100);
    }
    if (phase) {
      initTime = millis();
      for (int i = 0; i < 13; i++) {
        while (true) {
          readIR();
          if (digitalRead(POINT_2_PIN)) {
            break;
          }
          if (millis() >= (initTime + segment * (long)i )) {
            if ((speed * speed_matrix[i]) > maxspeed) {
              stepper(maxspeed);
              break;
            } else if ((speed * speed_matrix[i]) < minspeed) {
              stepper(minspeed);
              break;
            } else {
              stepper(speed * speed_matrix[i]);
            }
            break;
          }
        }
      }

    } else {
      stepper(speed * 2);
    }
    readIR();
    if (Operation != SPECIAL_2) {
      break;
    }
  }
  return;
}

//posizioni
void special_3() {
  readIR();
  if (results.value == 0x01 || results.value == 0x10001 || results.value == 0x02 || results.value == 0x10002 || results.value == 0x03 || results.value == 0x10003 || results.value == 0x04 || results.value == 0x10004) {
    if (prev_results_special3.value != results.value) {
      isRunning = true;
      prev_results_special3 = results;
    }
  }
  if (isRunning) {
    if (results.value == 0x01 || results.value == 0x10001) {
      start();
      setSpeed(200, true);
      seekPoint(0, true);
      isRunning = false;

    } else if (results.value == 0x02 || results.value == 0x10002) {
      start();
      setSpeed(200, true);
      seekPoint(1, true);
      isRunning = false;

    } if (results.value == 0x03 || results.value == 0x10003) {
      start();
      setSpeed(200, true);
      seekPoint(2, true);
      isRunning = false;

    } if (results.value == 0x04 || results.value == 0x10004) {
      start();
      setSpeed(200, true);
      seekPoint(3, true);
      isRunning = false;
    }
  }
}

//-------------------------------------------------------------------------//
//  Functions
//-------------------------------------------------------------------------//

void path(int newspeed, int angle) { //100% = 180grad
  period = round(32000 / (newspeed / 50)) * angle / 100;
  periodx = round(period * 0.9 / 2);
  periodcd = round(period * periodcd_perc / 2);
}

void readIR() {
  if (irrecv.decode(&results)) {  // Grab an IR code
    // Output the results as source code
    irrecv.resume(); // Prepare for the next value
    if (prev_results.value != results.value) {
      Serial.println(results.value, HEX);
      applyRemoteCommand(results.value);
    }
    prev_results.value = results.value;
  }
}

//timer callback function
void t_callback() {
  int current_speed = speed;
  int segments = 10;
  int segment;
  int timeFrame = round(period * 0.05 / segments);
  timer.stop();
  if (Operation == RECIPROCATING) {
    //create ramp down
    int ramp = current_speed * -1;
    segment = round(ramp / segments);
    initTime = millis();
    for (int i = 0; i <= segments; i++) {
      while (true) {
        if (millis() >= (initTime + timeFrame * (long)i )) {
          if ((current_speed + (segment * i)) < minspeed) {
            stepper(minspeed);
            break;
          } else {
            stepper(current_speed + (segment * i));
          }
          break;
        }
      }
    }
    digitalWrite(MOTOR_DIRECTION_PIN, !digitalRead(MOTOR_DIRECTION_PIN));
    //create ramp up
    segment = round(speed / segments);
    initTime = millis();
    for (int i = 1; i <= segments; i++) {
      while (true) {
        if (millis() >= (initTime + timeFrame * (long)i )) {
          ramp = segment * i;
          if (ramp < minspeed) {
            stepper(minspeed);
            break;
          } else {
            stepper(ramp);
          }
          break;
        }
      }
    }
    timer.setInterval(period);
    timer.start();
    /*--------------------------------------------------------------*/
  } else if (Operation == SPECIAL_2) {
    Serial.println("ma via");
    if (phase) {
      current_speed = speed;
      segments = 3;
      segment;
      timeFrame = ((period / 2) - periodcd) / 2 / segments;
      Serial.println(timeFrame);
      timer.stop();
      //create ramp down
      int ramp = current_speed * 4;
      segment = round(ramp / segments);
      initTime = millis();
      for (int i = 0; i <= segments; i++) {
        while (true) {
          if (millis() >= (initTime + timeFrame * (long)i )) {
            if ((current_speed + (segment * i)) < minspeed) {
              stepper(minspeed);
              break;
            } else {
              stepper(current_speed + (segment * i));
            }
            break;
          }
        }
      }
    } else { // se fase di ritorno velocita' costante
      timer.stop();
      //create ramp down
      int ramp = current_speed * 4;
      segment = round(ramp / segments);
      initTime = millis();
      for (int i = 0; i <= segments; i++) {
        while (true) {
          if (millis() >= (initTime + timeFrame * (long)i )) {
            break;
          }
        }
      }
    }
    digitalWrite(MOTOR_DIRECTION_PIN, !digitalRead(MOTOR_DIRECTION_PIN));
    phase = !phase;
    stepper(speed);
    timer.setInterval(period);
    timer.start();
    readIR();
  }
}

void applyRemoteCommand(int code) {
  //philips tv remote command codes
  if (results.value == 0x0 || results.value == 0x10000) {
    // 0
  } else if (results.value == 0xCB16 || results.value == 0x6D || results.value == 0x1006D) {
    // red
    setCommand(1);
  } else if (results.value == 0xCB96 || results.value == 0x6E || results.value == 0x1006E) {
    // verde
    setCommand(2);
  } else if (results.value == 0xCBE6 || results.value == 0xA || results.value == 0x1000A) {
    // back
    setCommand(3);
  } else if (results.value == 0xCBA0 || results.value == 0x5A || results.value == 0x1005A) {
    // freccia sinistra
    setCommand(4);
  } else if (results.value == 0xCB10 || results.value == 0x5B || results.value == 0x1005B) {
    // freccia destra
    setCommand(5);
  } else if (results.value == 0xCB4A || results.value == 0xCC || results.value == 0x100CC) {
    // guide
    setCommand(6);
  } else if (results.value == 0xCB8A || results.value == 0x54 || results.value == 0x10054) {
    // menu
    setCommand(7);
  }  else  if (results.value == 0x20 || results.value == 0x10020) {
    //ch+
    setCommand(8);
  } else if (results.value == 0x21 || results.value == 0x10021) {
    //ch-
    setCommand(9);
  } else if (results.value == 0x10 || results.value == 0x10010) {
    //vol+
    setCommand(10);
  } else if (results.value == 0x11 || results.value == 0x10011) {
    //vol-
    setCommand(11);
  } else if (results.value == 0xCBC0 || results.value == 0x90 || results.value == 0x10090) {
    //stop
    setCommand(12);
  } else if (results.value == 0xCB30 || results.value == 0xD3 || results.value == 0x100D3) {
    //play
    setCommand(13);
  } else if (results.value == 0xCB60 || results.value == 0x58 || results.value == 0x10058) {
    //freccia su
    setCommand(14);
  } else if (results.value == 0xCBE0 || results.value == 0x59 || results.value == 0x10059) {
    //freccia giu
    setCommand(15);
  } else if (results.value == 0xCBF4 || results.value == 0xEB || results.value == 0x100EB) {
    //doppia freccia sinistra
    setCommand(16);
  } else if (results.value == 0xCBBA || results.value == 0xEA || results.value == 0x100EA) {
    //doppia freccia destra
    setCommand(17);
  } else if (results.value == 0x6F || results.value == 0x1006F) {
    //yellow
    setCommand(18);
  }
}

void setCommand(int code) {
  int newspeed;
  switch (code) {
    case 1:      // red
      Operation = SPECIAL_1;
      angleToRun = 100;
      Movement = ROTATE;
      setSpeed(200, true);
      NXupdMode();
      break;
    case 2: // verde
      Operation = SPECIAL_2;
      angleToRun = 50;
      setSpeed(200, true);
      NXupdMode();
      break;
    case 3:  // back
      angleToRun = 100;
      Operation = RECIPROCATING;
      setSpeed(minspeed, true);
      NXupdMode();
      break;
    case 4: // freccia sinistra
      setSpeed(100, true);
      break;
    case 5: // freccia destra
      setSpeed(maxspeed, true);
      break;
    case 6: //rotate
      Operation = ROTATE;
      timer.stop();
      angleToRun = 100;
      setSpeed(200, true);
      NXupdAngle();
      NXupdMode();
      NXupdSpeed();
      break;
    case 7: //reciprocating
      // menu
      Operation = RECIPROCATING;
      angleToRun = 50;
      setSpeed(100, true);
      NXupdAngle();
      NXupdMode();
      NXupdSpeed();
      break;
    case 8: //steps per mm+
      break;
    case 9://steps per mm-
      break;
    case 10: //steps +
      angleToRun = angleToRun + 3;
      if (angleToRun >= 50 ) {
        angleToRun = 50;
      }
      NXupdAngle();
      break;
    case 11: //steps-
      angleToRun = angleToRun - 3;
      if (angleToRun <= 1 ) {
        angleToRun = 1;
      }
      NXupdAngle();
      break;
    case 12: //pause
      isRunning = false;
      stop();
      NXupdStatus();
      break;
    case 13: //play
      isRunning = true;
      digitalWrite(MOTOR_ENABLE_PIN, HIGH);
      Operation = NORMAL;
      start();
      NXupdStatus();
      break;
    case 14: //speed+
      newspeed = speed + 100;
      if (speed > maxspeed ) {
        newspeed = maxspeed;
      }
      if (Operation != RECIPROCATING) {
        setSpeed(newspeed, true);
      } else {
        speed = newspeed;
      }
      break;
    case 15: //speed-
      newspeed = speed - 100;
      if (speed <= minspeed ) {
        newspeed = minspeed;
      }
      if (Operation != RECIPROCATING) {
        setSpeed(newspeed, true);
      } else {
        speed = newspeed;
      }
      Serial.println(speed);
      break;
    case 16: //minima estensione
      setSpeed(minspeed, true);
      seekPoint(0, true);
      break;
    case 17: //massima estensione
      setSpeed(minspeed, true);
      seekPoint(2, true);
      break;
    case 18: //special_3
      Operation = SPECIAL_3;
      setSpeed(minspeed, true);
      seekPoint(0, true);
      break;
  }

}

void playIntro() {

  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }
    tone(BUZZER_PIN, melody[thisNote], noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZER_PIN);
  }
  delay(1000);
}

void beep() {
  noTone(MOTOR_STEP_PIN);
  tone(BUZZER_PIN, 2000, 100);
  delay(100);
  noTone(BUZZER_PIN);
}
