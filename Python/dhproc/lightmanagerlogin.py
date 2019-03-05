# Hack to allow relative import above top level package
import sys
import os
folder = os.path.dirname(os.path.abspath(__file__))  # noqa
sys.path.insert(0, os.path.normpath("%s/.." % folder))  # noqa

from pytradfri import Gateway
from pytradfri.api.libcoap_api import APIFactory
from pytradfri.error import PytradfriError
from pytradfri.util import load_json, save_json

import uuid
import argparse
import threading
import time
import datetime

import multiprocessing as mp
from multiprocessing import Queue
from multiprocessing.managers import SyncManager

HOST = ''
PORT8 = 5555
PORT9 = 5556
AUTHKEY = str("123456").encode("utf-8")

ip_host = "192.168.1.75"
CONFIG_FILE = 'tradfri_standalone_psk.conf'

# Assign configuration variables.
# The configuration check takes care they are present.
conf = load_json(CONFIG_FILE)

#if ip_host not in load_json(CONFIG_FILE):
#	key = input().strip()
#	if len(key) != 16:
#		raise PytradfriError("Invalid 'Security Code' provided.")
#	else:
#		print("Key OK")

def get_index(id, list_):
	for i, s in enumerate(list_):	
		if str(id) == str(s.path[1]):			
			return i
	return -1	
	
def QueueServerClient(HOST, PORT, AUTHKEY):
    class QueueManager(SyncManager):
        pass
    QueueManager.register('get_queue')
    QueueManager.register('get_name')
    QueueManager.register('get_description')
    manager = QueueManager(address = (HOST, PORT), authkey = AUTHKEY)
    manager.connect() # This starts the connected client
    return manager

#------- Main section ----------------------------#
#------- Run once --------------------------------#
# create three connected managers
qmLightManager = QueueServerClient(HOST, PORT8, AUTHKEY)
qmLightResponseManager = QueueServerClient(HOST, PORT9, AUTHKEY)
# Get the queue objects from the clients
qLightCommand = qmLightManager.get_queue()
qLightResponse = qmLightResponseManager.get_queue()

api_factory = APIFactory(host=ip_host, psk_id="", psk="")
api = api_factory.request
lights = []
groups = []

def connect():
	print("connect")
	global conf
	global api_factory	
	global lights
	global groups
	global api
	try:
		identity = conf[ip_host].get('identity')
		psk = conf[ip_host].get('key')
		api_factory = APIFactory(host=ip_host, psk_id=identity, psk=psk)
	except KeyError:
		identity = uuid.uuid4().hex
		api_factory = APIFactory(host=ip_host, psk_id=identity)

		try:
			psk = api_factory.generate_psk(key)
			print('Generated PSK: ', psk)

			conf[ip_host] = {'identity': identity,'key': psk}
			save_json(CONFIG_FILE, conf)
		except AttributeError:
			raise PytradfriError("Please provide the 'Security Code' on the back of your Tradfri gateway using the -K flag.")

	api = api_factory.request
	gateway = Gateway()

	#get all devices
	devices_command = gateway.get_devices()
	devices_commands = api(devices_command)
	devices = api(devices_commands)
	# create list of available bulbs
	lamps = [dev for dev in devices if dev.has_light_control]

	# get all available groups
	groups_command = gateway.get_groups()
	groups_commands = api(groups_command)
	groupc = api(groups_commands)
	groups = [dev for dev in groupc]
	
	lights = [dev for dev in devices if dev.has_light_control]
	#-------------------------------------------------------------------
	

def outputq(x):
	print(str(datetime.datetime.now().time())[:8] + " "+ str(x))
	sys.stdout.flush()
	
threading.Timer(900, connect).start()	#reconnect every 15 minutes
connect()

# supported_features 1=mono 23=color
#device info  TRADFRI bulb E27 W opal 1000lm
#device info  TRADFRI bulb E27 opal 1000lm
#device info  TRADFRI bulb E27 opal 1000lm
#device info  TRADFRI bulb GU10 WS 400lm
#device info  TRADFRI bulb GU10 WS 400lm
#device info  TRADFRI bulb GU10 WS 400lm
#device info  LCT001

#for light in lights:
#	print("device info ", light.device_info.model_number) 
#	print("supported feature ", light.light_control.lights[0].supported_features) 
#print("supported feature ", light.light_control.can_set_dimmer) 
#print("supported feature ", light.light_control.can_set_temp) 
#print("supported feature ", light.light_control.can_set_color) 
#	print("Tipo ", getLightType(light.light_control.can_set_dimmer, light.light_control.can_set_temp, light.light_control.can_set_color))

