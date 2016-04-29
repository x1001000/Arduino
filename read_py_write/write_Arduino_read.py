import serial
import time
with serial.Serial('COM11', 9600, timeout = 1) as ser:
    time.sleep(2)
    while True:
        s_out = raw_input('Send a string to Arduino to do something:')
        if s_out == '':
            break
        ser.write(s_out)
        s_in = ser.read(len(s_out))
        print s_in
