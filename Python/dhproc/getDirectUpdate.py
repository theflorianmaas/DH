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
from apcaccess import status as apc

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
	apiM=row[0]
	locationM=row[1]
	urlM=row[2]	
	
# -- get forecast parameters ------------------------#
sql = "SELECT api, location, url FROM tbapi WHERE tbapi.service = 'forecast'"
cur.execute(sql)
row = cur.fetchone()
if row:
	apiF=row[0]
	locationF=row[1]
	urlF=row[2]	
	

def getTemp():
	global temp_c
	global relative_humidity
	global pressure
	global meteo
	global api
	global location
	global url
	try:	
		f = urlopen(urlM+"&id="+locationM+"&appid="+apiM)
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
		f = urlopen(urlF+"&id="+locationF+"&appid="+apiF)
		json_string = f.read()
		dataJSON = json.loads(json_string.decode())
		now = datetime.datetime.now()
		hour = now.hour
				
		print(hour)
		print(dataJSON['list'][phases[hour]+12]['dt_txt'])
		#print(dataJSON['list'][phases[hour]+12]['main']['temp'])
		#print(dataJSON['list'][phases[hour]+12]['main']['temp'])
		#print(dataJSON['list'][phases[hour]+20]['main']['temp'])
		#print(dataJSON['list'][phases[hour]+28]['main']['temp'])
		
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


def execForecast():
	output ("Exec Forecast")
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
	timerForecast = threading.Timer(1800.0, execForecast).start()	
   
def execMeteo():
	output ("Exec Meteo")
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
	timerMeteo = threading.Timer(300.0, execMeteo).start()
	
def execUps():
	output ("Exec UPS")
	OrderedDict = apc.parse(apc.get(), strip_units=True)
	bcharge = int(float(OrderedDict['BCHARGE']))
	status = OrderedDict['STATUS']
	battv = float(OrderedDict['BATTV'])
	timeleft = float(OrderedDict['TIMELEFT'])
	linev = int(float(OrderedDict['LINEV']))

	if status == 'ONLINE':
		vstatus = 1
	else:
		vstatus = 0	
	
	try:
		sql = "UPDATE tbsensor SET currentvalue = %s , lastupdate = '%s' where pin_number = '%s'"  % (bcharge, datetime.datetime.now(),'27')
		#print(sql)
		cur.execute(sql)
		cur.execute("commit")
		
		sql = "UPDATE tbsensor SET currentvalue = %s , lastupdate = '%s' where pin_number = '%s'"  % (vstatus, datetime.datetime.now(),'28')
		#print(sql)
		cur.execute(sql)
		cur.execute("commit")
		
		sql = "UPDATE tbsensor SET currentvalue = %s , lastupdate = '%s' where pin_number = '%s'"  % (battv, datetime.datetime.now(),'29')
		#print(sql)
		cur.execute(sql)
		cur.execute("commit")
		
		sql = "UPDATE tbsensor SET currentvalue = %s , lastupdate = '%s' where pin_number = '%s'"  % (timeleft, datetime.datetime.now(),'30')
		#print(sql)
		cur.execute(sql)
		cur.execute("commit")
		
		sql = "UPDATE tbsensor SET currentvalue = %s , lastupdate = '%s' where pin_number = '%s'"  % (linev, datetime.datetime.now(),'31')
		#print(sql)
		cur.execute(sql)
		cur.execute("commit")
			
	except: 
		output ("database error...")
	timerUps = threading.Timer(30.0, execUps).start()

timerForecast = threading.Timer(10.0, execForecast).start()	
timerMeteo = threading.Timer(1.0, execMeteo).start()
timerUps = threading.Timer(3.0, execUps).start()
		

while True:
	a = 0
	
