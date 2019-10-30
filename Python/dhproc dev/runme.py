#!/usr/bin/python
# -*- coding: utf-8 -*-
# Ver 17 - 23 January 2019 -

import sys
import os
from os import system

sys.path.append(os.path.join(os.path.dirname(__file__), '../'))

import datetime

import subprocess 
from subprocess import *
import time
from threading import Thread

from queue import Queue, Empty

import mysql.connector
from mysql.connector import errorcode
from db import * 

time.sleep(1) #wait for mysql starts

maxIdx = 19

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
idx[15] = 15
idx[16] = 16
idx[16] = 17
idx[16] = 18

arg[1] = ""
arg[2] = "0"
arg[3] = "1"
arg[4] = "2"
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
arg[15] = "192.168.178.75"
arg[16] = "3"
arg[17] = "4"
arg[18] = "5"


proc[1] = "p_server_queue"
proc[2] = "scheduler"
proc[3] = "scheduler"
proc[4] = "scheduler"
proc[5] = "sendMail"
proc[6] = "p_cmd_in"
proc[7] = "p_cmd_out"
proc[8] = "p_cmd_get"
proc[9] = "p_serial"
proc[10] = "setHistoryData"
proc[11] = "getDirectUpdate"
proc[12] = "setHistoryDataStatistic"
proc[13] = "syncTime"
proc[14] = "lightmanagerlogin"
proc[15] = "lightmonitor"
proc[16] = "scheduler"
proc[17] = "scheduler"
proc[18] = "scheduler"

status_text = ""
dbstarted = False

while dbstarted == False: # wait until mysql server is not started
	time.sleep(2)
	# -- DB Connection ---------------------------
	try:
		dbstarted = True
		db = mysql.connector.connect(**config)		
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
		print("starting:",p[num]) 
		p[num] = initProcess(num)
		t[num] = Thread(target=enqueue_output, args=(p[num].stdout, q, num))
		t[num].daemon = True
		t[num].start()
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
		print("started:",p[num]) 		

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

def exec_ss_15():
	startProc(15)		
	
def exec_ss_16():
	startProc(16)
	
def exec_ss_16():
	startProc(17)
	
def exec_ss_16():
	startProc(18)		
		
def log(t, m):
	sql = "insert into tblog (type,msg) VALUES (%s, %s)" 
	cur.execute(sql, (t,m))
	db.commit()
	cur.close		

def initProcess(id):
	try:
		px = Popen([cmd, p_path + proc[id] + ".py", str(arg[id])], shell=False, stdin=None, stderr=None, stdout=PIPE, bufsize=1)
		pid = px.pid
		print("pid:", pid)
		cur.execute("UPDATE tbproctl SET sts = %s WHERE id = %s", (pid, 100+id))
		db.commit()	
		#setOutput("Starting " + proc[index], px)
		return px
	except:
		a = 0
		
def setProcess(index):
	px = Popen([cmd, p_path + proc[index] + ".py", str(arg[index])], shell=False, stdin=None, stderr=None, stdout=PIPE, bufsize=1)
	pid = px.pid
	cur.execute("UPDATE tbproctl SET sts = %s WHERE id = %s", (pid, 100+index))
	db.commit()
	# obj = self.builder.get_object('statusbar')
	#setOutput("Starting " + proc[index], obj)
	return px

def killProc(index, p):
	try:
		cur.execute("UPDATE tbproctl SET sts = %s WHERE id = %s", (0, 100+id))
		db.commit()
		time.sleep(3)
		return
	except:
		a = 0
	p.kill()
	#obj = self.builder.get_object('statusbar')
	setOutput("Stopping " + proc[index], obj)
	return 0

def checkStatus(index, pindex):
	sts = 0
	ts = time.time()
	timestamp = datetime.datetime.fromtimestamp(ts).strftime('%Y-%m-%d %H:%M:%S')
	if pindex.poll() is not None:
		sts = 9			
	else:
		sts = 1		
	try:
		cur.execute("UPDATE tbproctl SET sts = %s, lastupdate = %s WHERE id = %s", (sts, timestamp ,100+index))
		db.commit()
		return
	except (MySQLdb.Error, MySQLdb.Warning):
		print(e)
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
	print(status_text)


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
			updateOutput(index, line)	
	except Empty:
		a = 1

	for num in range(1, maxIdx):
		checkStatus(num, p[num])

#	self.mainwindow.after(10, self.checkProc)		
begin()	
while True:
	for num in range(1, maxIdx):
		checkStatus(num, p[num])
	time.sleep(30)		
