from bluetooth import BluetoothSocket, RFCOMM


def parsa_data(data):
    print (data)
    #while ("{" in data and "}" in data):
    #    print (data[data.find('{'):data.find('}')])
    #    data = data[data.find('}'):]
    return data


sock = BluetoothSocket(RFCOMM)
sock.connect(("98:D3:32:30:AB:5C", 1))

data = ""
#while True:
data += sock.recv(160)
data = parsa_data(data)

sock.close()
