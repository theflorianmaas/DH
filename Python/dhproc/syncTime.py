#!/usr/bin/python
# -*- coding: utf-8 -*-
# Ver 16 - 26 March 2017 -

import time
import mysql.connector
from mysql.connector import errorcode
import string
import datetime
from db import * 
import sys

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

cur = db.cursor()
cur2 = db.cursor()

sql = "SELECT id FROM tbnode WHERE sendtime = 1 AND tbstatus_id = 1 AND currentstatus = 1 "
cur.execute(sql)

id = list(cur.fetchall())

while True:
	for row in id:
		try:
			output ("Synchronizing time")
			sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,v5) VALUES (millis(),6," + str(row[0] ) + ",DAY(NOW()),MONTH(NOW()),YEAR(NOW()),HOUR(NOW()),MINUTE(NOW()))"
			cur2.execute(sql)
			db.commit()	
			output ("Time synchronized")
		except mysql.connector.Error as err:
			output ("database error... " + proc + "- " + str(err))	
			db.commit()			
	time.sleep(1200)	
	


		


