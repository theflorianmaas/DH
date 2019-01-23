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

from os import system, devnull
from subprocess import call, STDOUT
from threading import Thread
from time import sleep 
#import queue

ctrlStr = "*../"

HOST = ''
PORT0 = 5011
PORT1 = 5012
PORT2 = 5013
PORT3 = 5014
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

# Gpio pin manager
class Gpio:
    def __init__(self):
        self.gpios = ["55", "57"]
        self.gpioval = [0, 0]
        self.gpiodir = [0, 0]
        self.current = 0
        self.OUTPUT = 1
        self.INPUT = 0
        self.HIGH = 1
        self.LOW = 0
        for num in self.gpios:
            try:
                with open("/sys/class/gpio/export", "w") as create:
                    create.write(num)
                with open("/sys/class/gpio/gpio" + self.gpios[current] + "/value", "r") as reads:
                    self.gpioval[self.current] = reads.read()
                with open("/sys/class/gpio/gpio" + self.gpios[current] + "/direction", "r") as readdir:
                    self.gpiodir[self.current] = (1 if "out" in readdir.read() else 0)
                self.current += 1
            except:
                sleep(0.000001)

    def pinMode(self, pin=0, direction=0):
        try:
            gpio = self.gpios[int(pin)]
            if int(direction) != self.gpiodir[pin]:
                with open("/sys/class/gpio/gpio" + gpio + "/direction", "w") as writer:
                    writer.write("in" if direction < 1 else "out")
                self.gpiodir[pin] = (0 if direction < 1 else 1)
            return True
        except ValueError:
            print ("ERROR: pinMode, value inserted wasn't an int")
            return False
        except:
            print ("ERROR: pinMode, error using pinMode")
            return False

    def digitalWrite(self, pin=2, value=0):
        try:
            gpio = self.gpios[int(pin)]
            if self.gpiodir[pin] != 1:
                with open("/sys/class/gpio/gpio" + gpio + "/direction", "w") as re:
                    re.write("out")
                self.gpiodir[pin] = 1
            if self.gpioval[pin] != int(value):
                with open("/sys/class/gpio/gpio" + gpio + "/value", "w") as writes:
                    writes.write("0" if value < 1 else "1")
                self.gpioval[pin] = (0 if value < 1 else 1)
            return True
        except ValueError:
            print ("ERROR: digitalWrite, value inserted wasn't an int")
            return False
        except:
            print ("ERROR: digitalWrite, error running")
            return False

    def digitalRead(self, pin=2):
        try:
            gpio = self.gpios[int(pin)]
            if self.gpiodir[pin] != 0:
                with open("/sys/class/gpio/gpio" + gpio + "/direction", "w") as re:
                    re.write("in")
                self.gpiodir[pin] = 0
            with open("/sys/class/gpio/gpio" + gpio + "/value", "r") as reader:
                self.gpioval[pin] = int(reader.read().replace('\n', ''))
            return self.gpioval[pin]
        except ValueError:
            print ("ERROR: digitalRead, value inserted wasn't an int")
            return -1
        except:
            print ("ERROR: digitalRead, error running")
            return -1


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
#---- get serial incoming data ---------------------------------------------------------------------#
#--------------------------------------------------------------------------------------------------------#
def getSerialData(qIN, qOUT, qResponse):
	print("init serial")
	serCoord.flushInput()
	readSerial = ""
	serCoord.timeout = 1
	while True:
		#output("Waiting for data on serial")
		gpio.digitalWrite(0,gpio.LOW) #write high value to pin
		serialBuffer = serCoord.inWaiting()
		if serialBuffer > 0: #data available on serial
			gpio.digitalWrite(0,gpio.HIGH)
			readSerial = serCoord.readline()
			readSerial.rstrip(endSerialChars)
			#output("Data received from serial")
			if isResponse(readSerial) == True:
			#	while not qResponse.empty():
			#		qResponse.get()
			#qResponse.put(readSerial)
				#output("Response received")	
				aa=1		
			else:	
				qIN.put(readSerial)	
				print("Data received:", serialBuffer)
				#print("Q size:", qIn.qsize()) 	

		while not qOUT.empty():
			print("Q OUT size:", qOUT.qsize()) 
			stg = qOUT.get()
			serCoord.write(bytes(stg, 'UTF-8')) 
			output("String sent: " + str(stg))


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
log("I", "Initialize coordinator")
gpio = Gpio()
gpio.pinMode(0, gpio.OUTPUT)
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
# Get the queue objects from the clients
qIn = qmIn.get_queue()
qOut = qmOut.get_queue()
qSql = qmSql.get_queue()	
qResp = qmResp.get_queue()		
while ret == 0:
	INString = "IN" # to send Node data to the coordinator
	ISString = "IS" # to send Sensor data to the coordinator
	IXString = "IX" # to send Address data to the coordinator
	IAString = "IA" # to send Actuators data to the coordinator
	IMString = "IM" # to send Methods data to the coordinator
	ret = initCoordinator()
	ret = 1
	
#------- End run once    -------------------------#
log("I", "Start main loop")
getSerialData(qIn, qOut, qResp)	

	
    


