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
	
def checkInit():
	# check Init 
	sql = "SELECT pvalue,pindex FROM tbparam WHERE ptype = 'I'" 
	cur.execute(sql)
	for (pvalue,pindex) in cur:
		i = int("{}".format(pindex))
		if i  == 1:
			output ("Initialize Coordinator")
			sql = "UPDATE tbparam SET pvalue = 0 WHERE ptype = 'I'" 
			cur.execute(sql)
			db.commit()
			cur.close
			initCoordinator()
			break
	sys.stdout.flush()		
	# end check Init
	
#-- Send Init data to the Coordinator --# 
def initCoordinator():  

	printTime()
	output ("Initializing...")

	global pnum
	global INString
	global IXString
	global ISString
	global IAString
	global IMString
	
	cur = db.cursor()
	
	#--------------------------------------------------------------------------------------------------------#
	#----begin building string to send out-------------------------------------------------------------------#
	#--------------------------------------------------------------------------------------------------------#
	# set numbers of parameters to build the string to send to the coordinator 	
	# count the number of nodes 
	sql = "select count(*) as CNT from vwnodes WHERE nodetype != 0" #exclude external node
	cur.execute(sql)
	for (CNT) in cur:
		nodeNum=parseint("{}".format(CNT))
		INString = INString + str(nodeNum*pnum)
	sql = "select count(*) as CNT from vwnodes WHERE nodetype = 2" #xbee nodes
	cur.execute(sql)
	for (CNT) in cur:
		nodeNum=parseint("{}".format(CNT))
		IXString = IXString + str(nodeNum)
	# retrieve node data and buid initialization strings
	sql = "select id, xbee_high_address, xbee_low_address, nodetype from vwnodes WHERE nodetype != 0 AND status = 1 order by id" 
	cur.execute(sql)
	for (id, xbee_high_address, xbee_low_address, nodetype) in cur:			
		INString = INString + "," + "{}".format(id) + "," + "{}".format(nodetype) + ",0,0,1"
		if int("{}".format(nodetype)) == 2: #xbee 
			IXString = IXString + "," + "{}".format(id) + "," + "{}".format(xbee_high_address) + "," + "{}".format(xbee_low_address)
	#db.commit()
	# count the number of sensors 
	sql = "select count(*) as CNT from vwsensors where tbNodeType_id != 0" 
	cur.execute(sql)
	for (CNT) in cur:
		sensorNum=parseint("{}".format(CNT))
		ISString = ISString + str(sensorNum*pnum)
	db.commit()
	#//col 0=node 1=sensor 2=value 3=alarm 4=spare	
    #retrieve sensor data and build initialization strings
	sql = "SELECT nodeid,tbnodetype_id,tbsensortype_id,pin_number FROM vwsensors where tbnodetype_id != 0 and tbstatus_id = 1 order by nodeid,pin_number" 
	cur.execute(sql)
	for (nodeid,tbnodetype_id,tbsensortype_id,pin_number) in cur:			
			ISString = ISString + "," + "{}".format(nodeid) + "," + "{}".format(pin_number) + ",0,0,0"	
	#db.commit()
	# count the number of actuators 
	sql = "select count(*) as CNT from vwactuator" 
	cur.execute(sql)
	for (CNT) in cur:
		actuatorNum=parseint("{}".format(CNT))
		IAString = IAString + str(actuatorNum*pnum)	
	db.commit()
	#//col 0=node 1=sensor 2=value 3=alarm 4=spare	
    #retrieve actuator data and build initialization strings
	sql = "select tbnode_id,pinnumber from tbactuator order by tbnode_id,pinnumber" 
	cur.execute(sql)
	for (tbnode_id,pinnumber) in cur:			
			IAString = IAString + "," + "{}".format(tbnode_id) + "," + "{}".format(pinnumber) + ",0,0,0"
					
	# count the number of methods 
	sql = "select count(*) as CNT from vwmethods" 
	cur.execute(sql)
	for (CNT) in cur:
		methodNum=parseint("{}".format(CNT))
		IMString = IMString + str(methodNum*pnum)	
	db.commit()
	#//col 0=node 1=actuator 2=method 3=value 4=spare	
    #retrieve method data and build initialization strings
	sql = "select tbnode_id,pinnumber,method from vwmethods order by tbnode_id,pinnumber,method" 
	cur.execute(sql)
	for (tbnode_id,pinnumber,method) in cur:		
			IMString = IMString + "," + "{}".format(tbnode_id) + "," + "{}".format(pinnumber) + "," + "{}".format(method) + ",0,0"			
	db.commit()	
	cur.close
	#--------------------------------------------------------------------------------------------------------#
	#----end building string to send out---------------------------------------------------------------------#
	#--------------------------------------------------------------------------------------------------------#

	#--------------------------------------------------------------------------------------------------------#
	#----begin Sending init string to the coordinator -------------------------------------------------------#
	#--------------------------------------------------------------------------------------------------------#
	output("Init sensors")
	ret = initSendStringsToCoordinator(ISString)  
	if ret == 0: #if fails
		return 0
	output("Init actuators")
	#output(IAString)
	ret = initSendStringsToCoordinator(IAString)  
	if ret == 0: #if fails
		return 0
	output("Init methods")
	ret = initSendStringsToCoordinator(IMString)  
	if ret == 0: #if fails
		return 0		
	output("Init nodes")
	ret = initSendStringsToCoordinator(INString)  
	if ret == 0: #if fails
		return 0
	output("Init node addresses Xbee")
	ret = initSendStringsToCoordinator(IXString)  
	if ret == 0: #if fails
		return 0		
	#--------------------------------------------------------------------------------------------------------#
	#----end Sending init string to the coordinator ---------------------------------------------------------#
	#--------------------------------------------------------------------------------------------------------#
	# if Ok
	cur.close
	output ("End Initializing")
	return 1
	
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
	
