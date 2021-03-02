/*
HMI Nextion Library

Bentley Born
bentley@crcibernetica.com

Ricardo Mena C
ricardo@crcibernetica.com

http://crcibernetica.com

 License
 **********************************************************************************
 This program is free software; you can redistribute it 
 and/or modify it under the terms of the GNU General    
 Public License as published by the Free Software       
 Foundation; either version 3 of the License, or        
 (at your option) any later version.                    
                                                        
 This program is distributed in the hope that it will   
 be useful, but WITHOUT ANY WARRANTY; without even the  
 implied warranty of MERCHANTABILITY or FITNESS FOR A   
 PARTICULAR PURPOSE. See the GNU General Public        
 License for more details.                              
                                                        
 You should have received a copy of the GNU General    
 Public License along with this program.
 If not, see <http://www.gnu.org/licenses/>.
                                                        
 Licence can be viewed at                               
 http://www.gnu.org/licenses/gpl-3.0.txt

 Please maintain this license information along with authorship
 and copyright notices in any redistribution of this code
 **********************************************************************************
 */

#include "Nextion.h"

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

#if defined(USE_SOFTWARE_SERIAL)
Nextion::Nextion(SoftwareSerial &next, uint32_t baud): nextion(&next){
  nextion->begin(baud);
  flushSerial();
}
#else
Nextion::Nextion(HardwareSerial &next, uint32_t baud): nextion(&next){
  nextion->begin(baud);
  flushSerial();
}
#endif

void Nextion::buttonToggle(boolean &buttonState, String objName, uint8_t picDefualtId, uint8_t picSelected){
  String tempStr = "";
  if (buttonState) {
    tempStr = objName + ".picc="+String(picDefualtId);//Select this picture
    sendCommand(tempStr.c_str());
    tempStr = "ref "+objName;//Refresh component
    sendCommand(tempStr.c_str());
    buttonState = false;
  } else {
    tempStr = objName + ".picc="+String(picSelected);//Select this picture
    sendCommand(tempStr.c_str());
    tempStr = "ref "+objName;//Refresh this component
    sendCommand(tempStr.c_str());
    buttonState = true;
  }
}//end buttonPressed

uint8_t Nextion::buttonOnOff(String find_component, String unknown_component, uint8_t pin, int btn_prev_state){  
  uint8_t btn_state = btn_prev_state;
  if((unknown_component == find_component) && (!btn_state)){
    btn_state = 1;//Led is ON
    digitalWrite(pin, HIGH);
  }else if((unknown_component == find_component) && (btn_state)){
    btn_state = 0;
    digitalWrite(pin, LOW);
  }else{
    //return -1;
  }//end if
  return btn_state;
}//end buttonOnOff

boolean Nextion::setComponentValue(String component, int value){
  String compValue = component +".val=" + value;//Set component value
  sendCommand(compValue.c_str());
  boolean acki = ack();
  return acki;
}//set_component_value

boolean Nextion::ack(void){
  /* CODE+END*/
  uint8_t bytes[4] = {0};
  nextion->setTimeout(20);
  if (sizeof(bytes) != nextion->readBytes((char *)bytes, sizeof(bytes))){
    return 0;
  }//end if
  if((bytes[1]==0xFF)&&(bytes[2]==0xFF)&&(bytes[3]==0xFF)){
    switch (bytes[0]) {
	case 0x00:
	  return false; break;
	  //return "0"; break;      
	case 0x01:
	  return true; break;
	  //return "1"; break;
	  /*case 0x03:
	  return "3"; break;
	case 0x04:
	  return "4"; break;
	case 0x05:
	  return "5"; break;
	case 0x1A:
	  return "1A"; break;
	case 0x1B:
	  return "1B"; break;//*/
	default: 
	  return false;
    }//end switch
  }//end if
}//end

unsigned int Nextion::getComponentValue(String component){
  String getValue = "get "+ component +".val";//Get componetn value
    unsigned int value = 0;
  sendCommand(getValue.c_str());
  uint8_t temp[8] = {0};
  nextion->setTimeout(3000);
  if (sizeof(temp) != nextion->readBytes((char *)temp, sizeof(temp))){
    return -1;
  }//end if

  if((temp[0]==(0x71))&&(temp[5]==0xFF)&&(temp[6]==0xFF)&&(temp[7]==0xFF)){
    value = (temp[4] << 24) | (temp[3] << 16) | (temp[2] << 8) | (temp[1]);//Little-endian convertion
  }//end if
  return value;
}//get_component_value */

unsigned int Nextion::pickComponentPic(String component){
  String getValue = "get "+ component +".pic";//Get componetn value
    unsigned int value = 0;
  sendCommand(getValue.c_str());
  uint8_t temp[8] = {0};
  nextion->setTimeout(20);
  if (sizeof(temp) != nextion->readBytes((char *)temp, sizeof(temp))){
    return -1;
  }//end if
  if((temp[0]==(0x71))&&(temp[5]==0xFF)&&(temp[6]==0xFF)&&(temp[7]==0xFF)){
    value = (temp[4] << 24) | (temp[3] << 16) | (temp[2] << 8) | (temp[1]);//Little-endian convertion
  }//end if
  return value;
}//get_component_value */

boolean Nextion::setComponentText(String component, String txt){
  String componentText = component + ".txt=\"" + txt + "\"";//Set Component text
  sendCommand(componentText.c_str());
  return ack();
}//end set_component_txt

