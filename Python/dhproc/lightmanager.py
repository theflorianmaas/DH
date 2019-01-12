# Hack to allow relative import above top level package
import sys
import os
folder = os.path.dirname(os.path.abspath(__file__))  # noqa
sys.path.insert(0, os.path.normpath("%s/.." % folder))  # noqa

import uuid
import argparse
import threading
import time

import multiprocessing as mp
from multiprocessing import Queue
from multiprocessing.managers import SyncManager

HOST = ''
PORT8 = 5555
PORT9 = 5556
AUTHKEY = str("123456").encode("utf-8")

parser = argparse.ArgumentParser()
parser.add_argument('host', metavar='IP', type=str,	help='IP Address of your Tradfri gateway')
parser.add_argument('-K', '--key', dest='key', required=False, help='Security code found on your Tradfri gateway')
parser.add_argument('pgroup', type=int,	help='default group. 0 = All groups')
parser.add_argument('plight', type=int,	help='default light. 0 = All lights')
parser.add_argument('pommand', type=str,	help='command for group or light')
parser.add_argument('palue', type=int, help='value to execute command')
args = parser.parse_args()

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

qLightCommand.put(args)
# initialize output
print("")

while True:
	if not qLightResponse.empty():
		res = qLightResponse.get()
		print(res)
		break;


