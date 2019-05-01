import serial

ser = serial.Serial('/dev/ttyACM0',9600)
while(True):
    _input = input()
    ser.write(_input.encode())