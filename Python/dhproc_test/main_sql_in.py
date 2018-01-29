#!/usr/bin/python
# -*- coding: utf-8 -*-
# Ver 18 - 15 November 2017 -

import time
import serial
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
PORT4 = 5015
PORT5 = 5016
AUTHKEY = str("123456").encode("utf-8")

def output(x):
	print(str(datetime.datetime.now().time())[:8] + " "+ str(x))
	sys.stdout.flush()
# -- DB Connection ---------------------------
try:
  db = mysql.connector.connect(**config)
except mysql.connector.Error as err:
  if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
    output("Something is wrong with your user name or password")
  elif err.errno == errorcode.ER_BAD_DB_ERROR:
    output("Database does not exists")
  else:
    output(err)
else:
  output("Start procedure")
# -- END DB Connection ---------------------------

# -- DB Connection ---------------------------
try:
  dbx = mysql.connector.connect(**config)
except mysql.connector.Error as err:
  if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
    output("Something is wrong with your user name or password")
  elif err.errno == errorcode.ER_BAD_DB_ERROR:
    output("Database does not exists")
  else:
    output(err)
else:
  output("Start procedure")
# -- END DB Connection ---------------------------


# -- Open Serial to the Coordinator---------------

serCoord = serial.Serial('/dev/ttymxc3', 115200, timeout=10)
#serCoord = serial.Serial('COM5', 115200, timeout=5)
serCoord.timeout = 10

serCoord.setDTR(False)
time.sleep(1)
# toss any data already received, see
serCoord.flushInput()

# -- End Open Serial to the Coordinator-----------
  
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
	print(m)
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


def execSQL(qSQL, qQuery1, qResult1):
	cur = db.cursor()
	curX = db.cursor()
	while True:
		try:
			if not qSQL.empty():
				sql = qSQL.get()
				inssql = "UPDATE tbqueue SET timekey = millis(), code = '" + sql + "'"
				cur.execute(inssql)
				db.commit()
				#print("command executed:", inssql)
				print("Command executed")
				
			if qResult1.empty():
				sql = "SELECT smartlight_id, tbactuator_id, tbnode_id, pinnumber FROM vwsmartlight"
				cur.execute(sql)
				row = cur.fetchone()
				while row is not None:	
					qResult1.put(row)
					row = cur.fetchone()
				print("qResult1 size:", qResult1.qsize())			
	
				
		except mysql.connector.Error as err:
			output("SQL error: {}".format(err))	
		time.sleep(0.1)	
			

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
qmIn = QueueServerClient(HOST, PORT0, AUTHKEY)
qmOut = QueueServerClient(HOST, PORT1, AUTHKEY)
qmSql = QueueServerClient(HOST, PORT2, AUTHKEY)
qmResp = QueueServerClient(HOST, PORT3, AUTHKEY)
qmQry = QueueServerClient(HOST, PORT4, AUTHKEY)
qmRslt = QueueServerClient(HOST, PORT5, AUTHKEY)
# Get the queue objects from the clients
qIn = qmIn.get_queue()
qOut = qmOut.get_queue()
qSql = qmSql.get_queue()	
qResp = qmResp.get_queue()	
qQuery = qmQry.get_queue()	
qResult = qmRslt.get_queue()		
#------- End run once    -------------------------#
log("I", "Start main loop")
execSQL(qSql, qQuery, qResult)




	
    



