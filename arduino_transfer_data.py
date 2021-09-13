import serial
import time
import serial.tools.list_ports

# speeds = ['1200', '2400', '4800', '9600', '19200', '38400', '57600', '115200']
port = []

def serial_ports():
    result = []
    for i in serial.tools.list_ports.comports ():
        # можно использовать, если хотим видеть в меню лишь названия COM1, COM2, COM3 ....
        # result.append(str(i).split(" ")[0])
        result.append(i)
    return result

def choose_com_port(com_port):
    port.clear()
    port.append(com_port)
    return port

def disp_com_port():
    return print("".join(port))

def enroll_person(id):
    arduino = serial.Serial("".join(port), 9600)
    time.sleep(2)
    data = 0
    data = arduino.write('1'.encode())
    time.sleep(2)
    data = arduino.write(id.encode())
    arduino.close()

def delete_person(id):
    arduino = serial.Serial("".join(port), 9600)
    time.sleep(2)
    data = 0
    data = arduino.write('2'.encode())
    time.sleep(2)
    data = arduino.write(id.encode())
    arduino.close()

def drop_arduino_database():
    arduino = serial.Serial("".join(port), 9600)
    time.sleep(2)
    data = 0
    data = arduino.write('3'.encode())
    arduino.close()
