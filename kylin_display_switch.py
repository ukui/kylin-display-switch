#!/usr/bin/python3
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

import os, sys, stat
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
import locale
import gettext
locale.setlocale(locale.LC_ALL, "")
gettext.bindtextdomain("kylin-display-switch", "/usr/share/locale")
gettext.textdomain("kylin-display-switch")
_ = gettext.gettext
from ui_mainwindow import Ui_MainWindow
from ui_mainwindow import Tips
from key_service_xlib import KeyServiceXlib
from display_service import DisplayService
from switchers_service import SwitchersService
from mediakey_service import MediakeyService
import fcntl

from gi.repository import Gio
from key_service_dbus import KeyServiceDbus

import enums as Keys

posBottom = 50


"""
get tip status
@return: no return
"""
def get_lock_tip_show():
    giosss = Gio.SettingsSchemaSource.get_default()
    schema = giosss.lookup('org.ukui.control-center.osd', True)
    if not schema:
        return True

    settings = Gio.Settings(settings_schema=schema)
    value = settings.get_boolean('show-lock-tip')
    return value


class KylinDisplaySwitch(QWidget):

    key_service = None
    display_service = None
    switchers_service = None
    settings = None
    current_button = 0

    key_dbus_service = None
    mediakey_service = None

    def __init__(self, parent = None):
        super(KylinDisplaySwitch, self).__init__(parent)

        self.check_singleton()

        self.init_ui()

        self.display_service = DisplayService()
        self.switchers_service = SwitchersService()
        self.mediakey_service = MediakeyService()
        self.settings = QSettings("kylinos.cn", "KylinDisplaySwitch")

        self.start_listen()
        self.start_listen_dbus()

    # singleton
    def check_singleton(self):
        homepath = os.path.expanduser('~')
        lockpath = "/tmp/instance_kds_" + homepath[homepath.rfind('/')+1:] + ".lock"
        
        if(os.path.exists(lockpath) == False):
            new_instance_file = open(lockpath, 'w')
            new_instance_file.close()
            os.chmod(lockpath, stat.S_IRWXU|stat.S_IRWXG|stat.S_IRWXO)

        self.instance_file = open(lockpath, 'w')
        try:
            fcntl.lockf(self.instance_file, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except IOError:
            print("only allow one instance...")
            sys.exit()

    def switch_window_type(self, big):
        qtVersion = int(QT_VERSION_STR.split('.')[1])

        if big:
            # main window radius
            rect = self.rect()
            rectf = QRectF(rect)
            blurPath = QPainterPath()
            blurPath.addRoundedRect(rectf, 24, 24)

            if qtVersion < 11:
                qtsf = QTransform()
                self.setProperty("blurRegion", QRegion(blurPath.toFillPolygon(qtsf).toPolygon()))
            else:
                self.setProperty("blurRegion", QRegion(blurPath.toFillPolygon().toPolygon()))

            self.setWindowOpacity(0.7)

        else:
            # main window radius
            rect = self.rect()
            rectf = QRectF(rect)
            blurPath = QPainterPath()
            blurPath.addRoundedRect(rectf, 4, 4)

            if qtVersion < 11:
                qtsf = QTransform()
                self.setProperty("blurRegion", QRegion(blurPath.toFillPolygon(qtsf).toPolygon()))
            else:
                self.setProperty("blurRegion", QRegion(blurPath.toFillPolygon().toPolygon()))

            self.setWindowOpacity(0.95)

    def init_ui(self):
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        self.setAttribute(Qt.WA_TranslucentBackground)
        self.setWindowFlags(Qt.FramelessWindowHint | Qt.ToolTip)

        self.ui.centralWidget.setAutoFillBackground(True)
        self.ui.centralWidget.setStyleSheet("#centralWidget{background-color: rgba(0,0,0,1); border-radius: 24px;}")

        self.switch_window_type(True)

        self.ui.lb_title.setText(_("Display Switch"))
        self.ui.lb_title.setStyleSheet("#lb_title{color:white; font-size: 22px;}")
        self.ui.lb_title.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)

        self.ui.lb_phone.setText(_("Control cellphone"))

        self.ui.lb_mode_1.setText(_("Computer"))
        self.ui.lb_mode_2.setText(_("Clone"))
        self.ui.lb_mode_3.setText(_("Extend"))
        self.ui.lb_mode_4.setText(_("Output"))
        self.ui.lb_mode_1.setAlignment(Qt.AlignLeft | Qt.AlignVCenter)
        self.ui.lb_mode_2.setAlignment(Qt.AlignLeft | Qt.AlignVCenter)
        self.ui.lb_mode_3.setAlignment(Qt.AlignLeft | Qt.AlignVCenter)
        self.ui.lb_mode_4.setAlignment(Qt.AlignLeft | Qt.AlignVCenter)

