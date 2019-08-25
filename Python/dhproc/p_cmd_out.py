#!/usr/bin/python
# -*- coding: utf-8 -*-
# Ver 18 - 15 November 2017 -

import time
import string
import sys
import mysql.connector
from mysql.connector import errorcode, pooling
from db import * 
import datetime
#from threading import Thread
import multiprocessing as mp
from multiprocessing import Queue
from multiprocessing.managers import SyncManager

#import queue

ctrlStr = "*../"

HOST = ''
PORT0 = 5011
PORT1 = 5012
PORT2 = 5013
PORT3 = 5014
PORT4 = 5017
PORT5 = 5018
AUTHKEY = str("123456").encode("utf-8")

def output(o, x):
	print(str(str(o) + " " + str(datetime.datetime.now().time())[:8]) + " "+ str(x))
	sys.stdout.flush()
	
def printDBError(x, e):
	output(x, "Error: " +  str(e))             # errno, sqlstate, msg values	
# -- DB Connection ---------------------------
try:
  db = mysql.connector.connect(**config)
except mysql.connector.Error as err:
  if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
    output("P_CMD_OUT","Something is wrong with your user name or password")
  elif err.errno == errorcode.ER_BAD_DB_ERROR:
    output("P_CMD_OUT","Database does not exists")
  else:
    output(err)
else:
  output("P_CMD_OUT","Start procedure")
  db.commit()
# -- END DB Connection ---------------------------

#----------------------------- 
# Global Variable declaration  
#----------------------------- 
 
endSerialChars = b"\r\n"

global readSerial 
global serialBuffer 

pnum = 5 #number of values to send for each sensor 

# coordinator commands
INString = "IN" # to send Node data to the coordinator
ISString = "IS" # to send Sensor data to the coordinator
IXString = "IX" # to send Address data to the coordinator
IAString = "IA" # to send Actuators to the coordinator
IMString = "IM" # to send Methods to the coordinator
CommExecutedTrue = b"CX1\r\n"
CommExecutedFalse = b"CX0\r\n"
CommExecutedTrueX = b"CX1"
CommExecutedFalseX = b"CX0"
CommNotExecuted = b"X"
 
#----------------------------- 
# End Global Variable declaration  
#-----------------------------  


#-- function to extract integer from strings
def parseint(string):
    return int(''.join([x for x in string if x.isdigit()]))

def log(t, m):
	#curLog = db.cursor()
	sql = "insert into tblog (type,msg) VALUES (%s, %s)" 
	#try:
		#curLog.execute(sql, (t,m))
		#db.commit()
		#curLog.close()
	#except:
		#raise
	#curLog.close()	

def printTime():
	now = datetime.datetime.now()
	print(now.strftime("%H %M %S %f"))	
	

def isResponse(response):
	if "CX0" in str(response, 'utf-8'):
		return True
	elif "CX1" in str(response, 'utf-8'):	
		return True
	else:
		return False	
		
def isResponseOK(response):
	print(response)
	res = False
	if "CX0" in str(response, 'utf-8'):
		print(1)
		res = False
	elif "CX1" in str(response, 'utf-8'):	
		print(2)
		res = True
	else:
		print(3)
		res = False	
	print("qqq:")	
	print("xx:", str(response))
	return res	


def execSQL(qOUT, qResponse, qQuery):
	cur = db.cursor()
	while True:
		try:
			#if qOUT.empty():
			sql = "select timekey,type,V0,V1,V2,V3,V4,V5,V6,V7,V8,V9,V10 from tbdataout order by timekey asc"
			cur.execute(sql)
			rows = cur.fetchall()
			db.commit()
			db.start_transaction(False, None, False)
			for row in rows:
				sendCommand(row, qOUT, qResponse)
				sql = "delete from tbdataout where timekey = " + str(row[0])
				#print(sql)
				cur.execute(sql)				
			db.commit
			if qOUT.qsize() > 0:			
				output("P_CMD_OUT","Commands to send: " + str(qOUT.qsize()))			

		except mysql.connector.Error as e:
			printDBError("P_CMD_OUT", e)
			
		time.sleep(0.1)	
			

