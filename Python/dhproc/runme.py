#!/usr/bin/python
# -*- coding: utf-8 -*-
# Ver 16 - 23 March 2017 -

import sys
import os
from os import system

try:
    import tkinter as tk
    from tkinter import messagebox
except:
    import Tkinter as tk
    import tkMessageBox as messagebox

sys.path.append(os.path.join(os.path.dirname(__file__), '../'))

import pygubu

import subprocess 
from subprocess import *
import time
from threading import Thread

from queue import Queue, Empty

import mysql.connector
from mysql.connector import errorcode
from db import * 

maxIdx = 10

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

arg[1] = "0"
arg[2] = "0"
arg[3] = "0"
arg[4] = "1"
arg[5] = "0"
arg[6] = "0"
arg[7] = "0"
arg[8] = "0"
arg[9] = "0"

proc[1] = "main"
proc[2] = "getExtTemp"
proc[3] = "scheduler"
proc[4] = "scheduler"
proc[5] = "setHistoryData"
proc[6] = "sendMail"
proc[7] = "setHistoryDataStatistic"
proc[8] = "syncTime"
proc[9] = "smartLights"

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
		
class Main:
	def __init__(self, master):
	
		for num in range(1, maxIdx):
			p[num] = self.initProcess(num)
			t[num] = Thread(target=self.enqueue_output, args=(p[num].stdout, q, num))
			t[num].daemon = True
			t[num].start()		

		#1: Create a builder
		self.builder = builder = pygubu.Builder()
		
		#2: Load an ui file
		builder.add_from_file(p_path + 'main.ui')
		
		#3: Create the widget using a master as parent
		self.mainwindow = builder.get_object('main', master)
		
		for num in range(1, maxIdx):
			label = self.builder.get_object('proc_' + str(num))
			label.insert("end", proc[num])
		
		for num in range(maxIdx, 10):	
			label = self.builder.get_object('proc_' + str(num))
			label.config(state="disabled")
			label = self.builder.get_object('proc_' + str(num) + '_sts')	
			label.config(bg='#D3D3D3')	
			btn = self.builder.get_object('btn_' + str(num))
			btn.config(state="disabled")		

		btn = self.builder.get_object('startall')
		btn.config(state="disabled")
		# Connect method callbacks
		builder.connect_callbacks(self)
				
	# define the method callbacks
	def exec_ss_1(self):
		num = 1
		if p[num].poll() is None: # it is running
			self.killProc(num, p[num])
		else:			
			p[num] = self.setProcess(num)
			p[num] = self.initProcess(num)
			t[num] = Thread(target=self.enqueue_output, args=(p[num].stdout, q, num))
			t[num].daemon = True
			t[num].start()	

	def exec_ss_2(self):
		num = 2
		if p[num].poll() is None: # it is running
			self.killProc(num, p[num])
		else:			
			p[num] = self.setProcess(num)
			p[num] = self.initProcess(num)
			t[num] = Thread(target=self.enqueue_output, args=(p[num].stdout, q, num))
			t[num].daemon = True
			t[num].start()

	def exec_ss_3(self):
		num = 3
		if p[num].poll() is None: # it is running
			self.killProc(num, p[num])
		else:			
			p[num] = self.setProcess(num)
			p[num] = self.initProcess(num)
			t[num] = Thread(target=self.enqueue_output, args=(p[num].stdout, q, num))
			t[num].daemon = True
			t[num].start()

	def exec_ss_4(self):
		num = 4
		if p[num].poll() is None: # it is running
			self.killProc(num, p[num])
		else:			
			p[num] = self.setProcess(num)
			p[num] = self.initProcess(num)
			t[num] = Thread(target=self.enqueue_output, args=(p[num].stdout, q, num))
			t[num].daemon = True
			t[num].start()

	def exec_ss_5(self):
		num = 5
		if p[num].poll() is None: # it is running
			self.killProc(num, p[num])
		else:			
			p[num] = self.setProcess(num)
			p[num] = self.initProcess(num)
			t[num] = Thread(target=self.enqueue_output, args=(p[num].stdout, q, num))
			t[num].daemon = True
			t[num].start()

	def exec_ss_6(self):
		num = 6
		if p[num].poll() is None: # it is running
			self.killProc(num, p[num])
		else:			
			p[num] = self.setProcess(num)
			p[num] = self.initProcess(num)
			t[num] = Thread(target=self.enqueue_output, args=(p[num].stdout, q, num))
			t[num].daemon = True
			t[num].start()

	def exec_ss_7(self):
		num = 7
		if p[num].poll() is None: # it is running
			self.killProc(num, p[num])
		else:			
			p[num] = self.setProcess(num)
			p[num] = self.initProcess(num)
			t[num] = Thread(target=self.enqueue_output, args=(p[num].stdout, q, num))
			t[num].daemon = True
			t[num].start()

	def exec_ss_8(self):
		num = 8
		if p[num].poll() is None: # it is running
			self.killProc(num, p[num])
		else:			
			p[num] = self.setProcess(num)
			p[num] = self.initProcess(num)
			t[num] = Thread(target=self.enqueue_output, args=(p[num].stdout, q, num))
			t[num].daemon = False
			t[num].start()			

	def exec_ss_9(self):
		num = 9
		if p[num].poll() is None: # it is running
			self.killProc(num, p[num])
		else:			
			p[num] = self.setProcess(num)
			p[num] = self.initProcess(num)
			t[num] = Thread(target=self.enqueue_output, args=(p[num].stdout, q, num))
			t[num].daemon = True
			t[num].start()		
			
	def p_start_all(self):
		for num in range(1, maxIdx):
			p[num] = self.setProcess(num)	
		btn = self.builder.get_object('startall')
		btn.config(state="disabled")	
		btn = self.builder.get_object('stopall')
		btn.config(state="enabled")		

	def p_stop_all(self):
		for num in range(1, maxIdx):
			self.killProc(num, p[num])
		btn = self.builder.get_object('stopall')
		btn.config(state="disabled")	
		btn = self.builder.get_object('startall')
		btn.config(state="enabled")	
	
	def log(t, m):
		sql = "insert into tblog (type,msg) VALUES (%s, %s)" 
		cur.execute(sql, (t,m))
		db.commit()
		cur.close		

	def initProcess(self, id):
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

	def checkStatus(self, index, pindex):
		if pindex.poll() is not None:
			sts = self.builder.get_object('proc_' + str(index) + '_sts')	
			sts.config(bg='#FF0000')			
		else:
			sts = self.builder.get_object('proc_' + str(index) + '_sts')	
			sts.config(bg='#00FF00')	
			
	def isNotInt(self, s):
		try:
			int(s)
			return False
		except ValueError:
			return True	

	def updateOutput(self, index, line):
		if self.isNotInt(line) is False:
			txt = line.rstrip().decode("utf-8")
		else:
			txt = str(line)
		label = self.builder.get_object('statusbar')
		self.setOutput(txt, label)
		label.delete("1.0", "end")
		label.insert("end", status_text)			
		lb = self.builder.get_object('lstmsg'+str(index))
		txt = (txt[:40] + '..') if len(txt) > 40 else txt
		lb.delete(0, "end")
		lb.insert("end", txt)
	
	def setOutput(self, txt, obj):
		global status_text
		global StatusBar
		status_text = txt + '\r\n' + status_text
		status_text = (status_text[:200] + '..') if len(status_text) > 200 else status_text	
		obj = self.builder.get_object('statusbar')
		obj.delete("1.0", "end")
		obj.insert("end", status_text)

	def enqueue_output(self, out, queue, idx):
		for line in iter(out.readline, b''):
			queue.put("thread")
			queue.put(idx)
			queue.put(line)
		#out.close()	
					
	def checkProc(self):
		try: 
			line = q.get_nowait()
			if line == "thread":
				index = q.get_nowait()
				line = q.get_nowait()
				self.updateOutput(index, line)	
		except Empty:
			a = 1

		for num in range(1, maxIdx):
			self.checkStatus(num, p[num])

		self.mainwindow.after(10, self.checkProc)		
	
if __name__ == '__main__':
	root = tk.Tk()
	root.geometry("690x400")
	root.resizable(width=False, height=False)
	app = Main(root)
	root.after(1, app.checkProc)
	root.mainloop()		
