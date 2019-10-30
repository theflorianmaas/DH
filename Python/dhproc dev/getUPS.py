#!/usr/bin/python
# -*- coding: utf-8 -*-
# Ver 16 - 28 July 2019 -

from urllib.request import *
import json
import time
import mysql.connector
from mysql.connector import errorcode
import string
import sys
import datetime
from db import * 
import threading
#from threading import Thread
import multiprocessing as mp
from multiprocessing import Queue
from multiprocessing.managers import SyncManager
from apcaccess import status as apc

HOST = ''
PORT0 = 5011
PORT1 = 5012
PORT2 = 5013
PORT3 = 5014
PORT4 = 5015
PORT5 = 5016
AUTHKEY = str("123456").encode("utf-8")

CMDPREFIX = "9"

def output(o, x):
	print(str(str(o) + " " + str(datetime.datetime.now().time())[:8]) + " "+ str(x))
	sys.stdout.flush()
	
def QueueServerClient(HOST, PORT, AUTHKEY):
    class QueueManager(SyncManager):
        pass
    QueueManager.register('get_queue')
    QueueManager.register('get_name')
    QueueManager.register('get_description')
    manager = QueueManager(address = (HOST, PORT), authkey = AUTHKEY)
    manager.connect() # This starts the connected client
    return manager	
	
# -- DB Connection ---------------------------
try:
  db = mysql.connector.connect(**config)
except mysql.connector.Error as err:
  if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
    output("DB", "Something is wrong with your user name or password")
  elif err.errno == errorcode.ER_BAD_DB_ERROR:
    output("DB", "Database does not exists")
  else:
    output("DB", err)
else:
  output("getUPS", "Start procedure")
  db.commit()
# -- END DB Connection ---------------------------

curM = db.cursor()
curF = db.cursor()
curM2 = db.cursor()
curF2 = db.cursor()
curU = db.cursor()
cnt = 10

def printDBError(x, e):
	output(x, "Error: " +  str(e))             # errno, sqlstate, msg values
	
def parseint(string):
    return int(''.join([x for x in string if x.isdigit()]))		
	
def execUps():
	global qSql
	#output ("UPS","Exec UPS")
	OrderedDict = apc.parse(apc.get(), strip_units=True)
	status = OrderedDict['STATUS']
	if status == 'ONLINE' or status == 'CHARGING':
		vstatus = 1
		bcharge = int(float(OrderedDict['BCHARGE']))
		battv = float(OrderedDict['BATTV'])
		timeleft = float(OrderedDict['TIMELEFT'])
		linev = int(float(OrderedDict['LINEV']))
	elif status == 'ONBATT':
		vstatus = 0	
		bcharge = int(float(OrderedDict['BCHARGE']))
		battv = float(OrderedDict['BATTV'])
		timeleft = float(OrderedDict['TIMELEFT'])
		linev = int(float(OrderedDict['LINEV']))
	elif status == 'COMMLOST':
		vstatus = 99
		bcharge = 0
		battv = 0.0
		timeleft = 0.0
		linev = 0
	else:
		vstatus = 999
		bcharge = 0
		battv = 0.0
		timeleft = 0.0
		linev = 0				
	
	sql = CMDPREFIX + "INSERT INTO tbdatain (timekey,type,v0,v1,v2) values (millis(),1,%s,%s,%s)" % ('8','32',bcharge)
	sql = sql + ",(millis(),1,%s,%s,%s)" % ('8','33',vstatus)
	sql = sql + ",(millis(),1,%s,%s,%s)" % ('8','34',battv)
	sql = sql + ",(millis(),1,%s,%s,%s)" % ('8','30',timeleft)
	sql = sql + ",(millis(),1,%s,%s,%s)" % ('8','31',linev)
	qSql.put(sql)	
	output ("UPS","Aggiornamento UPS fatto")
	timerUps = threading.Timer(30.0, execUps).start()


#------- Main section ----------------------------#
#------- Run once --------------------------------#
# create three connected managers
qmSql = QueueServerClient(HOST, PORT2, AUTHKEY)
# Get the queue objects from the clients
qSql = qmSql.get_queue()	
		
timerUps = threading.Timer(1.0, execUps).start()
#------- End run once    -------------------------#
		
while True:
	a = 0
	