def run():	
	# arg1=IP, arg2=key, arg3=group, arg4=light, arg5=command, arg6=value
	output = "no command"
	if not qLightCommand.empty():
		outputq("Command received")
		args = qLightCommand.get()
		group = args.pgroup
		light = args.plight
		command = args.pommand
		value = args.palue
		if value > 254:
			value = 254
		elif value < 0:
			value = 0 	
		#print(args)
		#return the group list
		if group == 0 and command == "listgroup":
			output = "listgroup,"
			for groupName in groups:
				output = output + str(groupName.path[1]) + ","
				output = output + getGroupName(str(groupName)) + ","
				output = output + "0,"
				output = output + "0,"
				
		#return the status of a group
		if group != 0 and command == "statusgroup":
			output = "statusgroup,"
			idx = get_index(group, groups)
			if idx != -1:
				sts = groups[idx].state
				dim = groups[idx].dimmer
				output = output + str(groups[idx].path[1]) + ","
				output = output + str(sts) + "," 
				output = output + str(dim) + "," 
				output = output + "0,"
				#print(groups[idx].mood)
				#print(groups[idx].mood_id)
		
		#return the light list
		if group != 0 and light == 0 and command == "listlight":
			idx = get_index(group, groups)
			if idx != -1:
				group_members = groups[idx].member_ids
				output = "listlight,"
				for i, s in enumerate(group_members):
					x = get_index(s, lights)
					if x > -1:
						output = output + str(lights[x].path[1]) + ","
						output = output + str(lights[x]) + ","
						output = output + str(getLightType(lights[x].light_control.can_set_dimmer, lights[x].light_control.can_set_temp, lights[x].light_control.can_set_color)) + ","
						output = output + str(lights[x].light_control.lights[0].state) + "," 
						output = output + str(lights[x].light_control.lights[0].dimmer) + "," 
						output = output + str(get_color_temp_idx(lights[x].light_control.lights[0].hex_color)) + "," 
				
		#return the lights status of a group
		if group != 0 and light == 0 and command == "statuslight":
			idx = get_index(group, groups)
			if idx != -1:
				group_members = groups[idx].member_ids
				output = "statuslight,"
				for i, s in enumerate(group_members):
					x = get_index(s, lights)
					if x > -1:
						sts = lights[x].light_control.lights[0].state
						dim = lights[x].light_control.lights[0].dimmer
						color = lights[x].light_control.lights[0].hex_color
						output = output + str(lights[x].path[1]) + "," 
						output = output + str(sts) + "," 
						output = output + str(dim) + "," 
						output = output + str(get_color_temp_idx(color)) + "," 		
		
		#-- Set group dimmer ---------------------------------------
		if group != 0 and light == 0 and command == "setdimmer":
			x = get_index(group, groups)
			cmd = groups[x].set_dimmer(value, transition_time=20)	
			api(cmd)
			output = command + " group"
	
		#-- Set lights dimmer ---------------------------------------
		if group == 0 and light != 0 and command == "setdimmer":
			x = get_index(light, lights)
			cmd = lights[x].light_control.set_dimmer(value, transition_time=20)	
			api(cmd)
			output = command + " light"
		
		#-- Set lights ---------------------------------------
		if group == 0 and light != 0 and value != 0 and command == "setcolor":
			x = get_index(light, lights)
			colorHex = get_color_temp(value)
			cmd = lights[x].light_control.set_hex_color(colorHex)	
			api(cmd)
			output = command
			
			
		#-- Set group color ---------------------------------------
		if group != 0 and light == 0 and value != 0 and command == "setcolor":
			idx = get_index(group, groups)
			if idx != -1:
				group_members = groups[idx].member_ids
				for i, s in enumerate(group_members):
					x = get_index(s, lights)
					if x > -1:
						colorHex = get_color_temp(value)
						cmd = lights[x].light_control.set_hex_color(colorHex)	
						api(cmd)
						output = command
			
		qLightResponse.put(output)
		outputq(output)
				
										

def getLightType(can_dimmer, can_color_temp, can_color):
	if can_dimmer == True and can_color_temp != True and can_color != True: 
		# monocrome
		return 0
	elif can_dimmer == True and can_color_temp == True and can_color != True: 
		# 2 color temperature
		return 2
	elif can_dimmer == True and can_color_temp == True and can_color == True: 
		# 3 color rgb
		return 1
		
def getGroupName(group):
	pos = group.find('-')
	return group[7:pos:]
			
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

while True:
	run()
