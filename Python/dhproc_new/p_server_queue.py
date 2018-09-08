from multiprocessing import Queue
from multiprocessing.managers import SyncManager

HOST = ''
PORT0 = 5011
PORT1 = 5012
PORT2 = 5013
PORT3 = 5014
PORT4 = 5015
PORT5 = 5016
PORT6 = 5017
PORT7 = 5018
AUTHKEY = str("123456").encode("utf-8")

description = 'Queue Server'

def CreateQueueServer(HOST, PORT, AUTHKEY, name = None, description = None):
    name = name
    description = description
    q = Queue()

    class QueueManager(SyncManager):
        pass

    QueueManager.register('get_queue', callable = lambda: q)
    QueueManager.register('get_name', callable = name)
    QueueManager.register('get_description', callable = description)
    manager = QueueManager(address = (HOST, PORT), authkey = AUTHKEY)
    manager.start() # This actually starts the server

    return manager

# Start three queue servers
qIn = CreateQueueServer(HOST, PORT0, AUTHKEY, 'qIn', description)
qOut = CreateQueueServer(HOST, PORT1, AUTHKEY, 'qOut', description)
qSql = CreateQueueServer(HOST, PORT2, AUTHKEY, 'qSql', description)
qResp = CreateQueueServer(HOST, PORT3, AUTHKEY, 'qResp', description)
qQuery = CreateQueueServer(HOST, PORT4, AUTHKEY, 'qQuery', description)
qResult = CreateQueueServer(HOST, PORT5, AUTHKEY, 'qResult', description)
qQuery2 = CreateQueueServer(HOST, PORT6, AUTHKEY, 'qQuery2', description)
qResult2 = CreateQueueServer(HOST, PORT7, AUTHKEY, 'qResult2', description)
input("return to end")
