#!/usr/bin/python
# -*- coding: utf-8 -*-
# Ver 20 - 2 December 2017 -

import logging
import time
#import string
import sys
import threading
import mysql.connector
from mysql.connector import errorcode
from db import * 
import datetime
import queue

from pytradfri import Gateway
from pytradfri.api.libcoap_api import APIFactory
   
def output(x):
	print(str(datetime.datetime.now().time())[:8] + " "+ str(x))
	sys.stdout.flush()
	
#----------------------------- 
# Global Variable declaration  
#----------------------------- 
ip = "192.168.1.75"

# Assign configuration variables.
# The configuration check takes care they are present.
api_factory = APIFactory(ip)
print(api_factory)
with open('gateway_psk.txt', 'a+') as file_psk, open('gateway_key.txt', 'a+') as file_key:
	file_psk.seek(0)
	psk = file_psk.read()
	if psk:
		api_factory.psk = psk.strip()
	else:
		file_key.seek(0)
		key = file.read()
		if key:
			psk = api_factory.generate_psk(key)
			print('Generated PSK: ', psk)
		file_psk.write(psk)
	file_psk.close()
	file_key.close()
	api = api_factory.request

gateway = Gateway()

#devices_command = gateway.get_devices()
#devices_commands = api(devices_command)
#evices = api(devices_commands)

#lights = [dev for dev in devices if dev.has_light_control]
  
groups_command = gateway.get_groups()
groups_commands = api(groups_command)
groups = api(groups_commands)

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
	output(now.strftime("%H %M %S %f"))	

def get_index(id, list_):
	for i, s in enumerate(list_):	
		if str(id) == str(s.path[1]):			
			return i
	return -1
	
def get_index_device(dev, listdev_):
	for s in listdev_:	
		if str(dev.path[1]) == str(s.path[1]):	
			return 0
	return -1	
	
def get_color_temp(id):
	# f5faf6 = cold | f1e0b5 = normal | efd275 = warm
	if id == 4:
		color = 'dcf0f8'
	elif id == 5:
		color = 'eaf6fb'
	elif id == 1:
		color = 'f5faf6'
	elif id == 6:
		color = 'f2eccf'
	elif id == 2:
		color = 'f1e0b5'
	elif id == 3:
		color = 'efd275'
	elif id == 7:		
		color = 'ebb63e'
	elif id == 8:	
		color = 'e78834'
	elif id == 9:	
		color = 'e57345'
	elif id == 10:	
		color = 'da5d41'
	elif id == 11:
		color = 'dc4b31'
	elif id == 12:		
		color = 'e491af'
	elif id == 13:		
		color = 'e8bedd'
	elif id == 14:		
		color = 'd9337c'
	elif id == 15:		
		color = 'c984bb'
	elif id == 16:
		color = '8f2686'
	elif id == 17:
		color = '4a418a'
	elif id == 18:
		color = '6c83ba'
	elif id == 19:
		color = 'a9d62b'
	elif id == 20:
		color = 'd6e44b'
	else:
		color = 'f5faf6'		
	return color		
	

def get_color_temp_idx(color):
	if color == 'dcf0f8':
		c=4
	elif color == 'eaf6fb':
		c=5
	elif color == 'f5faf6':
		c=1		
	elif color == 'f2eccf':
		c=6	
	elif color == 'f1e0b5':
		c=2
	elif color == 'efd275':
		c=3		
	elif color == 'ebb63e':
		c=7		
	elif color == 'e78834':
		c=8		
	elif color == 'e57345':
		c=9		
	elif color == 'da5d41':
		c=10		
	elif color == 'dc4b31':
		c=11		
	elif color == 'e491af':
		c=12		
	elif color == 'e8bedd':
		c=13		
	elif color == 'd9337c':
		c=14		
	elif color == 'c984bb':
		c=15
	elif color == '8f2686':
		c=16
	elif color == '4a418a':
		c=17
	elif color == '6c83ba':
		c=18
	elif color == 'a9d62b':
		c=19
	elif color == 'd6e44b':
		c=20
	else: 
		c=1
	return c	
						

def getStatus():	
	print(groups)

	

#------- Main section ----------------------------#

#------- Start main loop -------------------------#
while True:
    # controllo se ci sono segnalazioni da arduino
	#sendCommand()
	#setStatus()	
	getStatus()
	time.sleep(0.5)
#------- End main loop -------------------------#
  


