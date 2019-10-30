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
    output("Something is wrong with your user name or password")
  elif err.errno == errorcode.ER_BAD_DB_ERROR:
    output("Database does not exists")
  else:
    output(err)
else:
  output("P_CMD_IN","Start procedure")
  db.commit()
# -- END DB Connection ---------------------------

#----------------------------- 
# Global Variable declaration  
#----------------------------- 
 
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
	
def execSQL(qSQL):
	cur = db.cursor()
	while True:
		#try:
		if not qSQL.empty():
			sql = str(qSQL.get())
			if sql[:1] == "0":
				sql = sql[1:]
				inssql = "UPDATE tbqueue SET timekey = millis(), code = '" + sql + "' WHERE id = 1"
			else: # = 9
				sql = sql[1:]
				print("comando")
				inssql = "INSERT INTO tbqueuecommand (timekey, code) VALUES (millis(), '" + sql + "')"

			try:
				db.start_transaction(False, None, False)
				cur.execute(inssql)
				db.commit()
			except mysql.connector.Error as e:
				printDBError("P_CMD_IN", e)	
		time.sleep(0.2)	
			

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
execSQL(qSql)




	
    


