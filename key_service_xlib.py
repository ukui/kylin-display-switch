#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
from PyQt5.QtCore import QThread, pyqtSignal
from Xlib import X, XK, display
from Xlib.ext import record
from Xlib.protocol import rq


# python X global event， need python-xlib
class KeyServiceXlib(QThread):

    local_dpy = display.Display()
    record_dpy = display.Display()
    main_window = None
    ctx = None
    is_modify_key_press = False
    is_active = False

    signal_switch_select = pyqtSignal()
    signal_switch_confirm = pyqtSignal()
    signal_tip_capslock = pyqtSignal()
    signal_tip_numlock = pyqtSignal()

    def __init__(self, main_window):
        QThread.__init__(self)
        self.main_window = main_window

        self.signal_switch_select.connect(self.main_window.slot_switch_select)
        self.signal_switch_confirm.connect(self.main_window.slot_switch_confirm)
        self.signal_tip_capslock.connect(self.main_window.slot_tip_capslock)
        self.signal_tip_numlock.connect(self.main_window.slot_tip_numlock)

    def run(self):
        self.check_record_extension()

        # Create a recording context; we only want key and mouse events
        self.ctx = self.record_dpy.record_create_context(
            0,
            [record.AllClients],
            [{
                'core_requests': (0, 0),
                'core_replies': (0, 0),
                'ext_requests': (0, 0, 0, 0),
                'ext_replies': (0, 0, 0, 0),
                'delivered_events': (0, 0),
                'device_events': (X.KeyPress, X.MotionNotify),
                'errors': (0, 0),
                'client_started': False,
                'client_died': False,
            }])

        # Enable the context; this only returns after a call to record_disable_context,
        # while calling the callback function in the meantime
        self.record_dpy.record_enable_context(self.ctx, self.record_callback)

        # Finally free the context
        self.record_dpy.record_free_context(self.ctx)

    def check_record_extension(self):
        if not self.record_dpy.has_extension("RECORD"):
            print("RECORD extension not found")
            sys.exit(1)

    def lookup_keysym(self, keysym):
        for name in dir(XK):
            if name[:3] == "XK_" and getattr(XK, name) == keysym:
                return name[3:]
        else:
            return keysym

    def record_callback(self, reply):
        if reply.category != record.FromServer:
            return
        if reply.client_swapped:
            return
        if not len(reply.data) or reply.data[0] < 2:
            return

        data = reply.data
        while len(data):
            event, data = rq.EventField(None).parse_binary_value(data, self.record_dpy.display, None, None)

            if event.type in [X.KeyPress, X.KeyRelease]:
                keysym = self.local_dpy.keycode_to_keysym(event.detail, 0)

                if event.type == X.KeyPress:
                    if keysym == XK.XK_Super_L:
                        self.is_modify_key_press = True
                    if keysym == XK.XK_F3 or keysym == XK.XK_F7 or keysym == XK.XK_p:
                        if self.is_modify_key_press == True:
                            self.is_active = True
                            self.signal_switch_select.emit()

                    # dev, press ESC to exit
                    # if keysym == XK.XK_Escape:
                    #     self.local_dpy.record_disable_context(self.ctx)
                    #     self.local_dpy.flush()
                    #     sys.exit(0)

                elif event.type == X.KeyRelease:
                    if(keysym == XK.XK_Super_L):
                        self.is_modify_key_press = False
                        if(self.is_active == True):
                            self.is_active = False
                            self.signal_switch_confirm.emit()

                    if keysym == XK.XK_Caps_Lock:
                        self.signal_tip_capslock.emit()
                    if keysym == XK.XK_Num_Lock:
                        self.signal_tip_numlock.emit()


def main():
    # app = QApplication(sys.argv)
    # sys.exit(app.exec_())
    k = KeyServiceXlib(None)
    k.run()


if __name__ == '__main__':
    main()
