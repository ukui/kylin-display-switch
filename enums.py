#!/usr/bin/python
# -*- coding: utf-8 -*-
# Copyright (C) 2017, Tianjin KYLIN Information Technology Co., Ltd.

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 3 as
# published by the Free Software Foundation.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


class DisplayMode:
    MODE_ONLY_ORI = 1
    MODE_CLONE = 2
    MODE_EXTEND = 3
    MODE_ONLY_OUT = 4

# The Key Code 431 is Fn + F6
keyDic = { "muteOnOff"    :  113, \
           "volumeDown"    :  114, \
           "volumeUp"    : 115, \
           "screenOnOff"    :  431, \
           "brightnessDown"    :  224, \
           "brightnessUp"    : 225, \
           "cameraOnOff"    : 212, \
           "micmuteOnOff"    : 248, \
           "bluetoothOnOff"    :  237, \
           "wlanOnOff"    :  238, \
           "touchpadOnOff"  :  530, \
           "touchpadOn"   :  531, \
           "touchpadOff"    :  532, \
           "videoModeSwitch"    :  227, \
           "debugKeyCode1" :  65, \
           "debugKeyCode2" :  66, \
}