def sendCommand(row, qOUT, qResponse):  
	#--------------------------------------------------------------------------------------------------------#
	#----begin building string to send out-------------------------------------------------------------------#
	#--------------------------------------------------------------------------------------------------------#
	#0=set trigger
	#1=delete local trigger 
	#2=set actuator 
	#3=reset remote trigger
	#4=set remote trigger
	#5=delete remote trigger
	#6=set time
	#7=set meteo
	#8=set meteo forecast
	#9=set meteo forecast temp
	#10=set smartlight
	#if qResult2.qsize() > 0: #attendo risutato
	#	row = qResult2.get()
	timekey = row[0]
	type = int(row[1])
	V0 = str(row[2])
	V1 = str(row[3])
	V2 = str(row[4])
	V3 = str(row[5])
	V4 = str(row[6])
	V5 = str(row[7])
	V6 = str(row[8])
	V7 = str(row[9])
	V8 = str(row[10])
	V9 = str(row[11])
	V10 = str(row[12])
	if type == 0:
		output("P_CMD_OUT","Set trigger")
		stg = "CW," + V0 + "," + V1 + "," + V2 + "," + V3 + "," + V4 + "," + V5 
	if type == 1:
		output("P_CMD_OUT","Reset trigger")
		stg = "CW," + V0 + "," + V1 + "," + V2 + "," + V3 + "," + V4 + "," + V5 
	if type == 2:
		output("P_CMD_OUT","Set actuator")
		stg = "CW," + V0 + "," + V1 + "," + V2 + "," + V3 + "," + V4 + "," + V5 
	if type == 3:
		stg = "CA," + V0 + "," + V1 + "," + V2 + "," + V3 + "," + V4 + ",0,0,0,0,0,0,0"
	if type == 4:
		output("P_CMD_OUT","Set remote triggers")
			## 0=sensor number 
			## 1=immediate alarm 1=permanent 2=temporary 3=flipflop
			## 2=actuator 
			## 3=level min
			## 4=status 
			## 5=current value
			## 6=level max
			## 7=output type 0=analog 1=digital
			## 8=action out of range
			## 9=action in range
			## 10=timer time
		stg = "CA," + V0 + "," + V1 + "," + V2 + "," + V3 + "," + V4 + ",0," + V5 + "," + V6 + "," + V7 + "," + V8 + "," + V9 + "," + V10
	if type == 5:
		output("P_CMD_OUT","Deleting remote trigger")
		stg = "CA," + V0 + "," + V1 + ",0,0,0,0,0,0,0,0,0,0"	
	if type == 6:
		output("P_CMD_OUT","Set time")
		stg = "CT," + V0 + "," + V1 + "," + V2 + "," + V3 + "," + V4 + "," + V5 
	if type == 7:
		output("P_CMD_OUT","Set meteo")
		stg = "CM," + V0 + "," + V1 + "," + V2 + "," + V3 + "," + V4 
	if type == 8:
		output("P_CMD_OUT","Set meteo forecast")
		stg = "CF," + V0 + "," + V1 + "," + V2 + "," + V3 + "," + V4 
	if type == 9:
		output("P_CMD_OUT","Set meteo forecast temperature")
		stg = "CG," + V0 + "," + V1 + "," + V2 + "," + V3 + "," + V4 + "," + V5 + "," + V6 + "," + V7 + "," + V8 				
	if type == 10:
		output("P_CMD_OUT","Set smartlight commands")
		#V0 node
		#V1 pin
		#V2 sts
		#V3 value
		#V4 color/mood
		stg = "CC," + V0 + "," + V1 + "," + V2 + "," + V3 + "," + V4
	if type == 11:
		output("P_CMD_OUT","Setup smartlight configuration (all device type)")
		#V0 node
		#V1 pin
		#V2 type
		#V3 mode 0=insert 1=delete 2=update
		stg = "CD," + V0 + "," + V1 + "," + V2 + "," + V3  								
	if type == 12:
		output("P_CMD_OUT","Setup smartlight configuration group MOODs")
		#V0 node
		#V1 pin
		#V2 group pin
		#V3 mood1
		#V4 mood2
		#V5 ...
		stg = "CE," + V0 + "," + V1 + "," + V2 + "," + V3 + "," + V4 + "," + V5 + "," + V6 + "," + V7 + "," + V8 + "," + V9 + "," + V10
	#-- common part --#
	#-- Try until 3 times to transmit --#
	for i in range(1):	
		# send the string
		qOUT.put(stg)
		output("P_CMD_OUT","Waiting for the response for attempt " + str(i))
		#print("stampa stringa:", stg)
		sResponse = "empty"
		#initTime = time.time() + 3 #set timeout for the next loop
		#loop = 0 # 0
		#while loop == 0: #data available on serial
		#	if initTime < time.time():
		#		output("Tempo scaduto")
		#		loop = 1
		#	if not qResponse.empty():
		#		sResponse = qResponse.get()
		#		loop = 1
		#if sResponse == CommExecutedTrue: #good response, transmission Ok
		#	output ("Response OkOkOkOKOkOkOk")
		#	break
		#else: #bad response		
		#	output ("Error. trying again....")				
	return

def QueueServerClient(HOST, PORT, AUTHKEY):
    class QueueManager(SyncManager):
        pass
    QueueManager.register('get_queue')
    QueueManager.register('get_name')
    QueueManager.register('get_description')
    manager = QueueManager(address = (HOST, PORT), authkey = AUTHKEY)
    manager.connect() # This starts the connected client
    return manager
	

#------- Main section ----------------------------#
#------- Run once --------------------------------#
# create three connected managers
qmOut = QueueServerClient(HOST, PORT1, AUTHKEY)
qmSql = QueueServerClient(HOST, PORT2, AUTHKEY)
qmResp = QueueServerClient(HOST, PORT3, AUTHKEY)
qmQry = QueueServerClient(HOST, PORT4, AUTHKEY)
qmRslt = QueueServerClient(HOST, PORT5, AUTHKEY)
# Get the queue objects from the clients
qOut = qmOut.get_queue()
qSql = qmSql.get_queue()	
qResp = qmResp.get_queue()	
qQuery2 = qmQry.get_queue()	
qResult2 = qmRslt.get_queue()		
	
#------- End run once    -------------------------#
log("I", "Start main loop")
execSQL(qOut, qResp, qQuery2)




	
    


