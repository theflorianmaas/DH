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
sql = "SELECT api, location, url FROM tbapi WHERE tbapi.service = 'forecast'"
cur.execute(sql)
row = cur.fetchone()
if row:
	api=row[0].decode("utf-8")
	location=row[1].decode("utf-8")
	url=row[2].decode("utf-8")
	

def getForecast():
	global forecast_day_1
	global forecast_day_2
	global forecast_day_3
	global forecast_day_4
	global t1min
	global t1max
	global t2min
	global t2max
	global t3min
	global t3max
	global t4min
	global t4max
	global api
	global location
	global url
	
	try:	
		phases = {	
			0 : 7,
			1 : 7,
			2 : 7,
			3 : 6,
			4 : 6,
			5 : 6,
			6 : 5,
			7 : 5,
			8 : 5,
			9 : 4,
			10: 4,
			11: 4,
			12: 3,
			13: 3,
			14: 3,
			15: 2,
			16: 2,
			17: 2,
			18: 1,
			19: 1,
			20: 1,
			21: 0,
			22: 0,
			23: 0		
		} 
		f = urlopen(url+"&id="+location+"&appid="+api)
		json_string = f.read()
		dataJSON = json.loads(json_string.decode())
		now = datetime.datetime.now()
		hour = now.hour
				
		print(hour)
		print(dataJSON['list'][phases[hour]+12]['dt_txt'])
		print(dataJSON['list'][phases[hour]+12]['main']['temp'])
		print(dataJSON['list'][phases[hour]+12]['main']['temp'])
		print(dataJSON['list'][phases[hour]+20]['main']['temp'])
		print(dataJSON['list'][phases[hour]+28]['main']['temp'])
		
		forecast_day_1 = dataJSON['list'][phases[hour]+4]['weather'][0]['id']
		forecast_day_2 = dataJSON['list'][phases[hour]+12]['weather'][0]['id']
		forecast_day_3 = dataJSON['list'][phases[hour]+20]['weather'][0]['id']
		forecast_day_4 = dataJSON['list'][phases[hour]+28]['weather'][0]['id']
		t1min = int(dataJSON['list'][phases[hour]+2]['main']['temp'])
		if (t1min < 0):
			t1min = abs(t1min)+9000
		t1max = int(dataJSON['list'][phases[hour]+5]['main']['temp'])
		if (t1max < 0):
			t1max = abs(t1max)+9000
		t2min = int(dataJSON['list'][phases[hour]+10]['main']['temp_min'])
		if (t2min < 0):
			t2min = abs(t1min)+9000
		t2max = int(dataJSON['list'][phases[hour]+13]['main']['temp_max'])
		if (t2max < 0):
			t2max = abs(t1max)+9000
		t3min = int(dataJSON['list'][phases[hour]+18]['main']['temp_min'])
		if (t3min < 0):
			t3min = abs(t1min)+9000
		t3max = int(dataJSON['list'][phases[hour]+21]['main']['temp_max'])
		if (t3max < 0):
			t3max = abs(t1max)+9000
		t4min = int(dataJSON['list'][phases[hour]+26]['main']['temp_min'])
		if (t4min < 0):
			t4min = abs(t1min)+9000
		t4max = int(dataJSON['list'][phases[hour]+28]['main']['temp_max'])
		if (t4max < 0):
			t4max = abs(t1max)+9000			

		f.close()
		return "Ok"
	except: 
		return "error"	

		
def parseint(string):
    return int(''.join([x for x in string if x.isdigit()]))		

   
while True:
	ret = getForecast()
	if (ret == "Ok"):
		try:
			#get meteo icon
			sql = "SELECT DISTINCT id FROM tbmeteo WHERE tbmeteo.condition = '" + str(forecast_day_1) + "'"
			cur.execute(sql)
			for (id) in cur:
				frcst1icon = id[0]
			sql = "SELECT DISTINCT id FROM tbmeteo WHERE tbmeteo.condition = '" + str(forecast_day_2) + "'"
			cur.execute(sql)
			for (id) in cur:
				frcst2icon = id[0]				
			sql = "SELECT DISTINCT id FROM tbmeteo WHERE tbmeteo.condition = '" + str(forecast_day_3) + "'"
			cur.execute(sql)
			for (id) in cur:
				frcst3icon = id[0]
			sql = "SELECT DISTINCT id FROM tbmeteo WHERE tbmeteo.condition = '" + str(forecast_day_4) + "'"
			cur.execute(sql)
			for (id) in cur:
				frcst4icon = id[0]										
			sql = "SELECT id FROM tbnode WHERE sendmeteo = 1"
			cur.execute(sql)
			for (id) in cur:
				id = id[0]
			sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4) VALUES (millis(),8,%s,%s,%s,%s,%s)" % (id, frcst1icon, frcst2icon, frcst3icon, frcst4icon)
			cur2.execute(sql)
			time.sleep(5)
			sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,V5,V6,V7,V8) VALUES (millis(),9,%s,%s,%s,%s,%s,%s,%s,%s,%s)" % (id, t1min, t1max, t2min, t2max, t3min, t3max, t4min, t4max)
			#print (sql)
			cur2.execute(sql)
			db.commit()	
			output ("Forecast sent")					
		except: 
			output ("database error...")	
	time.sleep(1800)
	
