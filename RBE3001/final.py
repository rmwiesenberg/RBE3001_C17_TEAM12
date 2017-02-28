import serial
import math
import time

avr = serial.Serial('/dev/ttyUSB0', '115200')
ard = serial.Serial('/dev/ttyACM0', '115200')
print("Started Serials")

state = 0

h = 140
a2 = 145
a1 = 150

py = -30

low = []

def scaleIR(ir):
    ir[0] = 1.31579*ir[0] + 14.211
    ir[1] = 4.16667*ir[1] - 11.667
    return ir

while 1:
    line = ard.readline().decode("utf-8")
    ir = scaleIR(list(map(int, line.split(","))))
    if state == 0:
        if ir[0] < 170:
            low.append(ir[0])
            state = 1
    elif state == 1:
        if ir[0] < 170:
            low.append(ir[0])
        else:
            px = min(low)
            print(px)
            low = []
            state = 2
    elif state == 2:
        c2 = (px**2 + py**2 - a2**2 -a1**2)/(2*a1*a2)
        s2 = math.sqrt(1-c2**2)
        theta2_1 = math.atan2(c2,s2)
        theta2_2 = math.atan2(c2,-s2)
        if theta2_1 > theta2_2:
            theta2 = theta2_2
            s2 = -s2
        else:
            theta2 = theta2_1

        c1 = (px*(a2*c2 + a1) + py*a2*s2)/((a2*c2+a1)**2 + (a2*s2)**2);
        s1 = (py*(a2*c2 + a1) - px*a2*s2)/((a2*c2+a1)**2 + (a2*s2)**2);

        theta1 = math.atan2(c1,s1)

        print(theta1,theta2)

        state = 3

    elif state == 3:
        if ir[1] < 180:
            time.sleep(.1)
            t1d = str(int(theta1*180/math.pi))
            t2d = str(int(theta2*180/math.pi))

            print(t1d,t2d)

            avr.write(t1d.encode())
            avr.write(' '.encode())
            avr.write(t2d.encode())
            avr.write('\n'.encode())
            state = 0
