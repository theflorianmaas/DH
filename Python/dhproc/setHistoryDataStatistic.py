#!/usr/bin/python
# -*- coding: utf-8 -*-
# Ver 16 - 23 March 2017 -

import time
import string
import sys
import mysql.connector
from mysql.connector import errorcode
from db import * 
import datetime

def output(o, x):
	print(str(str(o) + " " + str(datetime.datetime.now().time())[:8]) + " "+ str(x))
	sys.stdout.flush()

# -- DB Connection ---------------------------
try:
  db = mysql.connector.connect(**config)
except mysql.connector.Error as err:
  if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
    output("SetHistoryDataStatistic", "Something is wrong with your user name or password")
  elif err.errno == errorcode.ER_BAD_DB_ERROR:
    output("SetHistoryDataStatistic","Database does not exists")
  else:
    output(err)
else:
  output("SetHistoryDataStatistic","Start procedure")
# -- END DB Connection ---------------------------
 
#----------------------------- 
# Global Variable declaration  
#----------------------------- 

cur = db.cursor()
cnt = 0

# inizializza timers
t = [1]
t[0] = time.time() #timer0

tx = [1]
tx[0] = 7200  #controllo parametri ogni x secondi da t0
# end timers

#----------------------------- 
# End Global Variable declaration  
#-----------------------------  

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
	output ("SetHistoryDataStatistic",now.strftime("%H %M %S %f"))	
	
#------- Main section ----------------------------#
try:
	output("SetHistoryDataStatistic","Executing HistoryData Statistic")
	cur.callproc('updateHistDataTableStatistic')
	t[0] = time.time()
except mysql.connector.Error as err:
	output(err)
	t[0] = time.time()	
#------- Start main loop -------------------------#
while True:
	time.sleep(2.0)
	# add here code to execute at timer 5 - execute every 24h	
	if time.time()-t[0] > tx[0]:
		try:
			output("SetHistoryDataStatistic","Executing HistoryData Statistic")
			cur.callproc('updateHistDataTableStatistic')
			t[0] = time.time()
		except mysql.connector.Error as err:
			output(err)
			t[0] = time.time()	
#------- End main loop -------------------------#
  



