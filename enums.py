#!/usr/bin/python
# -*- coding: utf-8 -*-

from PyQt4.QtCore import SIGNAL


class Signals:
    signal_switch_select = SIGNAL("signal-switch-select")
    signal_switch_confirm = SIGNAL("signal-switch-confirm")
    signal_tip_capslock = SIGNAL("signal-tip-capslock")
    signal_tip_numlock = SIGNAL("signal-tip-numlock")

class DisplayMode:
    MODE_ONLY_ORI = 1
    MODE_CLONE = 2
    MODE_EXTEND = 3
    MODE_ONLY_OUT = 4
