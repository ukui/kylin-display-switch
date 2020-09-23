#!/usr/bin/python3
# -*- coding: utf-8 -*-

from evdev import InputDevice
from select import select
import dbus
import os
import time
from datetime import datetime

import enums as Keys



system_bus = dbus.SystemBus()
obj = system_bus.get_object('com.kylin.display.switch', '/')
interface = dbus.Interface(obj, 'com.kylin.display.switch.interface')

devname = ""


if os.path.exists("/proc/bus/input/devices"):
    devfd = open("/proc/bus/input/devices", "r")
    devlist = devfd.readlines()
    devlist2 = "".join(devlist).split("\n\n")
    for dev in devlist2:
        if  "Sysfs" in dev and "keyboard" in dev.lower():
            for line in filter(lambda str : str.startswith("H: Handlers="), dev.split("\n")):
                for unit in line.split(" "):
                    if unit.startswith("event"):
                        devname = "/dev/input/" + unit

if not devname:
    exit(1)


time.sleep(1)

dev = InputDevice(devname)

pressTime = None
releaseTime = None
longPressTime = None

"""
run loop.

"""
while True:
    r, w, x = select([dev], [], [])
    for event in dev.read():
        if (event.type == 1):  # vaild key event
            print(event.code)
            if event.code in Keys.keyDic.values():
                # The Key Value 1 Is Means Press
                if event.value == 1:
                    if pressTime is None:
                        pressTime = datetime.now()
                # The Key Value 0 Is Means Release
                if event.value == 0:
                    if releaseTime is None:
                        releaseTime = datetime.now()
                # The Key Value 2 Is Means Long Press
                if event.value == 2:
                    longPressTime = datetime.now()

                if releaseTime and pressTime:
                    diffTime = (releaseTime - pressTime).seconds
                    # emit dbus signal
                    interface.emitKeyTrigger(event.code, -1)
                    # reset time
                    pressTime = None
                    releaseTime = None
                    longPressTime = None
