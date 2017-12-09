/*
 *  Manage strings by using a generic, dynamic queue data structure.
 *
 *  Copyright (C) 2010  Efstathios Chatzikyriakidis (contact@efxa.org)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

String Queue[10];
int QueueIndex = 0;

// declare string messages.
String msgA = "1,1,1,1,1,6,111,78,34,111";
String msgB = "0,0,0,5,12,34,56,7,0";
String msg1 = "1,2,3,4,5,6,7,8,9,10,11,2,13,14,15,16,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16";
String msg2 = "1,2,3,4,5,6,7,8,9,10,11,99";
String msg3 = "5,12,34,56,7,0,2,1,256";
String msg4 = "10,11,12,13,14,20,21,221,5,12,34,56,7,0,2,1,256,67,0,15,99";

// create a queue of strings messages.

// startup point entry (runs once).
void setup () 
{
  // start serial communication.
  // set the printer of the queue.
  //queue.setPrinter (Serial);
  Serial.begin(115200);
  // push all the string messages to the queue.
  QueueInsert(msgA);
  QueueInsert(msgB);
  QueueInsert(msg2);
  QueueInsert(msg3);
  //queue.push (msgA);
  //queue.push (msg2.c_str());
 Serial.print("Ok: ");
 Serial.println(QueueCount());
  // pop all the string messages from the queue.
  while (QueueIsEmpty() == false){
    Serial.print("Coda: ");
    Serial.println(QueueCount());
    //Serial.println (QueuePop());
     Serial.println("--------------------------------");
    run(QueuePop());
    }
   
}

void run(String x){
  
   int cnt = parseIntRxCount(x);
   int aRxData[cnt]; 
   parseIntRx(x, aRxData);
   // Serial.println(arr);
    Serial.println("--------------------------------");
    for (int i =0;i<sizeof(aRxData)/2;i++)
      Serial.println(aRxData[i]);
}

int parseIntRxCount(String data) {
    String separator = ",";
    int index = 0;

    for(int i = 0; i<data.length()-1; i++) //count the number of occurences
    {    //Walk through the text one letter at a time
      if (String(data[i]) == separator)
        index++;     
    }
    return index+1;
 }    

void parseIntRx(String data, int aRxData[]) {
    String dataPart;      //variable to hole the return text  
    String separator = ",";
    int index = 0;
    int idx = 0;
    int idxNext = 0;
    int cnt = 0;

    idx = 0;
    idxNext = -1;
    cnt = 0;    
    for(int i = 0; i<data.length()-1; i++) 
    {    //Walk through the text one letter at a time
      idx = idxNext+1;
      idxNext = data.indexOf(separator, idx);
      if (idxNext != -1)  {
        dataPart = data.substring(idx,idxNext);
        aRxData[cnt] = dataPart.toInt();
        cnt++;
        }
      else {
        dataPart = data.substring(idx,data.length());
        aRxData[cnt] = dataPart.toInt();
        break;
      }     
    }
  
    //return aRxData;
}

void QueueInsert(String x){
  if (QueueIsFull() == false){
      Queue[QueueIndex] = x; 
      //Serial.println(Queue[QueueIndex]);
      QueueIndex++;
      //Serial.println(QueueIndex);
      
  }
}

String QueuePop(){
  if (QueueIsEmpty() == false){ //not empty
    String value = Queue[0];
    for (int i =1;i<10;i++){
        Queue[i-1] = Queue[i];
    }
    QueueIndex--; 
    return value;
  }
  else
    return "";
}

int QueueCount(){
  return QueueIndex+1; 
}

boolean QueueIsEmpty(){
  if (QueueIndex == 0)
    return true;
  else
    return false;  
}

boolean QueueIsFull(){
  if (QueueIndex == 9)
    return true;
  else
    return false;  
}

// loop the main sketch.
void loop () 
{
  // nothing here.
}


