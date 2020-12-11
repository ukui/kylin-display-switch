#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
from PyQt5.QtCore import QThread, pyqtSignal

import dbus
import dbus.mainloop.glib
from gi.repository import GObject

from mediakey_service import MediakeyService

"""
dbus service for fn key
"""
class KeyServiceDbus(QThread):

    signal_key_trigger = pyqtSignal(int, int)

    def __init__(self, main_window):
        QThread.__init__(self)
        self.main_window = main_window

        self.signal_key_trigger.connect(self.main_window.slot_mediakey_trigger)

        self.mediakey_service = MediakeyService()

    """
    run loop
    """
    def run(self):

        dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)

        # new dbus object & iface
        self.system_bus = dbus.SystemBus()
        self.obj = self.system_bus.get_object('com.kylin.display.switch', '/')
#        self.interface = dbus.Interface(self.obj, 'com.kylin.display.switch.interface')

        # Connect Dbus Signal
        self.obj.connect_to_signal("KeyTrigger", self.trigger_handler, dbus_interface="com.kylin.display.switch.interface")

        mainloop = GObject.MainLoop()
        mainloop.run()

    """
    handle the dbus action.

    @param param1: key code
    @param param2: key value
    @return: no return
    """
    def trigger_handler(self, keyCode, keyValue):
        """
           keyCode: Which key is it
           keyValue: The key is pressed or released
        """

        # F6 trigger and toggle the screen status, open or close
        if (keyCode == 431):
            self.mediakey_service.screenToggle()

        self.signal_key_trigger.emit(keyCode, keyValue)

