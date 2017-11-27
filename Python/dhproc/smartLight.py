#!/usr/bin/env python3
"""
This is an example of how the pytradfri-library can be used.

To run the script, do the following:
$ pip3 install pytradfri
$ Download this file (example_sync.py)
$ python3 test_pytradfri.py <IP> <KEY>

Where <IP> is the address to your IKEA gateway and
<KEY> is found on the back of your IKEA gateway.
"""

import sys
import threading

import time

from pytradfri import Gateway
from pytradfri.api.libcoap_api import APIFactory


ip = "192.168.1.75"

def observe(api, device):
    def callback(updated_device):
        light = updated_device.light_control.lights[0]
        print("Received message for: %s" % light)

    def err_callback(err):
        print(err)

    def worker():
        api(device.observe(callback, err_callback, duration=120))

    threading.Thread(target=worker, daemon=True).start()
    print('Sleeping to start observation task')
    time.sleep(1)


def run():
	# Assign configuration variables.
    # The configuration check takes care they are present.
    api_factory = APIFactory(ip)
    
    with open('gateway_psk.txt', 'a+') as file:
        file.seek(0)
        psk = file.read()
        if psk:
            api_factory.psk = psk.strip()
        else:
            psk = api_factory.generate_psk(key)
            print('Generated PSK: ', psk)
            file.write(psk)
    api = api_factory.request

    gateway = Gateway()

    devices_command = gateway.get_devices()
    print(devices_command)
    devices_commands = api(devices_command)
    devices = api(devices_commands)

    lights = [dev for dev in devices if dev.has_light_control]
    xx = [dev for dev in devices]
	
    tasks_command = gateway.get_smart_tasks()
    tasks = api(tasks_command)
    
    groups_command = gateway.get_groups()
    groups_commands = api(groups_command)
    groups = api(groups_commands)
    
    
    
    print(len(groups))
    print ("xxxxxxxxxxxxxxxx")
    print(xx)

    # Print all lights
    #print(lights)

    # Lights can be accessed by its index, so lights[1] is the second light
    light = lights[0]

    #observe(api, light)

    # Example 1: checks state of the light 2 (true=on)
    #print(light.light_control.lights[0].state)

    ## Example 2: get dimmer level of light 2
    #print(light.light_control.lights[0].dimmer)

    # Example 3: What is the name of light 2
    #print(light.name)

    # Example 4: Set the light level of light 2
    #dim_command = light.light_control.set_dimmer(255)
    #api(dim_command)

    # Example 5: Change color of light 2
    # f5faf6 = cold | f1e0b5 = normal | efd275 = warm
    #color_command = light.light_control.set_hex_color('efd275')
    #api(color_command)
    
    #x = get_group_index(str(3), groups)
    for i, s in enumerate(groups):	
        print(s)
        print(s.path[1])
		#print(s.[i].state)
		#print(s.[i].dimmer)
		#print(s.[i].member_ids)
		#print(s.[i].members)
		#print(s.[i].mood_id)
		#print(s.[i].mood)
        print("+++++-----------------------------------++++")

    print("XXXXXX-----------------------------------++++") 
 
    for i, s in enumerate(lights):	
        print(s)
        print(s.path[1])
        print("+++++-----------------------------------++++")   

    #print("-----------------------------------------")    
    #print(light.light_control.lights[0].state)
    #print(light.light_control.lights[0].light.state)
    #print(light.light_control.lights[0].dimmer)
    #print(light.light_control.lights[0].hex_color)
    #print(light.light_control.lights[0].raw)
    #print(lights[0].path[1])
    #print(groups[0].path[1])
    

    print("-----------------------------------------")

    #z = groups[x].set_state(0)
    #api(z)
    #z = groups[x].set_dimmer(255)
    #api(z)
    time.sleep(2)
    #groups[x].set_state(0)

    # Example 6: Return the transition time (in minutes) for task#1
    if tasks:
        #print(tasks[0].task_control.tasks[0].transition_time)

        # Example 7: Set the dimmer stop value to 30 for light#1 in task#1
        dim_command_2 = tasks[0].start_action.devices[0].item_controller\
            .set_dimmer(30)
        api(dim_command_2)

    #print("Sleeping for 2 min to receive the rest of the observation events")
    #print("Try altering the light (%s) in the app, and watch the events!" %
          #light.name)
    #time.sleep(120)

def get_group_index(id, groups):
	for i, s in enumerate(groups):
		if id in str(s):
			return i
	return -1
	

run()
