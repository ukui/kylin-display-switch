#!/usr/bin/python
# -*- coding: utf-8 -*-

from PyQt5.QtCore import pyqtSignal


class Signals:
    pass
    # signal_switch_select = pyqtSignal("signal-switch-select")
    # signal_switch_confirm = pyqtSignal("signal-switch-confirm")
    # signal_tip_capslock = pyqtSignal("signal-tip-capslock")
    # signal_tip_numlock = pyqtSignal("signal-tip-numlock")

class DisplayMode:
    MODE_ONLY_ORI = 1
    MODE_CLONE = 2
    MODE_EXTEND = 3
    MODE_ONLY_OUT = 4
