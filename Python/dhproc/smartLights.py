#!/usr/bin/python
# -*- coding: utf-8 -*-
# Ver 19 - 20 November 2017 -

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
 
curUno = db.cursor(buffered=True)
curObs = db.cursor()
curUpd = db.cursor()
curDel = db.cursor()
	

ip = "192.168.1.75"
UpdDdevQueue = queue.LifoQueue()

# Assign configuration variables.
# The configuration check takes care they are present.
api_factory = APIFactory(ip)
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

devices_command = gateway.get_devices()
devices_commands = api(devices_command)
devices = api(devices_commands)

lights = [dev for dev in devices if dev.has_light_control]
#25 lights
tl = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
tp = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
  
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
						

def sendCommand():  
	#--------------------------------------------------------------------------------------------------------#
	#----begin building string to send out-------------------------------------------------------------------#
	#--------------------------------------------------------------------------------------------------------#
	#0=on/off
	#1=set color temperature
	#2=set dimmer 
	#3=rgb color
	#output ("Checking commands...")	
	sql = ("select timekey,tbdataoutsmartlight.type,V0,V1,V2,V3,V4,V5,V6 as smartlight_id,V7 as sltype "
		   "from tbdataoutsmartlight, tbsmartlight "
		   "where tbdataoutsmartlight.V0 = tbsmartlight.tbactuator_id "
		   "order by timekey asc")
	#V0 = Node
	#V1 = actuator_id
	#V2 = method id
	#V3 = output type id
	#V4	= value
	#V5 = not used
	#V6 = smartlight_id
	#V7 = type group=0 light=1
	curUno.execute(sql)
	lista = list(curUno.fetchall())
	for (timekey,type,V0,V1,V2,V3,V4,V5,smartlight_id,sltype) in lista:
		output ("Executing command...")	
		action_type = int("{}".format(type))
		status = int("{}".format(V4))
		light_type = "{}".format(sltype) #G=Group L=Light
		if action_type == 0: #0=on/off 1=set color 2=dimmer
			output("Switching light...")			
			if light_type == "0": #group
				x = get_index("{}".format(smartlight_id), groups)
				cmd2 = groups[x].set_state(status)
				api(cmd2)
				group_members = groups[x].member_ids				
				for i, s in enumerate(group_members):
					x = get_index(s, lights)	
					cmd2 = lights[x].light_control.set_state(status)
					api(cmd2)						
			if light_type == "1": #light
				x = get_index("{}".format(smartlight_id), lights)
				cmd2 = lights[x].light_control.set_state(status)
				api(cmd2)		
		if action_type == 1: #smart light color temperature
			output("Setting light color")
			if light_type == "0": #groups
				# setting mood
				x = get_index("{}".format(smartlight_id), groups)
				#cmd = groups[x].set_dimmer(int("{}".format(V4)))	
				#api(cmd)
			if light_type == "1": #light
				x = get_index("{}".format(smartlight_id), lights)
				color = get_color_temp(int("{}".format(V4)))
				cmd = lights[x].light_control.set_hex_color(color)	
				api(cmd)
		if action_type == 2: #Smartlight dimmer
			output("Setting dimmer")
			if light_type == "0": #groups
				x = get_index("{}".format(smartlight_id), groups)
				cmd = groups[x].set_dimmer(int("{}".format(V4)))	
				api(cmd)
			if "{}".format(sltype) == "1": #light
				x = get_index("{}".format(smartlight_id), lights)
				cmd = lights[x].light_control.set_dimmer(int("{}".format(V4)))	
				api(cmd)	
		if action_type == 3: #Smartlight rgb color picker 
			output("Setting light rgb color")
			
		#-- common part --#			
		sql = "delete from tbdataoutsmartlight where timekey = " + "{}".format(timekey)
		#print(sql)
		try:	
			curUpd.execute(sql)
			output ("Ok")
		except:
			raise			
	db.commit()
	
