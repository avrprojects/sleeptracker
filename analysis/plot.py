# -*- coding: utf-8 -*-

import collections as col
import sys

import numpy as np
import serial

import matplotlib
matplotlib.use("GTKAgg")
from matplotlib import pyplot

x = np.arange(200)

y1 = col.deque(maxlen=len(x))
y2 = col.deque(maxlen=len(x))
y3 = col.deque(maxlen=len(x))

for i in x:
    y1.append(0)
    y2.append(0)
    y3.append(0)

port = serial.Serial(sys.argv[1],
                     baudrate=57600,
                     bytesize=serial.EIGHTBITS,
                     parity=serial.PARITY_NONE,
                     stopbits=serial.STOPBITS_ONE,
                     xonxoff=False,
                     rtscts=False,
                     timeout=1)

if not port.isOpen():
    print "Failed to open serial port."
    sys.exit(1)

figure = pyplot.figure(figsize=(8, 3))

line1, = pyplot.plot(x, y1, label="X")
line2, = pyplot.plot(x, y2, label="Y")
line3, = pyplot.plot(x, y3, label="Z")

pyplot.title('Timeline')
pyplot.xlabel('Sample')
pyplot.ylabel('Force [ADC Counts]')
pyplot.legend(loc='upper center', ncol=3)
pyplot.ylim(-512, 512)
pyplot.ion()

try:
    while port.isOpen():
        port.write("\x00")
        try:
            line = port.readline()
            x, y, z = map(int, line.strip().split())
        except KeyboardInterrupt:
            raise
        except:
            continue

        y1.append(x)
        y2.append(y)
        y3.append(z)
        line1.set_ydata(y1)
        line2.set_ydata(y2)
        line3.set_ydata(y3)

        pyplot.draw()
except:
    pass

pyplot.ioff()
pyplot.close(figure)
