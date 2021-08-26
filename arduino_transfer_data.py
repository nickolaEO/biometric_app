import serial
import time

#print("Connection to " + port + " established succesfully!\n")


def enroll_person(id):
    port = "COM3"
    arduino = serial.Serial(port, 9600)
    time.sleep(2)
    data = 0
    data = arduino.write('1'.encode())
    time.sleep(2)
    data = arduino.write(id.encode())
    arduino.close()

def delete_person(id):
    port = "COM3"
    arduino = serial.Serial(port, 9600)
    time.sleep(2)
    data = 0
    data = arduino.write('2'.encode())
    time.sleep(2)
    data = arduino.write(id.encode())
    arduino.close()

def drop_arduino_database():
    port = "COM3"
    arduino = serial.Serial(port, 9600)
    time.sleep(2)
    data = 0
    data = arduino.write('3'.encode())
    arduino.close()