#        self.ui.lb_caps_on.setText(_("Caps on"))
#        self.ui.lb_caps_off.setText(_("Caps off"))
#        self.ui.lb_num_on.setText(_("Num on"))
#        self.ui.lb_num_off.setText(_("Num off"))

        self.ui.caps_on_bg.setStyleSheet("QWidget{border-image:url(res/capslockOn.png); border: none; border-radius: 8px;}")
        self.ui.caps_off_bg.setStyleSheet("QWidget{border-image:url(res/capslockOff.png); border: none; border-radius: 8px;}")
        self.ui.num_on_bg.setStyleSheet("QWidget{border-image:url(res/numlockOn.png); border:0px; border-radius: 8px;}")
        self.ui.num_off_bg.setStyleSheet("QWidget{border-image:url(res/numlockOff.png); border:0px; border-radius: 8px;}")
        self.ui.tp_on_bg.setStyleSheet("QWidget{border-image:url(res/touchpadOn.png); border:0px; border-radius: 8px;}")
        self.ui.tp_off_bg.setStyleSheet("QWidget{border-image:url(res/touchpadOff.png); border:0px; border-radius: 8px;}")
        self.ui.wlan_on_bg.setStyleSheet("QWidget{border-image:url(res/wlanOn.png); border:0px; border-radius: 8px;}")
        self.ui.wlan_off_bg.setStyleSheet("QWidget{border-image:url(res/wlanOff.png); border:0px; border-radius: 8px;}")
        self.ui.bluetooth_on_bg.setStyleSheet("QWidget{border-image:url(res/bluetoothOn.png); border:0px; border-radius: 8px;}")
        self.ui.bluetooth_off_bg.setStyleSheet("QWidget{border-image:url(res/bluetoothOff.png); border:0px; border-radius: 8px;}")

