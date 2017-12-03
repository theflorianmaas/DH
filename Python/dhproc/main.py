#!/usr/bin/python
# -*- coding: utf-8 -*-
# Ver 18 - 15 November 2017 -

import time
import serial
import string
import sys
import mysql.connector
from mysql.connector import errorcode
from db import * 
import datetime

ctrlStr = "*../"

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


# -- Open Serial to the Coordinator---------------

serCoord = serial.Serial('/dev/ttymxc3', 115200, timeout=10)
#serCoord = serial.Serial('COM5', 115200, timeout=5)
serCoord.timeout = 10

serCoord.setDTR(False)
time.sleep(1)
# toss any data already received, see
# http://pyserial.sourceforge.net/pyserial_api.html#serial.Serial.flushInput
serCoord.flushInput()
serCoord.setDTR(True)

#serCoord.flush()
# send a string to initialize serial
#serCoord.write(b"+++")
# -- End Open Serial to the Coordinator-----------
  
#----------------------------- 
# Global Variable declaration  
#----------------------------- 
 
cur = db.cursor()
curUpd = db.cursor()
curDel = db.cursor()
endSerialChars = b"\r\n"

pnum = 5 #number of values to send for each sensor 

# coordinator commands
INString = "IN" # to send Node data to the coordinator
ISString = "IS" # to send Sensor data to the coordinator
IXString = "IX" # to send Address data to the coordinator
IAString = "IA" # to send Actuators to the coordinator
IMString = "IM" # to send Methods to the coordinator
CommExecutedTrue = b"CX1\r\n"
CommExecutedFalse = b"CX0\r\n"
CommNotExecuted = b"X"
 
#----------------------------- 
# End Global Variable declaration  
#-----------------------------  

#truncate alarm tables
cur.callproc('init')
	
#-- function to extract integer from strings
def parseint(string):
    return int(''.join([x for x in string if x.isdigit()]))

def log(t, m):
	sql = "insert into tblog (type,msg) VALUES (%s, %s)" 
	try:
		cur.execute(sql, (t,m))
		db.commit()
		cur.close
	except:
		raise

def printTime():
	now = datetime.datetime.now()
	output(now.strftime("%H %M %S %f"))	
	
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
	print(IAString)
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
	output ("End Initializing")
	return 1

#--------------------------------------------------------------------------------------------------------#
#---- Receive data from coordinator ---------------------------------------------------------------------#
#--------------------------------------------------------------------------------------------------------#
def Controller():
	global endSerialChars
	output("Waiting for data on serial")
	if serCoord.inWaiting() > 0: #data available on serial
		serCoord.timeout = 1
		readSerial = serCoord.readline()
		readSerial.rstrip(endSerialChars)
		output("Data received")	
		serCoord.timeout = 2
		AUTOreceiveDataFromCoordinator(readSerial)
	else:	
		sendCommand()
		
def AUTOreceiveDataFromCoordinator(receivedData):  # CR0=sensors CR1=nodes CR3=Actuators CR5=All CommExecutedTrue = "CX1" CommExecutedFalse = "CX0"
	output("Parsing received data...")
	arrayData = str(receivedData).split(',')
	dNum = int((len(arrayData)-1)/(pnum+1))
	if dNum != 0: #if data received on serial
		#--- Write data to database ---#
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
			try:
				inssql = "UPDATE tbqueue SET timekey = millis(), code = '" + sql + "'"
				cur.execute(inssql)
				db.commit()
			except mysql.connector.Error as err:
				output("SQL error: {}".format(err))	
				
		elif (parseint(arrayData[0])) == 99: #smart light command
			# get the smartlight_id
			node = str(parseint(arrayData[1]))
			pin = str(parseint(arrayData[2]))
			sts = str(parseint(arrayData[3]))
			dim = str(parseint(arrayData[4]))
			color = str(parseint(arrayData[5]))
			dev_type = 0
			if pin != "0": #it is a light 0 = group
				dev_type = 1
			sql = "SELECT smartlight_id, type, tbactuator_id, tbnode_id FROM vwsmartlight WHERE tbnode_id = "  
			sql = sql + node + " AND pinnumber = " + pin
			print (sql)
			cur.execute(sql)
			for (smartlight_id, type, tbactuator_id, tbnode_id) in cur:
				smartlight_id = parseint("{}".format(smartlight_id))
				actuator_id = parseint("{}".format(tbactuator_id))
				node_id = "{}".format(tbnode_id)
				action_type = "{}".format(type)
			
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
			print(sql)										
			try:
				#inssql = "UPDATE tbqueue SET timekey = millis(), code = '" + sql + "'"
				cur.execute(sql)
				db.commit()
			except mysql.connector.Error as err:
				output("SQL error: {}".format(err))	
						
	return 1
