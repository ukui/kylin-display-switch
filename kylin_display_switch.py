#!/usr/bin/python3
# -*- coding: utf-8 -*-

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
from key_service_xlib import KeyServiceXlib
from display_service import DisplayService
from switchers_service import SwitchersService
import fcntl


class KylinDisplaySwitch(QWidget):

    key_service = None
    display_service = None
    switchers_service = None
    settings = None
    current_button = 0

    def __init__(self, parent = None):
        super(KylinDisplaySwitch, self).__init__(parent)

        self.check_singleton()

        self.init_ui()

        self.display_service = DisplayService()
        self.switchers_service = SwitchersService()
        self.settings = QSettings("kylinos.cn", "KylinDisplaySwitch")

        self.start_listen()

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

    def init_ui(self):
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        self.setAttribute(Qt.WA_TranslucentBackground)
        self.setWindowFlags(Qt.FramelessWindowHint | Qt.ToolTip)

        self.ui.centralWidget.setAutoFillBackground(True)
        pc = QPalette()
        img_bg = QPixmap("res/bg.png")
        pc.setBrush(QPalette.Window, QBrush(img_bg))
        self.ui.centralWidget.setPalette(pc)

        self.ui.lb_mode_1.setText(_("Computer"))
        self.ui.lb_mode_2.setText(_("Clone"))
        self.ui.lb_mode_3.setText(_("Extend"))
        self.ui.lb_mode_4.setText(_("Output"))

        self.ui.lb_caps_on.setText(_("Caps on"))
        self.ui.lb_caps_off.setText(_("Caps off"))
        self.ui.lb_num_on.setText(_("Num on"))
        self.ui.lb_num_off.setText(_("Num off"))

        self.ui.caps_on_bg.setStyleSheet("QWidget{background-image:url(res/capslock-on.png); border:0px;}")
        self.ui.caps_off_bg.setStyleSheet("QWidget{background-image:url(res/capslock-off.png); border:0px;}")
        self.ui.num_on_bg.setStyleSheet("QWidget{background-image:url(res/numlock-on.png); border:0px;}")
        self.ui.num_off_bg.setStyleSheet("QWidget{background-image:url(res/numlock-off.png); border:0px;}")

        self.ui.lb_caps_on.setStyleSheet("QLabel{color:white;font-size:19px;}")
        self.ui.lb_caps_off.setStyleSheet("QLabel{color:white;font-size:19px;}")
        self.ui.lb_num_on.setStyleSheet("QLabel{color:white;font-size:19px;}")
        self.ui.lb_num_off.setStyleSheet("QLabel{color:white;font-size:19px;}")

        self.ui.bg_mode_1.setStyleSheet("QWidget{background-image:url(res/btn1.png); border:0px;}")
        self.ui.bg_mode_2.setStyleSheet("QWidget{background-image:url(res/btn2.png); border:0px;}")
        self.ui.bg_mode_3.setStyleSheet("QWidget{background-image:url(res/btn3.png); border:0px;}")
        self.ui.bg_mode_4.setStyleSheet("QWidget{background-image:url(res/btn4.png); border:0px;}")

        self.ui.split_1.setStyleSheet("QLabel{background-image:url(res/split.png); border:0px;}")
        self.ui.split_2.setStyleSheet("QLabel{background-image:url(res/split.png); border:0px;}")
        self.ui.split_3.setStyleSheet("QLabel{background-image:url(res/split.png); border:0px;}")

        self.ui.mode_1.setStyleSheet("QPushButton{background-image:url(res/hover.png); border:0px;}")
        self.ui.mode_2.setStyleSheet("QPushButton{background-image:url(res/hover.png); border:0px;}")
        self.ui.mode_3.setStyleSheet("QPushButton{background-image:url(res/hover.png); border:0px;}")
        self.ui.mode_4.setStyleSheet("QPushButton{background-image:url(res/hover.png); border:0px;}")

        self.ui.mode_1.setFocusPolicy(Qt.NoFocus)
        self.ui.mode_2.setFocusPolicy(Qt.NoFocus)
        self.ui.mode_3.setFocusPolicy(Qt.NoFocus)
        self.ui.mode_4.setFocusPolicy(Qt.NoFocus)

        self.timer_tip = QTimer()
        self.timer_tip.timeout.connect(self.slot_hide_tip)
        self.ui.tipWidget.hide()

    def start_listen(self):
        self.key_service = KeyServiceXlib(self)
        self.key_service.start()

    def switch_button(self):
        self.ui.mode_1.hide()
        self.ui.mode_2.hide()
        self.ui.mode_3.hide()
        self.ui.mode_4.hide()

        self.current_button += 1

        if self.current_button > 4:
            self.current_button = 1

        if self.current_button == 1:
            self.ui.mode_1.show()
        if self.current_button == 2:
            self.ui.mode_2.show()
        if self.current_button == 3:
            self.ui.mode_3.show()
        if self.current_button == 4:
            self.ui.mode_4.show()

    # swich select mode
    def slot_switch_select(self):
        self.timer_tip.stop()
        self.resize(744, 126)
        self.ui.tipWidget.hide()
        self.ui.centralWidget.resize(744, 126)
        self.ui.centralWidget.show()
        self.show()

        desktop = QApplication.desktop()
        if(desktop.screenCount() > 1):
            desktop = desktop.screenGeometry(0)
        self.move((desktop.width() - self.width()) / 2, (desktop.height() - self.height()) *10 / 21)

        self.switch_button()

    # confirm current selected mode
    def slot_switch_confirm(self):
        self.hide()

        (current_mode, flag) = self.display_service.switch_display(self.current_button)
        # keep select mode when next active
        self.current_button = current_mode - 1

    # CapsLock tip
    def slot_tip_capslock(self):
        self.timer_tip.stop()
        self.timer_tip.start(2500)

        self.resize(190, 190)
        self.ui.centralWidget.hide()
        self.ui.caps_on_widget.hide()
        self.ui.caps_off_widget.hide()
        self.ui.num_on_widget.hide()
        self.ui.num_off_widget.hide()

        if(self.switchers_service.get_capslock_status() == True):
            self.ui.caps_on_widget.show()
        else:
            self.ui.caps_off_widget.show()

        desktop = QApplication.desktop()
        if (desktop.screenCount() > 1):
            desktop = desktop.screenGeometry(0)
        self.move((desktop.width() - self.width()) / 2, (desktop.height() - self.height()) / 2)
        self.ui.tipWidget.show()
        self.show()

    # NumLock tip
    def slot_tip_numlock(self):
        self.timer_tip.stop()
        self.timer_tip.start(2500)

        self.resize(190, 190)
        self.ui.centralWidget.hide()
        self.ui.caps_on_widget.hide()
        self.ui.caps_off_widget.hide()
        self.ui.num_on_widget.hide()
        self.ui.num_off_widget.hide()

        if(self.switchers_service.get_numlock_status() == True):
            self.ui.num_on_widget.show()
        else:
            self.ui.num_off_widget.show()

        desktop = QApplication.desktop()
        if (desktop.screenCount() > 1):
            desktop = desktop.screenGeometry(0)
        self.move((desktop.width() - self.width()) / 2, (desktop.height() - self.height()) / 2)
        self.ui.tipWidget.show()
        self.show()

    def slot_hide_tip(self):
        self.timer_tip.stop()
        self.ui.tipWidget.hide()
        self.hide()


def callback():
    pass


def main():
    app = QApplication(sys.argv)
    QTextCodec.setCodecForLocale(QTextCodec.codecForName("UTF8"))

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