#        self.ui.lb_caps_on.setStyleSheet("QLabel{color:white;font-size:19px;}")
#        self.ui.lb_caps_off.setStyleSheet("QLabel{color:white;font-size:19px;}")
#        self.ui.lb_num_on.setStyleSheet("QLabel{color:white;font-size:19px;}")
#        self.ui.lb_num_off.setStyleSheet("QLabel{color:white;font-size:19px;}")
#        self.ui.lb_tp_on.setStyleSheet("QLabel{color:white;font-size:19px;}")
#        self.ui.lb_tp_off.setStyleSheet("QLabel{color:white;font-size:19px;}")
#        self.ui.lb_wlan_on.setStyleSheet("QLabel{color:white;font-size:19px;}")
#        self.ui.lb_wlan_off.setStyleSheet("QLabel{color:white;font-size:19px;}")
#        self.ui.lb_bluetooth_on.setStyleSheet("QLabel{color:white;font-size:19px;}")
#        self.ui.lb_bluetooth_off.setStyleSheet("QLabel{color:white;font-size:19px;}")

        self.ui.widget_1.setStyleSheet("#widget_1{background-color: rgba(255,255,255,0.05);}")
        self.ui.widget_3.setStyleSheet("#widget_3{background-color: rgba(255,255,255,0.05);}")

        self.ui.bg_mode_1.setStyleSheet("QWidget{border-image:url(res/btn1.png); border:0px;}")
        self.ui.bg_mode_2.setStyleSheet("QWidget{border-image:url(res/btn2.png); border:0px;}")
        self.ui.bg_mode_3.setStyleSheet("QWidget{border-image:url(res/btn3.png); border:0px;}")
        self.ui.bg_mode_4.setStyleSheet("QWidget{border-image:url(res/btn4.png); border:0px;}")

        self.ui.s_mode_1.setStyleSheet("QWidget{background-image:url(res/selected.png); border:0px;}")
        self.ui.s_mode_2.setStyleSheet("QWidget{background-image:url(res/selected.png); border:0px;}")
        self.ui.s_mode_3.setStyleSheet("QWidget{background-image:url(res/selected.png); border:0px;}")
        self.ui.s_mode_4.setStyleSheet("QWidget{background-image:url(res/selected.png); border:0px;}")

        self.ui.lb_mode_1.setStyleSheet("QLabel{color: white; font-size: 18px;}")
        self.ui.lb_mode_2.setStyleSheet("QLabel{color: white; font-size: 18px;}")
        self.ui.lb_mode_3.setStyleSheet("QLabel{color: white; font-size: 18px;}")
        self.ui.lb_mode_4.setStyleSheet("QLabel{color: white; font-size: 18px;}")

        self.ui.mode_1.setStyleSheet("QPushButton{background-color: rgba(255,255,255,0.12); border:0px;}")
        self.ui.mode_2.setStyleSheet("QPushButton{background-color: rgba(255,255,255,0.12); border:0px;}")
        self.ui.mode_3.setStyleSheet("QPushButton{background-color: rgba(255,255,255,0.12); border:0px;}")
        self.ui.mode_4.setStyleSheet("QPushButton{background-color: rgba(255,255,255,0.12); border:0px;}")

        self.ui.mode_1.setFocusPolicy(Qt.NoFocus)
        self.ui.mode_2.setFocusPolicy(Qt.NoFocus)
        self.ui.mode_3.setFocusPolicy(Qt.NoFocus)
        self.ui.mode_4.setFocusPolicy(Qt.NoFocus)

        self.ui.bmode_1.setStyleSheet("QPushButton{border:0px;} QPushButton:Hover{background-color: rgba(255,255,255,0.08);} QPushButton:Pressed{background-color: rgba(255,255,255,0.04);}")
        self.ui.bmode_2.setStyleSheet("QPushButton{border:0px;} QPushButton:Hover{background-color: rgba(255,255,255,0.12);} QPushButton:Pressed{background-color: rgba(255,255,255,0.08);}")
        self.ui.bmode_3.setStyleSheet("QPushButton{border:0px;} QPushButton:Hover{background-color: rgba(255,255,255,0.08);} QPushButton:Pressed{background-color: rgba(255,255,255,0.04);}")
        self.ui.bmode_4.setStyleSheet("QPushButton{border:0px;} QPushButton:Hover{background-color: rgba(255,255,255,0.12);} QPushButton:Pressed{background-color: rgba(255,255,255,0.08);}")

        self.ui.bmode_1.setFocusPolicy(Qt.NoFocus)
        self.ui.bmode_2.setFocusPolicy(Qt.NoFocus)
        self.ui.bmode_3.setFocusPolicy(Qt.NoFocus)
        self.ui.bmode_4.setFocusPolicy(Qt.NoFocus)

        self.ui.split_1.setStyleSheet("QLabel{background-color: rgba(255,255,255,0.2); border:0px;}")

        self.ui.bg_phone.setStyleSheet("#bg_phone{border-image:url(res/phone.png); border:0px;}")
        self.ui.lb_phone.setStyleSheet("#lb_phone{color: rgba(220,220,220,0.8); font-size: 16px;}")
        self.ui.lb_phone.setAlignment(Qt.AlignLeft | Qt.AlignVCenter)

        self.timer_tip = QTimer()
        self.timer_tip.timeout.connect(self.slot_hide_tip)
        self.ui.tipWidget.hide()

        self.ui.bmode_1.clicked.connect(self.slot_switch_confirm_push_1)
        self.ui.bmode_2.clicked.connect(self.slot_switch_confirm_push_2)
        self.ui.bmode_3.clicked.connect(self.slot_switch_confirm_push_3)
        self.ui.bmode_4.clicked.connect(self.slot_switch_confirm_push_4)

        # self.ui.s_mode_1.hide()
        self.ui.s_mode_2.hide()
        self.ui.s_mode_3.hide()
        self.ui.s_mode_4.hide()

        self.resize(400, 500)
        self.ui.centralWidget.resize(400, 500)

    def start_listen(self):
        self.key_service = KeyServiceXlib(self)
        self.key_service.start()

    def start_listen_dbus(self):
        self.key_dbus_service = KeyServiceDbus(self)
        self.key_dbus_service.start()

    def switch_button(self, direction=3):
        self.ui.mode_1.hide()
        self.ui.mode_2.hide()
        self.ui.mode_3.hide()
        self.ui.mode_4.hide()

        if direction == 1:
            self.current_button += 1
        else:
            self.current_button -= 1

        if self.current_button > 4:
            self.current_button = 1
        if self.current_button < 1:
            self.current_button = 4

        if self.current_button == 1:
            self.ui.mode_1.show()
        if self.current_button == 2:
            self.ui.mode_2.show()
        if self.current_button == 3:
            self.ui.mode_3.show()
        if self.current_button == 4:
            self.ui.mode_4.show()

    def switch_show_selected(self):
        self.ui.s_mode_1.hide()
        self.ui.s_mode_2.hide()
        self.ui.s_mode_3.hide()
        self.ui.s_mode_4.hide()

        print(self.current_button)
        if self.current_button == 0:
            self.ui.s_mode_1.show()
        if self.current_button == 1:
            self.ui.s_mode_2.show()
        if self.current_button == 2:
            self.ui.s_mode_3.show()
        if self.current_button == 3:
            self.ui.s_mode_4.show()

    # switch select mode
    def slot_switch_select(self, direction=1):
        self.timer_tip.stop()
        self.switch_window_type(True)
        self.resize(400, 500)
        self.ui.tipWidget.hide()
        self.ui.centralWidget.resize(400, 500)
        self.ui.centralWidget.show()
        self.show()

        desktop = QApplication.desktop()
        if(desktop.screenCount() > 1):
            desktop = desktop.screenGeometry(0)
        self.move((desktop.width() - self.width()) / 2, (desktop.height() - self.height()) *10 / 21)

        self.switch_button(direction)

    # confirm current selected mode
    def slot_switch_confirm_push_1(self):
        self.hide()
        (current_mode, flag) = self.display_service.switch_display(1)
        # keep select mode when next active
        self.current_button = current_mode - 1
        self.key_service.is_active = False
        self.key_service.is_shown = False
        self.switch_show_selected()

    def slot_switch_confirm_push_2(self):
        self.hide()
        (current_mode, flag) = self.display_service.switch_display(2)
        # keep select mode when next active
        self.current_button = current_mode - 1
        self.key_service.is_active = False
        self.key_service.is_shown = False
        self.switch_show_selected()

    def slot_switch_confirm_push_3(self):
        self.hide()
        (current_mode, flag) = self.display_service.switch_display(3)
        # keep select mode when next active
        self.current_button = current_mode - 1
        self.key_service.is_active = False
        self.key_service.is_shown = False
        self.switch_show_selected()

    def slot_switch_confirm_push_4(self):
        self.hide()
        (current_mode, flag) = self.display_service.switch_display(4)
        # keep select mode when next active
        self.current_button = current_mode - 1
        self.key_service.is_active = False
        self.key_service.is_shown = False
        self.switch_show_selected()

    # confirm current selected mode
    def slot_switch_confirm(self):
        self.hide()

        (current_mode, flag) = self.display_service.switch_display(self.current_button)
        # keep select mode when next active
        self.current_button = current_mode - 1
        self.switch_show_selected()

    # esc by any key pressed
    def slot_switch_esc(self):
        self.hide()

    # esc by any clicked
    def slot_switch_esc2(self, x, y):
        if self.key_service.is_active and self.key_service.is_shown and not self.geometry().contains(x, y):
            self.key_service.is_active = False
            self.key_service.is_shown = False
            self.hide()

    # CapsLock tip
    def slot_tip_capslock(self):
        if not get_lock_tip_show():
            return

        self.switch_window_type(False)
        self.key_service.is_active = False
        self.key_service.is_shown = False

        self.timer_tip.stop()
        self.timer_tip.start(2500)

