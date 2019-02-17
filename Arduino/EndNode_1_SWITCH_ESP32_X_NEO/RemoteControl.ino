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
        irTV.sendSony(0x90, 12, 2);
        break;
      case TVCHANNELDOWN:
        irTV.sendSony(0x890, 12, 2);
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

}