#--------------------------------------------------------------------------------------------------------#
#---- Receive data from coordinator ---------------------------------------------------------------------#
#--------------------------------------------------------------------------------------------------------#
	
def AUTOreceiveDataFromCoordinator(qDataIn, qDataOut, qResponse, qSQL, qQuery1, qResult1):  # CR0=sensors CR1=nodes CR3=Actuators CR5=All CommExecutedTrue = "CX1" CommExecutedFalse = "CX0"
	global endSerialChars
	#print("Coda in: ",qDataIn.qsize())
	if not qDataIn.empty(): # if there is data to process
		readSerial = qDataIn.get()	
		arrayData = str(readSerial).split(',')
		output("Parsing received data: " + str(len(arrayData)))
		dNum = int((len(arrayData)-1)/(pnum+1))
		if dNum != 0: #if data received on serial
			#--- Write data to database ---#
			sql = ""
			if (parseint(arrayData[0])) != 99: #check the first value if is NOT 99 = smartlight command	
				sql = "insert into tbdatain (timekey,type,"  
				x = 0
				while x < pnum-1:
					sql = sql+ "V" + str(x) + ","	
					x = x+1
				else:	
					sql = sql+ "V" + str(x)	+ ") values "	
				# create Value part of insert statement				
				for i in range(0, dNum):	
					if (parseint(arrayData[i*(pnum+1)])) == 0:
						sql = sql+ "(millis(),1" # sensor
					if (parseint(arrayData[i*(pnum+1)])) == 1:
						sql = sql+ "(millis(),0" # node	
					if (parseint(arrayData[i*(pnum+1)])) == 3:
						sql = sql+ "(millis(),2" # actuator	
					if (parseint(arrayData[i*(pnum+1)])) == 9:
						sql = sql+ "(millis(),9" # method		
					for v in range((i*(pnum+1))+1, (i*(pnum+1))+pnum+1):					
						sql = sql + "," + str(parseint(arrayData[v])) + ""
					sql = sql + ")"	
					if i != dNum-1:
						sql = sql + ","					
			elif (parseint(arrayData[0])) == 99: #smart light command
				output("Ricevuto comando luci")
				# get the smartlight_id
				node = str(parseint(arrayData[1]))
				pin = str(parseint(arrayData[2]))
				sts = str(parseint(arrayData[3]))
				dim = str(parseint(arrayData[4]))
				color = str(parseint(arrayData[5]))
				dev_type = 0
				if pin != "0": #it is a light 0 = group
					dev_type = 1
	
				while not qResult1.empty(): #attendo risutato
					row = qResult1.get()
					smartlight_id = int(row[0])
					actuator_id = int(row[1])
					node_id     = int(row[2])
					pinnumber     = int(row[3])	
					
					if int(pin) == int(pinnumber) and int(node) == int(node_id):			
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
						sql = "insert into tbdataoutsmartlight (timekey,type,V0,V1,V2,V3,V4,V5,V6,V7) values "  
						# create Value part of insert statement		
						sql = sql+ "(millis(),0" # action type 1 set color
						sql = sql+ "," + str(pin) 
						sql = sql+ "," + sts
						sql = sql+ "," + dim
						sql = sql+ "," + color #color/mood	
						sql = sql+ "," + str(actuator_id)	 
						sql = sql+ "," + str(smartlight_id) 
						sql = sql+ "," + str(dev_type) + ",99)"
						#print(sql)										
			# execute sql
			qSQL.put(sql)
	return
			
#--------------------------------------------------------------------------------------------------------#
#---- End AUTOreceiveDataFromCoordinator --------------------------------------------------------------------#
#--------------------------------------------------------------------------------------------------------#
			


#--------------------------------------------------------------------------------------------------------#
#---- End AUTOreceiveDataFromCoordinator --------------------------------------------------------------------#
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

		
def Main():


	return				

#------- Main section ----------------------------#
#------- Run once --------------------------------#
log("I", "Initialize coordinator")
ret = 0
curInit = db.cursor()
#truncate output tables
curInit.callproc('init')
curInit.close()
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
	AUTOreceiveDataFromCoordinator(qIn, qOut, qResp, qSql, qQuery, qResult)



	
    



