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
	
	try:	
		f = urlopen('http://api.wunderground.com/api/2b8816d29bb95b21/forecast/q/IT/Firenze.json')
		json_string = f.read()
		parsed_json = json.loads(json_string.decode())
		forecast_day_1 = parsed_json['forecast']['simpleforecast']['forecastday'][0]['icon']
		forecast_day_2 = parsed_json['forecast']['simpleforecast']['forecastday'][1]['icon']
		forecast_day_3 = parsed_json['forecast']['simpleforecast']['forecastday'][2]['icon']
		forecast_day_4 = parsed_json['forecast']['simpleforecast']['forecastday'][3]['icon']
		t1min = int(parsed_json['forecast']['simpleforecast']['forecastday'][0]['low']['celsius'])
		if (t1min < 0):
			t1min = abs(t1min)+9000
		t1max = int(parsed_json['forecast']['simpleforecast']['forecastday'][0]['high']['celsius'])
		if (t1max < 0):
			t1max = abs(t1max)+9000
		t2min = int(parsed_json['forecast']['simpleforecast']['forecastday'][1]['low']['celsius'])
		if (t2min < 0):
			t2min = abs(t2min)+9000
		t2max = int(parsed_json['forecast']['simpleforecast']['forecastday'][1]['high']['celsius'])
		if (t2max < 0):
			t2max = abs(t2max)+9000
		t3min = int(parsed_json['forecast']['simpleforecast']['forecastday'][2]['low']['celsius'])
		if (t3min < 0):
			t3min = abs(t3min)+9000
		t3max = int(parsed_json['forecast']['simpleforecast']['forecastday'][2]['high']['celsius'])
		if (t3max < 0):
			t3max = abs(t3max)+9000
		t4min = int(parsed_json['forecast']['simpleforecast']['forecastday'][3]['low']['celsius'])
		if (t4min < 0):
			t4min = abs(t4min)+9000
		t4max = int(parsed_json['forecast']['simpleforecast']['forecastday'][3]['high']['celsius'])
		if (t4max < 0):
			t4max = abs(t4max)+9000
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
			sql = "SELECT DISTINCT id FROM tbmeteo WHERE icon = '" + forecast_day_1 + "'"
			cur.execute(sql)
			for (id) in cur:
				frcst1icon = id[0]
			sql = "SELECT DISTINCT id FROM tbmeteo WHERE icon = '" + forecast_day_2 + "'"
			cur.execute(sql)
			for (id) in cur:
				frcst2icon = id[0]				
			sql = "SELECT DISTINCT id FROM tbmeteo WHERE icon = '" + forecast_day_3 + "'"
			cur.execute(sql)
			for (id) in cur:
				frcst3icon = id[0]
			sql = "SELECT DISTINCT id FROM tbmeteo WHERE icon = '" + forecast_day_4 + "'"
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
	