#        self.resize(190, 190)
#        self.ui.centralWidget.hide()
#        self.ui.caps_on_widget.hide()
#        self.ui.caps_off_widget.hide()
#        self.ui.num_on_widget.hide()
#        self.ui.num_off_widget.hide()
#        self.ui.tp_on_widget.hide()
#        self.ui.tp_off_widget.hide()
#        self.ui.wlan_on_widget.hide()
#        self.ui.wlan_off_widget.hide()
#        self.ui.bluetooth_on_widget.hide()
#        self.ui.bluetooth_off_widget.hide()
        # 隐藏显示切换界面
        self.ui.centralWidget.hide()

        if(self.switchers_service.get_capslock_status() == True):
            self.ui.tipWidget.setCurrentIndex(Tips.CAPS_ON)
        else:
            self.ui.tipWidget.setCurrentIndex(Tips.CAPS_OFF)

        desktop = QApplication.desktop()
        if (desktop.screenCount() > 1):
            desktop = desktop.screenGeometry(0)
        self.move(desktop.width() * 3 / 4, (desktop.height() - posBottom - self.ui.tipWidget.width()))
        self.ui.tipWidget.show()
        self.show()

    # NumLock tip
    def slot_tip_numlock(self):
        if not get_lock_tip_show():
            return

        self.switch_window_type(False)
        self.key_service.is_active = False
        self.key_service.is_shown = False

        self.timer_tip.stop()
        self.timer_tip.start(2500)

        self.ui.centralWidget.hide()

        if(self.switchers_service.get_numlock_status() == True):
            self.ui.tipWidget.setCurrentIndex(Tips.NUM_ON)
        else:
            self.ui.tipWidget.setCurrentIndex(Tips.NUM_OFF)

        desktop = QApplication.desktop()
        if (desktop.screenCount() > 1):
            desktop = desktop.screenGeometry(0)
        self.move(desktop.width() * 3 / 4, (desktop.height() - posBottom - self.ui.tipWidget.width()))
        self.ui.tipWidget.show()
        self.show()


    # MediaKey listen
    def slot_mediakey_trigger(self, keyCode = -1, keyValue = -1):
        """keyCode: Which key is it
           keyValue: The key is pressed or released"""
        print("the", keyCode, "is trigger")

        def showOsdOnDesktop():

            desktop = QApplication.desktop()
            if (desktop.screenCount() > 1):
                desktop = desktop.screenGeometry(0)

            self.move(desktop.width() * 3 / 4, (desktop.height() - posBottom - self.ui.tipWidget.width()))
            self.ui.centralWidget.hide()