def observe(api, device):
	def callback(updated_device):
		global UpdDdevQueue
		print("Received message for: %s" % device)
		UpdDdevQueue.put(updated_device)
			
	def err_callback(err):
		print(err)

	def worker():
		api(device.observe(callback, err_callback, duration=1))

	for i in range(1):
		t = threading.Thread(target=worker)
		t.setDaemon(True)
		t.start()

	time.sleep(0.1)   
	return t 	

def callback(updated_group):
	a = 1

def err_callback(err):
	print(err)	
	
def startObserve():
	global tl	
	output ("Starting observe...")	
	for (tbactuator_id, smartlight_id, type, tbnode_id, pinnumber) in list_devices:
		if "{}".format(type) == "bytearray(b'G')": #groups
			x = get_index("{}".format(smartlight_id), groups)
			if x != -1:
				group = groups[x]
				tp[x] = observe(api, group)
		elif "{}".format(type) == "bytearray(b'L')": #light	
			x = get_index("{}".format(smartlight_id), lights)
			if x != -1:
				light = lights[x]
				tl[x] = observe(api, light)
	
def getStatus():	
	global tl
	for (tbactuator_id, smartlight_id, type, tbnode_id, pinnumber) in list_devices:
		if "{}".format(type) == "bytearray(b'G')": #group
			x = get_index("{}".format(smartlight_id), groups)
			if x != -1:
				group = groups[x]
				sts = groups[x].state
				dim = groups[x].dimmer
				if tp[x].isAlive() == False:
					tp[x] = observe(api, group)
					print("Restart________________________Alive")
				sql = "UPDATE tbactuatormethodvalue SET value = " + str(sts) + " WHERE tbactuator_id = " + str("{}".format(tbactuator_id)) +  " AND tbactuatormethod_id = 38"
				curUpd.execute(sql)	
				sql = "UPDATE tbactuator SET currentstatus = " + str(sts) + " WHERE id = " + "{}".format(tbactuator_id) 
				curUpd.execute(sql)
				sql = "UPDATE tbactuatormethodvalue SET value = " + str(dim) + " WHERE tbactuator_id = " + str("{}".format(tbactuator_id)) +  " AND tbactuatormethod_id = 36"
				curUpd.execute(sql)	
		elif "{}".format(type) == "bytearray(b'L')": #light	
			x = get_index("{}".format(smartlight_id), lights)
			if x != -1:
				xd = get_index("{}".format(smartlight_id), devices)
				light = lights[x]
				if tl[x].isAlive() == False:
					tl[x] = observe(api, light)
					print("Restart________________________Alive")
				rec = light.reachable
				if rec == True:
					sts = light.light_control.lights[0].state
					dim = light.light_control.lights[0].dimmer
					color = light.light_control.lights[0].hex_color
					if color == "none":
						color=1
					idx = get_color_temp_idx(color)
					sql = "UPDATE tbactuatormethodvalue SET value = " + str(dim) + " WHERE tbactuator_id = " + str("{}".format(tbactuator_id)) +  " AND tbactuatormethod_id = 36"
					curUpd.execute(sql)	
					sql = "UPDATE tbactuatormethodvalue SET value = " + str(idx) + " WHERE tbactuator_id = " + str("{}".format(tbactuator_id)) +  " AND tbactuatormethod_id = 35" # for normal bulbs
					curUpd.execute(sql)	
					sql = "UPDATE tbactuatormethodvalue SET value = " + str(idx) + " WHERE tbactuator_id = " + str("{}".format(tbactuator_id)) +  " AND tbactuatormethod_id = 37" #for RGB
					curUpd.execute(sql)			
				else:
					sts = 0
				sql = "UPDATE tbactuatormethodvalue SET value = " + str(sts) + " WHERE tbactuator_id = " + str("{}".format(tbactuator_id)) +  " AND tbactuatormethod_id = 38"
				curUpd.execute(sql)	
				sql = "UPDATE tbactuator SET currentstatus = " + str(sts) + " WHERE id = " + "{}".format(tbactuator_id) 
				curUpd.execute(sql)
				db.commit()

