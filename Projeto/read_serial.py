import serial

ser = serial.Serial('/dev/ttyUSB0',1000000 )
#ser.write('5')
#ser.write(b'5') #Prefixo b necessario se estiver utilizando Python 3.X
while(True):
    _str = ser.read().decode()
    print(_str,end='')