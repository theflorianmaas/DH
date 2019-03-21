#!/usr/bin/python
# -*- coding: utf-8 -*-
# Ver 16 - 23 March 2017 -

from urllib.request import *
import json
import time
import mysql.connector
from mysql.connector import errorcode
import string
import sys
import datetime
from db import * 

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
cnt = 10

# -- get meteo parameters ------------------------#
sql = "SELECT api, location, url FROM tbapi WHERE tbapi.service = 'weather'"
cur.execute(sql)
row = cur.fetchone()
if row:
	api=row[0].decode("utf-8")
	location=row[1].decode("utf-8")
	url=row[2].decode("utf-8")

def getTemp():
	global temp_c
	global relative_humidity
	global pressure
	global meteo
	global api
	global location
	global url
	try:	
		f = urlopen(url+"&id="+location+"&appid="+api)
		json_string = f.read()
		dataJSON = json.loads(json_string.decode())
		
		temp_c = float(dataJSON['main']['temp'])
		relative_humidity = int(dataJSON['main']['humidity'])
		meteo = dataJSON['weather'][0]['id']
		pressure = dataJSON['main']['pressure']
		f.close()
		print(temp_c)
		print(relative_humidity)
		print(meteo)
		return "Ok"
	except: 
		return "error"

	
def parseint(string):
    return int(''.join([x for x in string if x.isdigit()]))		

   
while True:
	global temp_c
	global relative_humidity
	global pressure
	global meteo
	ret = getTemp()
	if (ret == "Ok"):
		try:
			sql = "UPDATE tbsensor SET currentvalue = %s , lastupdate = '%s' where pin_number = '%s'"  % (temp_c, datetime.datetime.now(),'1000')
			cur.execute(sql)
			cur.execute("commit")
			output ("Temperatura corrente "+str(temp_c)+" Celsius")
			sql = "UPDATE tbsensor SET currentvalue = %s  , lastupdate = '%s' where pin_number = '%s'"  % (relative_humidity, datetime.datetime.now(), '1001')
			cur.execute(sql)
			#cur.execute("commit")
			output ("Umidita' relativa corrente "+str(relative_humidity))
			sql = "UPDATE tbsensor SET currentvalue = %s  , lastupdate = '%s' where pin_number = '%s'"  % (pressure, datetime.datetime.now(), '1002')
			cur.execute(sql)
			output ("Pressione corrente "+str(pressure))
			cur.execute("commit")
			#get meteo icon
			sql = "SELECT DISTINCT id FROM tbmeteo WHERE tbmeteo.condition = '" + str(meteo) + "'"
			cur.execute(sql)
			for (id) in cur:
				meteoid = id[0]
			sql = "SELECT id FROM tbnode WHERE sendmeteo = 1"
			cur.execute(sql)
			print ("t1:",int(temp_c))
			if int(temp_c) < 0:
				temp_c_fixed = abs(int(temp_c))+9000
			else: 	
				temp_c_fixed = int(temp_c)*100
			for (id) in cur:
				id = id[0]
				print (temp_c_fixed)
				sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4) VALUES (millis(),7,%s,%s,%s,%s,%s)" % (id, temp_c_fixed, relative_humidity, pressure, meteoid)
				cur2.execute(sql)
				db.commit()	
			output ("Meteo corrente "+str(meteo))			
		except: 
			output ("database error...")
	time.sleep(300)
	
