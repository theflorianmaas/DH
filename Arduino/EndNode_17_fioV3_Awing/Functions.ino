
#if defined (REMOTECONTROL_MODE)
void r_on()
{
  remote_control.turnON();
}

void r_off()
{
  remote_control.turnOFF();
}

void r_mode(int mode)
{

  // allowed values: mode_auto, mode_cool, mode_heat, mode_no_humidity

  remote_control.setMode(mode);
  remote_control.emit();

}

void r_fan(int speed)
{

  // allowed values: fan_off,fan_auto,fan_speed_1,fan_speed_2,fan_speed_3

  remote_control.setSpeedFan(speed);
  remote_control.emit();
}

void r_temp(int temp)
{
  // allowed values: 17-30 C°
  remote_control.setTemperature(temp);
  remote_control.emit();
}

void r_swing()
{
  // toggle swing
  remote_control.doOscilate();
}

void r_nosound()
{
  remote_control.setNoSound();
}

int r_getmode()
{
  return remote_control.getMode();
}

int r_getfan()
{
  return remote_control.getSpeedFan();
}

int r_getstate()
{
  return remote_control.getState();
}

int r_gettemp()
{
  return remote_control.getTemperature();
}

//TV Commands
void tv_on(int brand) {
  if (brand == SONY) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0xa90, 12);
      delay(25);
    }
  }
}

void tv_off(int brand) {
  if (brand == SONY) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0xa90, 12);
      delay(25);
    }
  }
}

void tv_volumeup(int brand) {
  if (brand == SONY) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0xc90, 12);
      delay(25);
    }
  }
}

void tv_volumedown(int brand) {
  if (brand == SONY) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0x490, 12);
      delay(25);
    }
  }
}

void tv_channelup(int brand) {
  if (brand == SONY) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0x90, 12);
      delay(25);
    }
  }
}

void tv_channeldown(int brand) {
  if (brand == SONY) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0x890, 12);
      delay(25);
    }
  }
}

void tv_mute(int brand) {
  if (brand == SONY) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0x290, 12);
      delay(25);
    }
  }
}

void tv_source(int brand) {
  if (brand == SONY) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0xa50, 12);
      delay(25);
    }
  }
}

void tv_home(int brand) {
  if (brand == SONY) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0x070, 12);
      delay(25);
    }
  }
}

void tv_red(int brand) {
  if (brand == SONY) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0x52e9, 12);
      delay(25);
    }
  }
}

void tv_green(int brand) {
  if (brand == SONY) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0x32e9, 12);
      delay(25);
    }
  }
}

void tv_yellow(int brand) {
  if (brand == SONY) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0x72e9, 12);
      delay(25);
    }
  }
}

void tv_blue(int brand) {
  if (brand == SONY) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0x12e9, 12);
      delay(25);
    }
  }
}

void tv_keyup(int brand) {
  if (brand == SONY) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0x2f0, 12);
      delay(25);
    }
  }
}

void tv_keydown(int brand) {
  if (brand == SONY) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0xaf0, 12);
      delay(25);
    }
  }
}

void tv_keyleft(int brand) {
  if (brand == SONY) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0x2d0, 12);
      delay(25);
    }
  }
}

void tv_keyright(int brand) {
  if (brand == SONY) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0xcd0, 12);
      delay(25);
    }
  }
}

void tv_keyok(int brand) {
  if (brand == SONY) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0xa70, 12);
      delay(25);
    }
  }
}

void tv_keyreturn(int brand) {
  if (brand == SONY) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0x62e9, 12);
      delay(25);
    }
  }
}

#endif