def init():
	global list_devices
	cur = db.cursor(buffered=True)
	sql = "SELECT tbactuator_id, smartlight_id, type, tbnode_id, pinnumber FROM vwsmartlight"
	cur.execute(sql)
	list_devices = list(cur.fetchall())
	for (tbactuator_id, smartlight_id, type, tbnode_id, pinnumber) in list_devices:
		print(str("{}".format(smartlight_id)))
		
		
def	get_list_index(id):
	i = 0
	for (tbactuator_id, smartlight_id, type, tbnode_id, pinnumber) in list_devices:
		if str(id) == str("{}".format(smartlight_id)):
			return i
		i = i +1	
	return -1			

def setStatus():
	global UpdDdevQueue
	while not UpdDdevQueue.empty(): # loop on all updated devices
		dev = UpdDdevQueue.get() 
		sql = ""
		node = 0	
		pin = 0
		color = 1
		sts = 0
		dim = 0
		devType = ""
		print ("Sending command...", dev)
		id = str(dev.path[1])
		x = get_index_device(dev, groups) #	check if it a group
		if x != -1:
			devType = "Group"
			group = dev
			sts = groups[x].state
			dim = groups[x].dimmer
			#print("Get_list_index:", get_list_index(id))
			node = list_devices[get_list_index(id)][3]
			pin = list_devices[get_list_index(id)][4]
			color = 1 #mood
		x = get_index_device(dev, lights) #	check if it a light
		if x != -1:
			devType = "Light"
			light = dev
			rec = light.reachable
			if rec == True:
				sts = light.light_control.lights[0].state
				dim = light.light_control.lights[0].dimmer
				color = light.light_control.lights[0].hex_color
				if color == "none":
					color=1
				#print("Get_list_index:", get_list_index(id))	
				node = list_devices[get_list_index(id)][3]
				pin = list_devices[get_list_index(id)][4]
				color = get_color_temp_idx(color)
		qry = "select currentstatus from tbnode WHERE id = " + str(node)
		print(qry)
		curObs.execute(qry)
		sss=str(curObs.fetchone())
		nodeSts = sss[1:2]	
		print(nodeSts)		
		if (devType == "Group") or ((devType == "Light") and (rec == True)) and (nodeSts == "1"):
			qry = "select count(*) from tbdataout WHERE type = 10 AND V0 = " + str(node) + " AND V1 = " + str(pin)
			curObs.execute(qry)
			cnt=curObs.fetchone()
			if cnt[0] == 0:  #id doeen't exist, insert
				sql = "insert into tbdataout (timekey,type,V0,V1,V2,V3,V4) values ("
				sql = sql + "millis(),"
				sql = sql + "10,"
				sql = sql + str(node) + ","
				sql = sql + str(pin) + ","
				sql = sql + str(sts) + ","
				sql = sql + str(dim) + ","
				sql = sql + str(color) + ")"
				print("Insert....", id)
				print("Node....", node)
				print("Pin....", pin)
				
				curObs.execute(sql)
			else: #if some records already exist, update
				sql = "update tbdataout set V2 = " + str(sts) + ",V3 = " + str(dim) + ",V4 = " + str(color) 
				sql = sql + " WHERE type = 10 AND V0 = " + str(node) + " AND V1 = " + str(pin)
				print("Update....", id)
				print("Node....", node)
				print("Pin....", pin)
				curObs.execute(sql)	
		else:
			print("Not exists or Off:", id)		
		

#------- Main section ----------------------------#
init()
startObserve()
#------- Start main loop -------------------------#
while True:
    # controllo se ci sono segnalazioni da arduino
	sendCommand()
	setStatus()	
	getStatus()
	time.sleep(0.3)
#------- End main loop -------------------------#
  


