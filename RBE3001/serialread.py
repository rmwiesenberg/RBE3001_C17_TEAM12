import serial
import math

avr = serial.Serial('/dev/ttyUSB0', '115200')
print("Started Serial")

h = 140
a2 = 160
a1 = 155

py = -40

while 1:
  line = avr.readline().decode("utf-8")
  print(line)
