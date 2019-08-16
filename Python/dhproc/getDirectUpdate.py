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

def output(o, x):
	print(str(str(o) + " " + str(datetime.datetime.now().time())[:8]) + " "+ str(x))
	sys.stdout.flush()
	
# -- DB Connection ---------------------------
try:
  db = mysql.connector.connect(**config)
except mysql.connector.Error as err:
  if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
    output("DB", "Something is wrong with your user name or password")
  elif err.errno == errorcode.ER_BAD_DB_ERROR:
    output("DB", "Database does not exists")
  else:
    output("DB", err)
else:
  output("getDirectUpdate", "Start procedure")
# -- END DB Connection ---------------------------

curM = db.cursor()
curF = db.cursor()
curM2 = db.cursor()
curF2 = db.cursor()
curU = db.cursor()
cnt = 10

# -- get meteo parameters ------------------------#
sql = "SELECT api, location, url FROM tbapi WHERE tbapi.service = 'weather'"
curM.execute(sql)
row = curM.fetchone()
if row:
	apiM=row[0]
	locationM=row[1]
	urlM=row[2]	
	
# -- get forecast parameters ------------------------#
sql = "SELECT api, location, url FROM tbapi WHERE tbapi.service = 'forecast'"
curF.execute(sql)
row = curF.fetchone()
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
		#print(temp_c)
		#print(relative_humidity)
		#print(meteo)
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
				
		#print(hour)
		#print(dataJSON['list'][phases[hour]+12]['dt_txt'])
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
	output ("Meteo Forecast", "Exec Forecast")
	ret = getForecast()
	if (ret == "Ok"):
		try:
			#get meteo icon
			sql = "SELECT DISTINCT id FROM tbmeteo WHERE tbmeteo.condition = '" + str(forecast_day_1) + "'"
			curF.execute(sql)
			for (id) in curF:
				frcst1icon = id[0]
			sql = "SELECT DISTINCT id FROM tbmeteo WHERE tbmeteo.condition = '" + str(forecast_day_2) + "'"
			curF.execute(sql)
			for (id) in curF:
				frcst2icon = id[0]				
			sql = "SELECT DISTINCT id FROM tbmeteo WHERE tbmeteo.condition = '" + str(forecast_day_3) + "'"
			curF.execute(sql)
			for (id) in curF:
				frcst3icon = id[0]
			sql = "SELECT DISTINCT id FROM tbmeteo WHERE tbmeteo.condition = '" + str(forecast_day_4) + "'"
			curF.execute(sql)
			for (id) in curF:
				frcst4icon = id[0]										
			sql = "SELECT id FROM tbnode WHERE sendmeteo = 1"
			curF.execute(sql)
			for (id) in curF:
				id = id[0]
			sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4) VALUES (millis(),8,%s,%s,%s,%s,%s)" % (id, frcst1icon, frcst2icon, frcst3icon, frcst4icon)
			curF2.execute(sql)
			time.sleep(5)
			sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4,V5,V6,V7,V8) VALUES (millis(),9,%s,%s,%s,%s,%s,%s,%s,%s,%s)" % (id, t1min, t1max, t2min, t2max, t3min, t3max, t4min, t4max)
			#print (sql)
			curF2.execute(sql)
			curF2.execute("commit")
			output ("Meteo Forecast","Forecast sent")					
		except: 
			output ("Meteo Forecast","database error...")			
	timerForecast = threading.Timer(1800.0, execForecast).start()	
   
def execMeteo():
	output ("Meteo","Exec Meteo")
	global temp_c
	global relative_humidity
	global pressure
	global meteo
	ret = getTemp()
	if (ret == "Ok"):
		try:
			sql = "INSERT INTO tbdatain (timekey,type,v0,v1,v2) values (millis(),1,%s,%s,%s)" % ('2','1000',temp_c)
			sql = sql + ",(millis(),1,%s,%s,%s)" % ('2','1001',relative_humidity)
			sql = sql + ",(millis(),1,%s,%s,%s)" % ('2','1002',pressure)
			curM.execute(sql)
			curM.execute("commit")
			output ("Meteo","Temperatura corrente "+str(temp_c)+" Celsius")
			#get meteo icon
			sql = "SELECT DISTINCT id FROM tbmeteo WHERE tbmeteo.condition = '" + str(meteo) + "'"
			curM.execute(sql)
			for (id) in curM:
				meteoid = id[0]
			sql = "SELECT id FROM tbnode WHERE sendmeteo = 1"
			curM.execute(sql)
			if int(temp_c) < 0:
				temp_c_fixed = abs(int(temp_c))+9000
			else: 	
				temp_c_fixed = int(temp_c)*100
			for (id) in curM:
				id = id[0]
				#print (temp_c_fixed)
				sql = "INSERT INTO tbdataout (timekey,type,v0,v1,v2,v3,v4) VALUES (millis(),7,%s,%s,%s,%s,%s)" % (id, temp_c_fixed, relative_humidity, pressure, meteoid)
				curM2.execute(sql)
				curM2.execute("commit")
			output ("Meteo","Meteo inviato")			
		except: 
			output ("Meteo","database error...")
	timerMeteo = threading.Timer(300.0, execMeteo).start()
	
def execUps():
	output ("UPS","Exec UPS")
	OrderedDict = apc.parse(apc.get(), strip_units=True)
	status = OrderedDict['STATUS']
	if status == 'ONLINE' or status == 'CHARGING':
		vstatus = 1
		bcharge = int(float(OrderedDict['BCHARGE']))
		battv = float(OrderedDict['BATTV'])
		timeleft = float(OrderedDict['TIMELEFT'])
		linev = int(float(OrderedDict['LINEV']))
	elif status == 'ONBATT':
		vstatus = 0	
		bcharge = int(float(OrderedDict['BCHARGE']))
		battv = float(OrderedDict['BATTV'])
		timeleft = float(OrderedDict['TIMELEFT'])
		linev = int(float(OrderedDict['LINEV']))
	elif status == 'COMMLOST':
		vstatus = 99
		bcharge = 0
		battv = 0.0
		timeleft = 0.0
		linev = 0
	else:
		vstatus = 999
		bcharge = 0
		battv = 0.0
		timeleft = 0.0
		linev = 0				
	
	try:
		#sql = "UPDATE tbsensor SET currentvalue = %s , lastupdate = '%s' where pin_number = '%s'"  % (bcharge, datetime.datetime.now(),'32')
		sql = "INSERT INTO tbdatain (timekey,type,v0,v1,v2) values (millis(),1,%s,%s,%s)" % ('8','32',bcharge)
		sql = sql + ",(millis(),1,%s,%s,%s)" % ('8','33',vstatus)
		sql = sql + ",(millis(),1,%s,%s,%s)" % ('8','34',battv)
		sql = sql + ",(millis(),1,%s,%s,%s)" % ('8','30',timeleft)
		sql = sql + ",(millis(),1,%s,%s,%s)" % ('8','31',linev)
		
		#output("UPS", sql)
		curU.execute(sql)
		curU.execute("commit")
		output ("UPS","Aggiornamento UPS fatto")
					
	except: 
		output ("UPS","database error...")
	timerUps = threading.Timer(30.0, execUps).start()

timerForecast = threading.Timer(10.0, execForecast).start()	
timerMeteo = threading.Timer(1.0, execMeteo).start()
timerUps = threading.Timer(3.0, execUps).start()
		

while True:
	a = 0
	
