//TV Commands
void tv_remote(int command, int brand) {
  if (brand == TV_SONY) {
    switch (command) {
      case TV0NOFF:
        irTV.sendSony(0xa90, 12, 2);
        break;
      case TVVOLUMEUP:
        irTV.sendSony(0xc90, 12, 2);
        break;
      case TVVOLUMEDOWN:
        irTV.sendSony(0x490, 12, 2);
        break;
      case TVCHANNELUP:
        irTV.sendSony(0x890, 12, 2);
        break;
      case TVCHANNELDOWN:
        irTV.sendSony(0x90, 12, 2);
        break;
      case TVKEYUP:
        irTV.sendSony(0x2f0, 12, 2);
        break;
      case TVKEYDOWN:
        irTV.sendSony(0xaf0, 12, 2);
        break;
      case TVKEYLEFT:
        irTV.sendSony(0x2d0, 12, 2);
        break;
      case TVKEYRIGHT:
        irTV.sendSony(0xcd0, 12, 2);
        break;
      case TVKEYOK:
        irTV.sendSony(0xa70, 12, 2);
        break;
      case TVKEYRETURN:
        irTV.sendSony(0x62e9, 12, 2);
        break;
      case TVHDMI:
        irTV.sendSony(0xa50, 12, 2);
        break;
      case TVMUTE:
        irTV.sendSony(0x290, 12, 2);
        break;
    }
  }
}

//AC Commands
void ac_remote(int command, int brand) {
  // Support for Midea, Hokkaido HVAC, Type:R51M/E remote control //
  switch (command) {
    case ACCOFF:
      irAC.off();
      break;
    case ACCON:
      irAC.on();
      break;
  }
}


/*
  void begin();
  void on();
  void off();
  void setPower(const bool state);
  bool getPower();
  void setTemp(const uint8_t temp, const bool useCelsius = false);
  uint8_t getTemp(const bool useCelsius = false);
  void setFan(const uint8_t fan);
  uint8_t getFan();
  void setMode(const uint8_t mode);
  uint8_t getMode();
  void setRaw(uint64_t newState);
  uint64_t getRaw();
  static bool validChecksum(const uint64_t state);
  void setSleep(const bool state);
  bool getSleep();
*/
