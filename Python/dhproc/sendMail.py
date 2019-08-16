#!/usr/bin/python
# -*- coding: utf-8 -*-
# Ver 16 - 31 July 2019 -
# set password 
## update users set user_email_password_hash =
## AES_ENCRYPT('password', user_name)
## where user_id = 1

from email.mime.text import MIMEText
from email.header import Header
import smtplib
import time
import string
import sys
import mysql.connector
from mysql.connector import errorcode
from db import * 
#import datetime
from datetime import datetime

ti = ['z','z','z','z','z','z','z','z','z','z','z','z','z','z','z','z','z','z','z','z']
td = ['z','z','z','z','z','z','z','z','z','z','z','z','z','z','z','z','z','z','z','z']

def output(x):
	print(str(datetime.now().time())[:8]+ " "+ str(x))
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

cur = db.cursor()
sql0 = "delete from tbsendemail" 
cur.execute(sql0)
cur.execute("commit")

# Function initSNTP(): #get email data for sending alerts
def initSNTP():
	global smtpserver
	global login 
	global password 
	global recipient
	#get email credentials
	cur.execute("select user_email, CAST(AES_DECRYPT(user_email_password_hash, user_name) AS CHAR(10000) CHARACTER SET utf8) from users;")
	row = cur.fetchone()
	login = row[0]
	password = row[1]
	#print ("Set email paramps for "+password)
	#cur.execute("commit")
	sql = "Select user_email from users where user_id = 1"
	cur.execute(sql)
	for i in range(cur.rowcount):
		row = cur.fetchone()
		recipient = row[0]
	#print(recipient)
	cur.close	

def sendemail(n,s,b):
    global login
    global password
    global recipient
    #print(recipient)
    #global smtpserver
    #idx = ti.index(x)
    from_addr    = 'udoo@myHomeMonitoring.it'
    to_addr_list = [recipient]
    cc_addr_list = ['zzz@xxx.cc'],
    subject      = 'Allarme sensore '+ s, #.decode("utf-8"), 
    message      = b
    header  = 'From: %s\n' % from_addr
    header += 'To: %s\n' % ','.join(to_addr_list)
    #header += 'Cc: %s\n' % ','.join(cc_addr_list)
    header += 'Subject: %s\n\n' % subject
    message = header + message
    smtpserver = smtplib.SMTP("smtp.gmail.com",587)
    smtpserver.ehlo()
    smtpserver.starttls()
    x = smtpserver.login(login, password)
    problems = smtpserver.sendmail(from_addr, to_addr_list, message) #.encode('utf-8'))
    smtpserver.quit()
    output (problems)
    return problems
    
def stripString(s):
	s.strip('bytearray')
	s.strip("'b'")
	s.strip("'")
	s.strip("(")
	s.strip(")")
	print (s)
	return s    

initSNTP()

while True:		
	sql = "SELECT tbtrigger_id,date,node,event,sensor,actuator,action,room,currentvalue FROM tbsendemail WHERE sent = 0";
	cur.execute(sql)
	for i in range(cur.rowcount):
		row = cur.fetchone()
		datetime = row[1]
		node = row[2]
		event = row[3]
		sensor = row[4]
		actuator = row[5]
		action = row[6]
		room = row[7]
		val = row[8]
		body = "Scattato allarme in " + stripString(str(room)) + " (nodo "+ stripString(str(node)) + ") per "+ stripString(str(event)) + str(" del sensore ") + stripString(str(sensor)) + str(" alle ore ") + stripString(str(datetime)) +str(" valore = ")+ stripString(str(val))
		#body = MIMEText(body.encode('utf-8'), _charset='utf-8')
		output (body)
		ret = sendemail(room,sensor,body)
		output (ret)
		if str(ret) == '{}':
			cur.execute("commit")
			cur.execute("UPDATE tbsendemail SET sent = 1 WHERE tbTrigger_id = "+str(row[0]))
			output (body)
	cur.execute("commit")
	cur.close	
	time.sleep(5)
