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

host = "192.168.1.75"
CONFIG_FILE = 'tradfri_standalone_psk.conf'

parser = argparse.ArgumentParser()
parser.add_argument('host', metavar='IP', type=str,	help='IP Address of your Tradfri gateway')
parser.add_argument('-K', '--key', dest='key', required=False, help='Security code found on your Tradfri gateway')
parser.add_argument('pgroup', type=int,	help='default group. 0 = All groups')
parser.add_argument('plight', type=int,	help='default light. 0 = All lights')
parser.add_argument('pommand', type=str,	help='command for group or light')
parser.add_argument('palue', type=int, help='value to execute command')
args = parser.parse_args()
print(args)

# arg1=IP, arg2=key, arg3=group, arg4=light, arg5=command, arg6=value


if args.host not in load_json(CONFIG_FILE) and args.key is None:
	print("Please provide the 'Security Code' on the back of your Tradfri gateway:", end=" ")
	key = input().strip()
	if len(key) != 16:
		raise PytradfriError("Invalid 'Security Code' provided.")
	else:
		args.key = key

def get_index(id, list_):
	for i, s in enumerate(list_):	
		if str(id) == str(s.path[1]):			
			return i
	return -1	  


def run():
	# Assign configuration variables.
	# The configuration check takes care they are present.
	conf = load_json(CONFIG_FILE)
	group = args.pgroup
	light = args.plight
	command = args.pommand
	value = args.palue
	
	try:
		identity = conf[args.host].get('identity')
		psk = conf[args.host].get('key')
		api_factory = APIFactory(host=args.host, psk_id=identity, psk=psk)
	except KeyError:
		identity = uuid.uuid4().hex
		api_factory = APIFactory(host=args.host, psk_id=identity)

		try:
			psk = api_factory.generate_psk(args.key)
			#print('Generated PSK: ', psk)

			conf[host] = {'identity': identity,'key': psk}
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
	
	#return the group list
	if group == 0 and command == "listgroup":
		output = "listgroup,"
		for groupName in groups:
			output = output + str(groupName.path[1]) + ","
			output = output + getGroupName(str(groupName)) + ","
			output = output + "0,"
			output = output + "0,"
		print(output)
			
		
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
			print(output)	
		
	#return the light list
	if group != 0 and light == 0 and command == "listlight":
		idx = get_index(group, groups)
		if idx != -1:
			group_members = groups[idx].member_ids
			output = "listlight,"
			for i, s in enumerate(group_members):
				x = get_index(s, lights)
				output = output + str(lights[x].path[1]) + ","
				output = output + str(lights[x]) + ","
				output = output + str(getLightType(lights[x].light_control.can_set_dimmer, lights[x].light_control.can_set_temp, lights[x].light_control.can_set_color)) + ","
				output = output + "0,"	
			print(output)
				
	#return the lights status of a group
	if group != 0 and light == 0 and command == "statuslight":
		idx = get_index(group, groups)
		if idx != -1:
			group_members = groups[idx].member_ids
			output = "statuslight,"
			for i, s in enumerate(group_members):
				x = get_index(s, lights)
				sts = lights[x].light_control.lights[0].state
				dim = lights[x].light_control.lights[0].dimmer
				color = lights[x].light_control.lights[0].hex_color
				output = output + str(lights[x].path[1]) + "," 
				output = output + str(sts) + "," 
				output = output + str(dim) + "," 
				output = output + str(color) + "," 
			print(output)
				
	#-- Set group dimmer ---------------------------------------
	if group != 0 and light == 0 and command == "setdimmer":
		x = get_index(group, groups)
		cmd = groups[x].set_dimmer(value, transition_time=20)	
		api(cmd)
	
	
	#-- Set lights dimmer ---------------------------------------
	if group == 0 and light != 0 and command == "setdimmer":
		x = get_index(light, lights)
		cmd = lights[x].light_control.set_dimmer(value, transition_time=20)	
		api(cmd)	
		
	#-- Set lights ---------------------------------------
	if group == 0 and light != 0 and value != 0 and command == "setcolor":
		x = get_index(light, lights)
		colorHex = get_color_temp(value)
		cmd = lights[x].light_control.set_hex_color(colorHex)	
		api(cmd)						

def getLightType(can_dimmer, can_color_temp, can_color):
	if can_dimmer == True and can_color_temp != True and can_color != True: 
		# monocrome
		return 1
	elif can_dimmer == True and can_color_temp == True and can_color != True: 
		# 2 color temperature
		return 2
	elif can_dimmer == True and can_color_temp == True and can_color == True: 
		# 3 color rgb
		return 3
		
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
	
run()
