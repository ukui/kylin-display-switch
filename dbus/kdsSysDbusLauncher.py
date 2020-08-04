#!/usr/bin/python3
# -*- coding: utf-8 -*-

#import gobject
from gi.repository import GObject

import os
import dbus
import signal
import dbus.service
import dbus.mainloop.glib

import time

''' dbus exception '''
class DemoException(dbus.DBusException):
    _dbus_error_name = "com.kylin.display.switch.Exception"


''' kds dbus daemon for root functions '''
class BridgeDaemon(dbus.service.Object):
    INTERFACE = "com.kylin.display.switch.interface"
    def __init__(self, system_bus, mainloop):
        dbus.service.Object.__init__(self, system_bus, "/")
        self.mainloop = mainloop

    """
    test function.

    @param param1: in_signature
    @param param2: out_signature
    @return: no return
    """
    @dbus.service.method(INTERFACE, in_signature = '', out_signature = 'as')
    def HelloWorld(self):
        return ["Hello", "with unique name", bus.get_unique_name()]

    """
    key event triger.

    @param param1: in_signature
    @return: no return
    """
    @dbus.service.signal(INTERFACE, signature='ii')
    def KeyTrigger(self, keyCode, keyValue):
        """keyCode: Which key is it
           keyValue: The key is pressed or released"""
        pass
    """
    emit a signal.

    @param param1: in_signature
    @param param2: out_signature
    @return: no return
    """
    @dbus.service.method(INTERFACE, in_signature='ii', out_signature = '')
    def emitKeyTrigger(self, keyCode, keyValue):
        self.KeyTrigger(keyCode, keyValue)

    """
    exit function.

    @param param1: in_signature
    @param param2: out_signature
    @return: no return
    """
    @dbus.service.method(INTERFACE, in_signature = '', out_signature = '')
    def Exit(self):
        self.mainloop.quit()


if __name__ == '__main__':
    os.environ["TERM"] = "xterm"
    os.environ["PATH"] = "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/X11R6/bin"

    dbus.mainloop.glib.DBusGMainLoop(set_as_default = True)
    #mainloop = gobject.MainLoop()
    GObject.threads_init()
    mainloop = GObject.MainLoop()

    bus = dbus.SystemBus()
    name = dbus.service.BusName("com.kylin.display.switch", bus)
    signal.signal(signal.SIGINT, lambda : mainloop.quit())

    obj = BridgeDaemon(bus, mainloop)

#    print("Running example_service...")
    mainloop.run()
