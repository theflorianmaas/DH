#!/usr/bin/python
# -*- coding: utf-8 -*-
# Ver 16 - 23 March 2017 -

import time
import mysql.connector
from mysql.connector import errorcode
import string
import datetime
from db import * 
import sys

def output(o, x):
	print(str(str(o) + " " + str(datetime.datetime.now().time())[:8]) + " "+ str(x))
	sys.stdout.flush()
	
# -- DB Connection ---------------------------
try:
  db = mysql.connector.connect(**config)
except mysql.connector.Error as err:
  if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
    output("SCHEDULER", "Something is wrong with your user name or password")
  elif err.errno == errorcode.ER_BAD_DB_ERROR:
    output("SCHEDULER", "Database does not exists")
  else:
    output(err)
else:
  output("SCHEDULER", "Start procedure")
# -- END DB Connection ---------------------------

cur = db.cursor()

sql = "SELECT sts, proc FROM tbproctl WHERE id = " + str(sys.argv[1] + " LIMIT 1")
cur.execute(sql)
row = cur.fetchone()
#proc = row[1].decode("utf-8")
proc = row[1]

while True:
	try:
		inittime = time.time()
		#output ("SCHEDULER", "START: " + proc)
		cur.callproc(proc)
		passedtime = time.time() - inittime 
		if passedtime > 7:
			output ("SCHEDULER", "PROCEDURA LENTA: " + proc + " " + str(passedtime))
	except mysql.connector.Error as err:
		output ("SCHEDULER", "database error... " + proc + " - " + str(err))	
	db.commit()	



