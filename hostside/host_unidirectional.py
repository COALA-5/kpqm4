#!/usr/bin/env python3
import serial
import sys
import platform
import os

if platform.system() == "Darwin":
    dev = serial.Serial("/dev/tty.usbserial-0001", 38400)
else:
    files = os.listdir("/dev")
    ACM = [x for x in files if "ACM" in x][0]
    print("found ACM:", ACM)
    dev = serial.Serial(f"/dev/{ACM}", 38400)

print("> Returned data:", file=sys.stderr)

while True:
    x = dev.read()
    sys.stdout.buffer.write(x)
    sys.stdout.flush()
