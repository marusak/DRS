import signal
from bluetooth import BluetoothSocket, RFCOMM


def parsa_data(data):
    while ("{" in data and "}" in data):
        print (data[data.find('{'):data.find('}')+1])
        data = data[data.find('}')+1:]
    return data

def finish_up(signum, frame):
    signal.signal(signal.SIGINT, original_sigint)
    sock.close()

original_sigint = signal.getsignal(signal.SIGINT)
signal.signal(signal.SIGINT, finish_up)
sock = BluetoothSocket(RFCOMM)
sock.connect(("98:D3:32:30:AB:5C", 1))

data = sock.recv(165)
data = data[data.find('}')+1:]
while True:
    data += sock.recv(165)
    data = parsa_data(data)

print(data)

sock.close()
