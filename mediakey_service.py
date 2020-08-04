#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import subprocess

from gi.repository import Gio

# include for debug

import time
import dbus

"""
fn key service
"""
class MediakeyService:

    def __init__(self):
        self.screenStatus = True
        self.wlanStatus = True
        self.bluetoothStatus = True

#        self.systemDbus = dbus.SystemBus()
#        self.obj = self.systemDbus.get_object('cn.kylinos.KylinDevAPI', '/')
#        self.iface = dbus.Interface(self.obj, 'cn.kylinos.KylinDevAPI')

    """
    open close monitor
    
    @return: no return
    """
    def screenToggle(self):
        # Use xrandr has problem, screen is black but the mouse is still show
        if self.screenStatus:
#            cmd = "xrandr --output " + currentMonitor + "--brightness 0"
            cmd = "gsettings set org.mate.power-manager brightness-ac 0"
            self.screenStatus = False
        else:
#            cmd = "xrandr --output " + currentMonitor + "--brightness 1"
            cmd = "gsettings set org.mate.power-manager brightness-ac 99"
            self.screenStatus = True
        os.system(cmd)


    """
    open close touchpad
    @return: no return
    touchpad's status control by two keys, so we don't care current status
    """
    def touchpadToggle(self, status):
        if status:
            cmd = "gsettings set org.mate.peripherals-touchpad touchpad-enabled true"
        else:
            cmd = "gsettings set org.mate.peripherals-touchpad touchpad-enabled false"
        os.system(cmd)


    """
    get current brightness
    @return: double
    """
    def _getCurrentBrightness(self):
        giosss = Gio.SettingsSchemaSource.get_default()
        schema = giosss.lookup('org.mate.power-manager', True)
        if not schema:
            return None

        settings = Gio.Settings(settings_schema=schema)
        value = settings.get_double('brightness-ac')
        return value


    """
    screen brightness increase
    @return: no return
    brightn 1 - 100, total 10 level
    """
    def brightnessIncrease(self):
        current = _getCurrentBrightness()
        if not current:
            return

        # current = 83, level = 90; current = 9, level = 10
        level = int(current / 10) + 1

        value = level * 10 if level > 0 and level <= 10 else current

        cmd = "gsettings set org.mate.power-manager brightness-ac " + value
        os.system(cmd)


    """
    screen brightness decrease
    @return: no return
    brightn 1 - 100, total 10 level
    """
    def brightnessDecrease(self):
        current = _getCurrentBrightness()
        if not current:
            return

        # current = 83, level = 8; current 40, level = 3
        level  = int(current / 10) if int(current) % 10 > 0 else int(current / 10) - 1

        value = level * 10 if level > 0 else 1

        cmd = "gsettings set org.mate.power-manager brightness-ac " + value
        os.system(cmd)


    """
    get current wlan or bluetooth status
    @return: bool
    """
    def _getCurrentWirelessStatus(self, device):
        cmd = "rfkill list " + device + " | grep 'Soft blocked:' | head -n 1"

        handle = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
        handle.wait()
        res = handle.communicate()[0]
        res = res.decode()

        if 'no' in res:
            return True
        else:
            return False


    """
    wlan enable/disable toggle
    @return: no return
    """
    def wlanToggle(self):
        wlanKey = "wlan"
        if self._getCurrentWirelessStatus(wlanKey):
            self.wlanStatus = False
            cmd = "rfkill block " + wlanKey
        else:
            self.wlanStatus = True
            cmd = "rfkill unblock " + wlanKey
        dbusCmd = "dbus-send --system \
                             --type=method_call \
                             --dest=cn.kylinos.KylinDevAPI / cn.kylinos.KylinDevAPI.root_run_cmd \
                                    string:'" + cmd + "'"
        os.system(dbusCmd)
#        interface.root_run_cmd(cmd)


    """
    wlan enable/disable toggle
    @return: no return
    """
    def bluetoothToggle(self):
        btKey = "bluetooth"
        if self._getCurrentWirelessStatus(btKey):
            self.bluetoothStatus = False
            cmd = "rfkill block " + btKey
        else:
            self.bluetoothStatus = True
            cmd = "rfkill unblock " + btKey

        dbusCmd = "dbus-send --system \
                             --type=method_call \
                             --dest=cn.kylinos.KylinDevAPI / cn.kylinos.KylinDevAPI.root_run_cmd \
                                    string:'" + cmd + "'"
        os.system(dbusCmd)

#        interface.root_run_cmd(cmd)

    """
    camera open/close toggle, open cheese?
    @return: no return
    """
    def cameraToggle(self):
        pass



def main():
    # app = QApplication(sys.argv)

    w = MediakeyService()
    w.screenToggle()

    # sys.exit(app.exec_())


if __name__ == '__main__':
    main()