#--------------------------------------------------------------------------------------------------------#
#---- End AUTOreceiveDataFromCoordinator --------------------------------------------------------------------#
#--------------------------------------------------------------------------------------------------------#
	
def initSendStringsToCoordinator(stg):  
	serCoord.flushInput()
	output(stg)
	# send the node string
	attemptsCnt = 0
	while serCoord.inWaiting() == 0  and attemptsCnt < 5:
		ret = serCoord.write(bytes(stg, 'UTF-8')) 
		readSerial = serCoord.readline()
		if readSerial == CommExecutedTrue:
			return 1
			time.sleep(0.2)
			break
		elif readSerial == CommExecutedFalse:
			# write error in log
			log("E", "Error "+stg)
		else:
			attemptsCnt = attemptsCnt + 1
			output(attemptsCnt)
			continue
	# write error in log
	log("E", "no serial available")
	return 0
	

def sendCommand():  
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
	sql = "select timekey,type,V0,V1,V2,V3,V4,V5,V6,V7,V8,V9,V10 from tbdataout order by timekey asc"
	cur.execute(sql)
	for (timekey,type,V0,V1,V2,V3,V4,V5,V6,V7,V8,V9,V10) in cur:
		if int("{}".format(type)) == 0:
			output("Set trigger")
			stg = "CW," + "{}".format(V0) + "," + "{}".format(V1) + "," + "{}".format(V2) + "," + "{}".format(V3) + "," + "{}".format(V4) + "," + "{}".format(V5) 
		if int("{}".format(type)) == 1:
			output("Reset trigger")
			stg = "CW," + "{}".format(V0) + "," + "{}".format(V1) + "," + "{}".format(V2) + "," + "{}".format(V3) + "," + "{}".format(V4) + "," + "{}".format(V5) 
		if int("{}".format(type)) == 2:
			output("Set actuator")
			stg = "CW," + "{}".format(V0) + "," + "{}".format(V1) + "," + "{}".format(V2) + "," + "{}".format(V3) + "," + "{}".format(V4) + "," + "{}".format(V5) 
		if int("{}".format(type)) == 3:
			stg = "CA," + "{}".format(V0) + "," + "{}".format(V1) + "," + "{}".format(V2) + "," + "{}".format(V3) + "," + "{}".format(V4) + ",0,0,0,0,0,0,0"
		if int("{}".format(type)) == 4:
			output("Set remote triggers")
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
			stg = "CA," + "{}".format(V0) + "," + "{}".format(V1) + "," + "{}".format(V2) + "," + "{}".format(V3) + "," + "{}".format(V4) + ",0," + "{}".format(V5) + "," + "{}".format(V6) + "," + "{}".format(V7) + "," + "{}".format(V8) + "," + "{}".format(V9) + "," + "{}".format(V10)
		if int("{}".format(type)) == 5:
			output("Deleting remote trigger")
			stg = "CA," + "{}".format(V0) + "," + "{}".format(V1) + ",0,0,0,0,0,0,0,0,0,0"	
		if int("{}".format(type)) == 6:
			output("Set time")
			stg = "CT," + "{}".format(V0) + "," + "{}".format(V1) + "," + "{}".format(V2) + "," + "{}".format(V3) + "," + "{}".format(V4) + "," + "{}".format(V5) 
		if int("{}".format(type)) == 7:
			output("Set meteo")
			stg = "CM," + "{}".format(V0) + "," + "{}".format(V1) + "," + "{}".format(V2) + "," + "{}".format(V3) + "," + "{}".format(V4) 
		if int("{}".format(type)) == 8:
			output("Set meteo forecast")
			stg = "CF," + "{}".format(V0) + "," + "{}".format(V1) + "," + "{}".format(V2) + "," + "{}".format(V3) + "," + "{}".format(V4) 
		if int("{}".format(type)) == 9:
			output("Set meteo forecast temperature")
			stg = "CG," + "{}".format(V0) + "," + "{}".format(V1) + "," + "{}".format(V2) + "," + "{}".format(V3) + "," + "{}".format(V4) + "," + "{}".format(V5) + "," + "{}".format(V6) + "," + "{}".format(V7) + "," + "{}".format(V8) 				
		if int("{}".format(type)) == 10:
			output("Set smartlight commands")
			#V0 node
			#V1 pin
			#V2 sts
			#V3 value
			#V4 color/mood
			stg = "CC," + "{}".format(V0) + "," + "{}".format(V1) + "," + "{}".format(V2) + "," + "{}".format(V3) + "," + "{}".format(V4)
		if int("{}".format(type)) == 11:
			output("Setup smartlight configuration (all device type)")
			#V0 node
			#V1 pin
			#V2 type
			#V3 mode 0=insert 1=delete 2=update
			stg = "CD," + "{}".format(V0) + "," + "{}".format(V1) + "," + "{}".format(V2) + "," + "{}".format(V3)  								
		if int("{}".format(type)) == 12:
			output("Setup smartlight configuration group MOODs")
			#V0 node
			#V1 pin
			#V2 group pin
			#V3 mood1
			#V4 mood2
			#V5 ...
			stg = "CE," + "{}".format(V0) + "," + "{}".format(V1) + "," + "{}".format(V2) + "," + "{}".format(V3) + "," + "{}".format(V4) + "," + "{}".format(V5) + "," + "{}".format(V6) + "," + "{}".format(V7) + "," + "{}".format(V8) + "," + "{}".format(V9) + "," + "{}".format(V10)
		#-- common part --#	
		# send the string
		ret = serCoord.write(bytes(stg, 'UTF-8')) 
		output("String: " + stg)
		readSerial = CommNotExecuted
		initTime = time.time() + 3 #set timeout for the next loop
		while initTime > time.time(): #data available on serial
			serialBuff = serCoord.inWaiting()
			if serialBuff == 5: #if there is the response code 
				readSerial = serCoord.readline()
				break
			elif serialBuff > 5:
				return					
		if readSerial == CommExecutedTrue:
			sql = "delete from tbdataout where timekey = " + "{}".format(timekey)
			try:	
				curUpd.execute(sql)
				output ("Ok")
			except:
				raise	
		elif readSerial == CommExecutedFalse:		
			# write error in log
			log(int("{}".format(type)), "Error "+stg)
			output ("Error")
		else:	
			output ("Wrong response")
			output (readSerial)
		#time.sleep(0.3)			
	db.commit()

#------- Main section ----------------------------#
#------- Run once --------------------------------#
log("I", "Initialize coordinator")
ret = 0
while ret == 0:
	INString = "IN" # to send Node data to the coordinator
	ISString = "IS" # to send Sensor data to the coordinator
	IXString = "IX" # to send Address data to the coordinator
	IAString = "IA" # to send Actuators data to the coordinator
	IMString = "IM" # to send Methods data to the coordinator
	ret = initCoordinator()
#------- End run once    -------------------------#
log("I", "Start main loop")

#------- Start main loop -------------------------#
while True:
    # controllo se ci sono segnalazioni da arduino
	#checkInit()
	Controller()
	time.sleep(0.1)
#------- End main loop -------------------------#
  


