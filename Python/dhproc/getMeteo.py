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

def getTemp():
	global temp_c
	global relative_humidity
	global pressure
	global meteo
	try:	
		f = urlopen('http://api.wunderground.com/api/2b8816d29bb95b21/conditions/q/IT/Firenze.json')
		json_string = f.read()
		parsed_json = json.loads(json_string.decode())
		temp_c = parsed_json['current_observation']['temp_c']
		relative_humidity = parsed_json['current_observation']['relative_humidity']
		relative_humidity = parseint(relative_humidity)
		str_pressure = parsed_json['current_observation']['pressure_mb']
		pressure = int(float(str_pressure))
		meteo = parsed_json['current_observation']['icon']
		f.close()
		return "Ok"
	except: 
		return "error"

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
			t1min = abs(t1min)+900
		t1max = int(parsed_json['forecast']['simpleforecast']['forecastday'][0]['high']['celsius'])
		if (t1max < 0):
			t1max = abs(t1max)+900
		t2min = int(parsed_json['forecast']['simpleforecast']['forecastday'][1]['low']['celsius'])
		if (t2min < 0):
			t2min = abs(t2min)+900
		t2max = int(parsed_json['forecast']['simpleforecast']['forecastday'][1]['high']['celsius'])
		if (t2max < 0):
			t2max = abs(t2max)+900
		t3min = int(parsed_json['forecast']['simpleforecast']['forecastday'][2]['low']['celsius'])
		if (t3min < 0):
			t3min = abs(t3min)+900
		t3max = int(parsed_json['forecast']['simpleforecast']['forecastday'][2]['high']['celsius'])
		if (t3max < 0):
			t3max = abs(t3max)+900
		t4min = int(parsed_json['forecast']['simpleforecast']['forecastday'][3]['low']['celsius'])
		if (t4min < 0):
			t4min = abs(t4min)+900
		t4max = int(parsed_json['forecast']['simpleforecast']['forecastday'][3]['high']['celsius'])
		if (t4max < 0):
			t4max = abs(t4max)+900
		f.close()
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
			#cur.execute("commit")
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
			sql = "SELECT DISTINCT id FROM tbmeteo WHERE icon = '" + meteo + "'"
			cur.execute(sql)
			for (id) in cur:
				meteoid = id[0]
			sql = "SELECT id FROM tbnode WHERE sendmeteo = 1"
			cur.execute(sql)
			for (id) in cur:
				id = id[0]
				temp_c = int(temp_c*100)
				sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4) VALUES (millis(),7,%s,%s,%s,%s,%s)" % (id, temp_c, relative_humidity, pressure, meteoid)
				cur2.execute(sql)
				db.commit()	
			output ("Meteo corrente "+str(meteo))			
		except: 
			output ("database error...")
	time.sleep(300)
	
