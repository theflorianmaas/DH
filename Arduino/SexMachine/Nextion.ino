void NXupdSpeed() {
  int spd = map(speed, 0, 3200, 0, 100);
  myNextion.setComponentValue("speed", spd);
}

void NXupdAngle() {
  myNextion.setComponentValue("angle", angleToRun);
}

void NXupdMode() {
  int mode;
  if (Operation == SPECIAL_1) {
    mode = 4;
  } else if (Operation == SPECIAL_2) {
    mode = 5;
  } else if (Operation == ROTATE) {
    mode = 3;
  } else if (Operation == RECIPROCATING) {
    mode = 2;
  } else if (Operation == NORMAL) {
    mode = 6;
  }
  myNextion.setComponentValue("vMode", mode);
}

void NXupdStatus() {
  int status;
  if (isRunning) {
    status = 0;
  } else  if (!isRunning) {
    status = 1;
  }
  myNextion.setComponentValue("vStatus", status);
}