#            self.ui.tipWidget.show()
            self.show()

        def touchpadOnMethod():
#            self.mediakey_service.touchpadToggle(True)
            self.ui.tipWidget.setCurrentIndex(Tips.TP_ON)
            showOsdOnDesktop()

        def touchpadOffMethod():
#            self.mediakey_service.touchpadToggle(False)
            self.ui.tipWidget.setCurrentIndex(Tips.TP_OFF)
            showOsdOnDesktop()

        def bluetoothOnOffMethod():
            # switch bluetooth status
            self.mediakey_service.bluetoothToggle()

            # show osd icon
            if self.mediakey_service.bluetoothStatus:
                self.ui.tipWidget.setCurrentIndex(Tips.BLUETOOTH_ON)
            else:
                self.ui.tipWidget.setCurrentIndex(Tips.BLUETOOTH_OFF)
            showOsdOnDesktop()

        def default():
            pass

            mediakeysMethod = { 113 : muteOnOffMethod, \
                            114 : volumeDownMethod, \
                            115 : volumeUpMethod, \
    #                        152 : screenlockMethod, \
                            66 : cameraOnOffMethod, \
                            224 : brightnessDownMethod, \
                            225 : brightnessUpMethod, \
                            227 : videoModeSwitchMethod, \
                            237 : bluetoothOnOffMethod, \
                            238 : flightModeOnOffMethod, \
                            248 : microphoneOnOffMethod, \
                            431 : screenOnOffMethod, \
                            530 : touchpadOnOffMethod, \
                            531 : touchpadOnMethod, \
                            532 : touchpadOffMethod, \
                            }

        self.timer_tip.stop()
        self.timer_tip.start(2500)

        mediakeysMethod.get(keyCode, default)()


    def slot_hide_tip(self):
        self.timer_tip.stop()
        self.ui.tipWidget.hide()
        self.hide()


def callback():
    pass


def main():
    app = QApplication(sys.argv)
    QTextCodec.setCodecForLocale(QTextCodec.codecForName("UTF8"))
    if app.desktop().width() >= 2560:
        QCoreApplication.setAttribute(Qt.AA_EnableHighDpiScaling)
        QCoreApplication.setAttribute(Qt.AA_UseHighDpiPixmaps)

    globalfont = QFont()
    globalfont.setPixelSize(16)
    app.setFont(globalfont)

    KylinDisplaySwitch()

    # disable global output
    # keystr1 = "<Super>P"
    # keybinder.bind(keystr1, callback)
    # keystr2 = "<Super>F3"
    # keybinder.bind(keystr2, callback)
    # keystr3 = "<Super>F7"
    # keybinder.bind(keystr3, callback)

    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