boolean Nextion::setComponentPic(String component, uint32_t pic){
  String componentPic = component + ".pic=" + pic;//Set Component picture
  sendCommand(componentPic.c_str());
  return ack();
}//end set_component_txt

boolean Nextion::updateProgressBar(int x, int y, int maxWidth, int maxHeight, int value, int emptyPictureID, int fullPictureID, int orientation){
	int w1 = 0;
	int h1 = 0;
	int w2 = 0;
	int h2 = 0;
	int offset1 = 0;
	int offset2 = 0;

	if(orientation == 0){ // horizontal
	value = map(value, 0, 100, 0, maxWidth);
	w1 = value;
	h1 = maxHeight;
	w2 = maxWidth - value;
	h2 = maxHeight;
	offset1 = x + value;
	offset2 = y;
	
	}else{ // vertical
	value = map(value, 0, 100, 0, maxHeight);
	offset2 = y;	
	y = y + maxHeight - value;
	w1 = maxWidth;
	h1 = value;
	w2 = maxWidth;
	h2 = maxHeight - value;
	offset1 = x;
	}//end if
	
	String wipe = "picq " + String(x) + "," + String(y) + "," + String(w1) + "," + String(h1) + "," + String(fullPictureID);
	sendCommand(wipe.c_str());
	wipe = "picq " + String(offset1) + "," + String(offset2) + "," + String(w2) + "," + String(h2) + "," + String(emptyPictureID);
	sendCommand(wipe.c_str());

	return ack();

}//end updateProgressBar

String Nextion::getComponentText(String component, uint32_t timeout){
  String tempStr = "get " + component + ".txt";
  sendCommand(tempStr.c_str());
  tempStr = "";
  //tempStr = listen(timeout);
  unsigned long start = millis();
  uint8_t ff = 0;//end message
  while((millis()-start < timeout)){
    if(nextion->available()){
      char b = nextion->read();
      if(String(b, HEX) == "ffff"){ff++;}
       tempStr += String(b);
	   if(ff == 3){//End line
		 ff = 0;
		 break;
	   }//end if
    }//end if
  }//end while
  if(tempStr.startsWith("p")){//0x70
	tempStr = tempStr.substring(1, tempStr.length()-3);
  }else{
	return "1a";
  }//end if*/
  return tempStr;
}//getComponentText

/*
uint8_t* Nextion::listen(unsigned long timeout){//returns generic

  char _bite;
  char _end = 0xff;//end of file x3
  int countEnd = 0;
  uint8_t buffer[10];
  memset(buffer, 0, sizeof(buffer));
  int i = 0;
  
  while(nextion->available()>0){
	//delay(10);
	if(nextion->available()>0){
	  _bite = nextion->read();
	  buffer[i] = _bite;
	  i++;
	  if(_bite == _end){
		countEnd++;
	  }//end if
	  if(countEnd == 3){
		break;
	  }//end if
	}//end if
  }//end while

  return buffer;
}
*/


uint8_t* Nextion::listen(unsigned long timeout)//returns generic
{
    static uint8_t __buffer[10];
	memset(__buffer, 0, sizeof(__buffer));
    uint16_t i;
    uint8_t c;  
    
    while (nextion->available() > 0)
    {   
        delay(10);
        c = nextion->read();
		/*
        if (NEX_RET_EVENT_TOUCH_HEAD == c)
        {
            if (nextion->available() >= 6)
            {
                __buffer[0] = c;  
                for (i = 1; i < 7; i++)
                {
                    __buffer[i] = nextion->read();
                }
                __buffer[i] = 0x00;
                
                if (0xFF == __buffer[4] && 0xFF == __buffer[5] && 0xFF == __buffer[6])
                {
                    //NexTouch::iterate(nex_listen_list, __buffer[1], __buffer[2], (int32_t)__buffer[3]);
					break;
                }
                
            }
        }
		else 
		{
			*/
            if (nextion->available() > 0)
            {
                __buffer[0] = c;  
                for (i = 1; i < 7; i++)
                {
                    __buffer[i] = nextion->read();
                }
                __buffer[i] = 0x00;
                
                if (0xFF == __buffer[4] && 0xFF == __buffer[5] && 0xFF == __buffer[6])
                {
                    //NexTouch::iterate(nex_listen_list, __buffer[1], __buffer[2], (int32_t)__buffer[3]);
					break;
                }
                
            }
       // }
    }
	return __buffer;
}

uint8_t Nextion::pageId(void){
  sendCommand("sendme");
  int a = -1;
  uint8_t pagId_ = listen();
  String pagId = String(pagId_);
  if(pagId != ""){
	return pagId.toInt();
  }
  return -1;
  
}//pageId

void Nextion::sendCommand(const char* cmd){
  while (nextion->available()){
	nextion->read();
  }//end while
  flushSerial();
  nextion->print(cmd);
  nextion->write(0xFF);
  nextion->write(0xFF);
  nextion->write(0xFF);
}//end sendCommand

boolean Nextion::init(const char* pageId){
  String page = "page " + String(pageId);//Page
  sendCommand("");
  ack();
  sendCommand(page.c_str());
  delay(100);
  return ack();
}//end nextion_init

void Nextion::flushSerial(){
  Serial.flush();
  nextion->flush();
}//end flush

 
