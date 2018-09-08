#!/usr/bin/python
# -*- coding: utf-8 -*-
# Ver 16 - 23 March 2017 -

import sys
import os
from os import system

sys.path.append(os.path.join(os.path.dirname(__file__), '../'))

#import pygubu

import subprocess 
from subprocess import *
import time
from threading import Thread

from queue import Queue, Empty

import mysql.connector
from mysql.connector import errorcode
from db import * 

maxIdx = 15

idx = [None]*maxIdx
proc = [None]*maxIdx
arg = [None]*maxIdx
p = [None]*maxIdx
t = [None]*maxIdx #threads
q = Queue()

p_path = "/var/dhproc/"

StatusBar = 0

cmd = "python3"
#for num in range(1, maxIdx):
#	idx[num] = num
#	arg[num] = "0"
	
idx[1] = 1
idx[2] = 2
idx[3] = 3
idx[4] = 4
idx[5] = 5
idx[6] = 6
idx[7] = 7
idx[8] = 8
idx[9] = 9
idx[10] = 10
idx[11] = 11
idx[12] = 12
idx[13] = 13
idx[14] = 14

arg[1] = ""
arg[2] = ""
arg[3] = "0"
arg[4] = "1"
arg[5] = ""
arg[6] = ""
arg[7] = ""
arg[8] = ""
arg[9] = ""
arg[10] = ""
arg[11] = ""
arg[12] = ""
arg[13] = ""
arg[14] = ""

proc[1] = "p_server_queue"
proc[3] = "scheduler"
proc[4] = "scheduler"
proc[6] = "sendMail"
proc[11] = "p_cmd_in"
proc[12] = "p_cmd_out"
proc[13] = "p_cmd_get"
proc[14] = "p_serial"
proc[5] = "setHistoryData"
proc[2] = "getMeteo"
proc[7] = "setHistoryDataStatistic"
proc[8] = "syncTime"
proc[9] = "smartlight"
proc[10] = "getMeteoForecast"
# proc[10] = "smartLights"

status_text = ""
dbstarted = False

while dbstarted == False: # wait until mysql server is not started
	time.sleep(2)
	# -- DB Connection ---------------------------
	try:
		db = mysql.connector.connect(**config)
		dbstarted = True
	except mysql.connector.Error as err:
		dbstarted = False
		if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
			print("Something is wrong with your user name or password")
		elif err.errno == errorcode.ER_BAD_DB_ERROR:
			print("Database does not exists")
		else:
			print(err)

cur = db.cursor()  
# -- END DB Connection ---------------------------

def begin():
	for num in range(1, maxIdx):
		p[num] = initProcess(num)
		t[num] = Thread(target=enqueue_output, args=(p[num].stdout, q, num))
		t[num].daemon = True
		t[num].start()
		print(num)
		if num == 1:
			time.sleep(5);	


def startProc(num):
	if p[num].poll() is None: # it is running
		killProc(num, p[num])
	else:			
		p[num] = setProcess(num)
		p[num] = initProcess(num)
		t[num] = Thread(target=self.enqueue_output, args=(p[num].stdout, q, num))
		t[num].daemon = True
		t[num].start()		

# define the method callbacks
def exec_ss_1():
	startProc(1)	
	
def exec_ss_2():
	startProc(2)	
	
def exec_ss_3():
	startProc(3)	
	
def exec_ss_4():
	startProc(4)	
	
def exec_ss_5():
	startProc(5)	
	
def exec_ss_6():
	startProc(6)	
	
def exec_ss_7():
	startProc(7)	
	
def exec_ss_8():
	startProc(8)	
	
def exec_ss_9():
	startProc(9)

def exec_ss_10():
	startProc(10)	
	
def exec_ss_11():
	startProc(11)	
	
def exec_ss_12():
	startProc(12)	
	
def exec_ss_13():
	startProc(13)	
	
def exec_ss_14():
	startProc(14)															

		
def log(t, m):
	sql = "insert into tblog (type,msg) VALUES (%s, %s)" 
	cur.execute(sql, (t,m))
	db.commit()
	cur.close		

def initProcess(id):
	try:
		px = Popen([cmd, p_path + proc[id] + ".py", str(arg[id])], shell=False, stdin=None, stderr=None, stdout=PIPE, bufsize=1)
		pid = px.pid
		cur.execute("UPDATE tbproctl SET sts = %s WHERE id = %s", (pid, 100+id))
		db.commit()	
		return px
	except:
		a = 0
		
def setProcess(self, index):
	px = Popen([cmd, p_path + proc[index] + ".py", str(arg[index])], shell=False, stdin=None, stderr=None, stdout=PIPE, bufsize=1)
	pid = px.pid
	cur.execute("UPDATE tbproctl SET sts = %s WHERE id = %s", (pid, 100+index))
	db.commit()
	obj = self.builder.get_object('statusbar')
	self.setOutput("Starting " + proc[index], obj)
	return px

def killProc(self, index, p):
	try:
		cur.execute("UPDATE tbproctl SET sts = %s WHERE id = %s", (0, 100+id))
		db.commit()
		time.sleep(3)
		return
	except:
		a = 0
	p.kill()
	obj = self.builder.get_object('statusbar')
	self.setOutput("Stopping " + proc[index], obj)
	return 0

def checkStatus(index, pindex):
	sts = 0
	if pindex.poll() is not None:
		sts = 9			
	else:
		sts = 1	
	try:
		cur.execute("UPDATE tbproctl SET sts = %s WHERE id = %s", (sts, 100+index))
		db.commit()
		#time.sleep(3)
		return
	except:
		print("sql errorrrrrrrrrrrrrrrrrrrrrrrrrrrr")		
			
def isNotInt(self, s):
	try:
		int(s)
		return False
	except ValueError:
		return True	

def updateOutput(index, line):
	if isNotInt(line) is False:
		txt = line.rstrip().decode("utf-8")
	else:
		txt = str(line)
		print(txt)
	
def setOutput(txt, obj):
	global status_text
	global StatusBar
	status_text = txt + '\r\n' + status_text
	status_text = (status_text[:200] + '..') if len(status_text) > 200 else status_text	
	#obj = self.builder.get_object('statusbar')
	#obj.delete("1.0", "end")
	#obj.insert("end", status_text)

def enqueue_output(out, queue, idx):
	for line in iter(out.readline, b''):
		queue.put("thread")
		queue.put(idx)
		queue.put(line)
		print(line)
		#out.close()	
					
def checkProc():
	try: 
		line = q.get_nowait()
		if line == "thread":
			index = q.get_nowait()
			line = q.get_nowait()
			self.updateOutput(index, line)	
	except Empty:
		a = 1

	for num in range(1, maxIdx):
		checkStatus(num, p[num])

#	self.mainwindow.after(10, self.checkProc)		
begin()	
while True:
	for num in range(1, maxIdx):
		checkStatus(num, p[num])
	time.sleep(5)		
