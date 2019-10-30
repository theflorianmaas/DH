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

HOST = ''
PORT0 = 5011
PORT1 = 5012
PORT2 = 5013
PORT3 = 5014
PORT4 = 5015
PORT5 = 5016
AUTHKEY = str("123456").encode("utf-8")

CMDPREFIXIN = "0"
CMDPREFIXOUT = "9"

def output(o, x):
	print(str(str(o) + " " + str(datetime.datetime.now().time())[:8]) + " "+ str(x))
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
  output("P_CMD_GET","Start procedure")
# -- END DB Connection ---------------------------

#----------------------------- 
# Global Variable declaration  
#----------------------------- 
cur = db.cursor()
pnum = 5 #number of values to send for each sensor
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
	
def getLightId(pin, node):
	cur.execute("START TRANSACTION")
	sql = "SELECT smartlight_id, tbactuator_id FROM vwsmartlight WHERE pinnumber = " + pin + " AND tbnode_id = " + node
	cur.execute(sql)
	row = cur.fetchone()
	cur.execute("COMMIT")
	return row			
	
#--------------------------------------------------------------------------------------------------------#
#---- Receive data from coordinator ---------------------------------------------------------------------#
#--------------------------------------------------------------------------------------------------------#
	
def AUTOreceiveDataFromCoordinator(qIN, qSQL):  # CR0=sensors CR1=nodes CR3=Actuators CR5=All CommExecutedTrue = "CX1" CommExecutedFalse = "CX0"
	global endSerialChars
	keepAlive()
	if not qIN.empty(): # if there is data to process
		readSerial = qIN.get()	
		arrayData = str(readSerial).split(',')
		#output("Parsing received data: " + str(len(arrayData)))
		dNum = int((len(arrayData)-1)/(pnum+1))
		if dNum != 0: #if data received on serial
			#--- Write data to database ---#
			sql = ""
			if (parseint(arrayData[0])) != 99: #check the first value if is NOT 99 = smartlight command	
				sql = CMDPREFIXIN + "replace into tbdatain (idx,timekey,type,"  
				x = 0
				while x < pnum-1:
					sql = sql+ "V" + str(x) + ","	
					x = x+1
				else:	
					sql = sql+ "V" + str(x)	+ ") values "	
				# create Value part of insert statement						
				for i in range(0, dNum):
					z = i+1000
					if (parseint(arrayData[i*(pnum+1)])) == 0:
						sql = sql+ "(" + str(z) +",millis(),1" # sensor
					if (parseint(arrayData[i*(pnum+1)])) == 1:
						sql = sql+ "(" + str(z) +",millis(),0" # node	
					if (parseint(arrayData[i*(pnum+1)])) == 3:
						sql = sql+ "(" + str(z) +",millis(),2" # actuator	
					if (parseint(arrayData[i*(pnum+1)])) == 9:
						sql = sql+ "(" + str(z) +",millis(),9" # method		
					for v in range((i*(pnum+1))+1, (i*(pnum+1))+pnum+1):					
						sql = sql + "," + str(parseint(arrayData[v])) + ""
					sql = sql + ")"	
					if i != dNum-1:
						sql = sql + ","					
			elif (parseint(arrayData[0])) == 99: #smart light command
				output("P_CMD_GET","Ricevuto comando luci")
				# get the smartlight_id
				node = str(parseint(arrayData[1]))
				pin = str(parseint(arrayData[2]))
				sts = str(parseint(arrayData[3]))
				dim = str(parseint(arrayData[4]))
				color = str(parseint(arrayData[5]))
				dev_type = 0
				if pin != "0": #it is a light 0 = group
					dev_type = 1
				row = getLightId(pin, node)	
				#cur.execute("START TRANSACTION")
				#sql = "SELECT smartlight_id, tbactuator_id FROM vwsmartlight WHERE pinnumber = " + pin + " AND tbnode_id = " + node
				#cur.execute(sql)
				#row = cur.fetchone()
				#cur.execute("COMMIT")
				if row is not None:	
					smartlight_id = int(row[0])
					actuator_id = int(row[1])				
					#millis
					#type always=0, for future use
					#V0=pin
					#V1=status
					#V2=value
					#V3=color/mood
					#V4=actuator_id
					#V5=smartlight_id
					#V6=type
					#V7=command origin 99=switch
					sql = CMDPREFIXOUT + "insert into tbdataoutsmartlight (timekey,type,V0,V1,V2,V3,V4,V5,V6,V7) values "  
					# create Value part of insert statement		
					sql = sql+ "(millis(),0" # action type 1 set color
					sql = sql+ "," + str(pin) 
					sql = sql+ "," + sts
					sql = sql+ "," + dim
					sql = sql+ "," + color #color/mood	
					sql = sql+ "," + str(actuator_id)	 
					sql = sql+ "," + str(smartlight_id) 
					sql = sql+ "," + str(dev_type) + ",99)"														
			# execute sql
			#print(sql)
			qSQL.put(sql)
	return
			
#--------------------------------------------------------------------------------------------------------#
#---- End AUTOreceiveDataFromCoordinator ----------------------------------------------------------------#
#--------------------------------------------------------------------------------------------------------#
	
def QueueServerClient(HOST, PORT, AUTHKEY):
    class QueueManager(SyncManager):
        pass
    QueueManager.register('get_queue')
    QueueManager.register('get_name')
    QueueManager.register('get_description')
    manager = QueueManager(address = (HOST, PORT), authkey = AUTHKEY)
    manager.connect() # This starts the connected client
    return manager

def keepAlive(): # keep mysql connection active
	cur.execute("SELECT * FROM tbum LIMIT 1")
	cur.execute("COMMIT")
	
		
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
while True:
	AUTOreceiveDataFromCoordinator(qIn, qSql)



	
    


