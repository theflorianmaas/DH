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

def output(x):
	print(str(datetime.datetime.now().time())[:8]+ " "+ str(x))
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

 
#----------------------------- 
# Global Variable declaration  
#----------------------------- 

cur = db.cursor()
cnt = 0

# inizializza timers
t = [1, 1, 1, 1, 1]
t[0] = time.time() #timer0
t[1] = time.time() #timer1
t[2] = time.time() #timer2
t[3] = time.time() #timer3
t[4] = time.time() #timer4

tx = [1, 1, 1, 1, 1]
tx[0] = 2  #controllo parametri ogni x secondi da t0
tx[1] = 10 #controllo parametri ogni x secondi da t1
tx[2] = 10 #controllo parametri ogni x secondi da t2
tx[3] = 10 #controllo parametri ogni x secondi da t3
tx[4] = 10 #controllo parametri ogni x secondi da t4
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
	output (now.strftime("%H %M %S %f"))	
	
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
	# end check Init
	

def init():
	global t
	global tx
	#global limit

	# set timers 
	sql = "SELECT pvalue,pindex FROM tbparam WHERE ptype = 'T' ORDER BY pindex" 
	cur.execute(sql)
		
	for (PVALUE,PINDEX) in cur:
		i = int("{}".format(PINDEX))
		v = int("{}".format(PVALUE))
		if tx[i] != v:
					tx[i] = v
					output ("Set timer tx[" +str(i)+"] to "+ str(v))
	db.commit()
	cur.close
	# end set timers

#------- Main section ----------------------------#
init()
#------- Start main loop -------------------------#
while True:
    # controllo se ci sono segnalazioni da arduino
	time.sleep(0.1)
	if time.time()-t[0] > tx[0]:
		checkInit()
		t[0] = time.time()
	# add here code to execute at timer 1
	if time.time()-t[1] > tx[1]:
		arg = [3]
		try:
			output("Execute HistoryData 1")
			cur.callproc('updateHistDataTable',arg)
			#printTime()
			t[1] = time.time()
		except DatabaseError:
			output(DatabaseError)
	# add here code to execute at timer 2
	if time.time()-t[2] > tx[2]:
		arg = [4]
		try:
			output("Execute HistoryData 2")
			cur.callproc('updateHistDataTable',arg)
			#printTime()
			t[2] = time.time()
		except DatabaseError:
			output(DatabaseError)
	# add here code to execute at timer 3		
	if time.time()-t[3] > tx[3]:
		arg = [5]
		try:
			output("Execute HistoryData 3")
			cur.callproc('updateHistDataTable',arg)
			#printTime()
			t[3] = time.time()
		except DatabaseError:
			output(DatabaseError)
	# add here code to execute at timer 4		
	if time.time()-t[4] > tx[4]:
		arg = [6]
		try:
			output("Execute HistoryData 4")
			cur.callproc('updateHistDataTable',arg)
			#printTime()
			t[4] = time.time()
		except DatabaseError:
			output(DatabaseError)		
#------- End main loop -------------------------#
  



