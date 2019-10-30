'''
*  Copyright (C) 2014 Ekironji <ekironjisolutions@gmail.com>
*
*  This file is part of serial libraries examples for UDOO
*
*  Serial libraries examples for UDOO is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This libraries are distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
'''
import time
import serial
import string
import sys
import mysql.connector
from mysql.connector import errorcode, pooling
from db import * 
import datetime
#from threading import Thread
import multiprocessing as mp
from multiprocessing import Queue
from multiprocessing.managers import SyncManager

from os import system, devnull
from subprocess import call, STDOUT
from threading import Thread
from time import sleep 
#import queue

HOST = ''
PORT0 = 5011
PORT1 = 5012
PORT2 = 5013
PORT3 = 5014
AUTHKEY = str("123456").encode("utf-8")

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
    output(err)
else:
  output("PYSERIAL","Start procedure")
  db.commit()
# -- END DB Connection ---------------------------

# -- Open Serial to the Coordinator---------------


serCoord = serial.Serial('/dev/ttyACM0',115200,timeout=1)
serCoord.flushOutput()
serCoord.timeout = 10

#serCoord.setDTR(False)
time.sleep(1)
# toss any data already received, see
serCoord.flushInput()
serCoord.flushOutput()

strx = "IS160,1,0,0,0,0,3,18,0,0,0,3,20,0,0,0,3,21,0,0,0,3,24,0,0,0,3,25,0,0,0,4,19,0,0,0,4,20,0,0,0,4,21,0,0,0,4,23,0,0,0,4,24,0,0,0,4,25,0,0,0,5,18,0,0,0,5,19,0,0,0,5,20,0,0,0,5,21,0,0,0,5,23,0,0,0,5,24,0,0,0,5,25,0,0,0,6,20,0,0,0,6,21,0,0,0,6,23,0,0,0,6,24,0,0,0,6,25,0,0,0,8,26,0,0,0,9,24,0,0,0,9,25,0,0,0,11,20,0,0,0,11,21,0,0,0,11,22,0,0,0,11,24,0,0,0,11,25,0,0,0"
endSerialChars = b"\r\n"

def initSendStringsToCoordinator(stg):  
	serCoord.flushInput()
	print(stg)
	# send the node string
	attemptsCnt = 0
	while serCoord.inWaiting() == 0  and attemptsCnt < 5:
		ret = serCoord.write(bytes(stg, 'UTF-8')) 
		time.sleep(1.0)
		print(ret)
		readSerial = serCoord.readline()
		print(readSerial)
		#print(readSerial.rstrip(endSerialChars))
		if readSerial == CommExecutedTrue:
			return 1
			time.sleep(0.2)
			break
		elif readSerial == CommExecutedFalse:
			# write error in log
			log("E", "Error "+stg)
		else:
			attemptsCnt = attemptsCnt + 1
			print(attemptsCnt)
			continue
	# write error in log
	log("E", "no serial available")
	return 0


print('Serial connected')
ret = 0

while ret == 0:
	initSendStringsToCoordinator(strx)
	time.sleep(5) 


