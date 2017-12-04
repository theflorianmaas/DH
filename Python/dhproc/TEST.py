import multiprocessing
import time
from ctypes import *
from multiprocessing import *

# shared obj class
class Struct(Structure):
    _fields_ = [('stringa', c_wchar_p), ('val', c_int) ]

def worker(arr):
	arr[0].stringa = "q"
	while True:
		name = multiprocessing.current_process().name
		#print (name, 'Starting')
		time.sleep(1)
		#arr[0] = arr[0] + 1	
		arr[0].val = arr[0].val + 1
		#y = arr[0].stringa + "x"
		arr[0].stringa = arr[0].stringa + "x"
		#print (arr[0].stringa)
		#print (y)

def worker1(arr):
	while True:
		name = multiprocessing.current_process().name
		print (name, 'Starting')
		time.sleep(2)
		print (name, 'Exiting')
		print("worker1:", arr[0].val)	
		#print("XXXX worker1:", len(arr[0].stringa))			

def my_service(arr):
	while True:
		name = multiprocessing.current_process().name
		print (name, 'Starting')
		time.sleep(1)
		print (name, 'Exiting')
		print("myservice:", arr[0].val)		
		print("XXXX myservice:", arr[0].stringa)		

if __name__ == '__main__':
	arr = multiprocessing.Array(Struct, [Struct() for i in range( 1 )] )
	service = multiprocessing.Process(name='my_service', target=my_service, args=(arr,))
	worker_1 = multiprocessing.Process(name='worker 1', target=worker1, args=(arr,))
	worker_2 = multiprocessing.Process(target=worker, args=(arr,)) # use default name

	worker_1.start()
	worker_2.start()
	service.start()