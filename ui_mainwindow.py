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

#from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5 import QtCore
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *

"""
Tips Enum
"""
class Tips():
    CAPS_ON = 0
    CAPS_OFF = 1
    NUM_ON = 2
    NUM_OFF = 3
    TP_ON = 4
    TP_OFF = 5
    WLAN_ON = 6
    WLAN_OFF = 7
    BLUETOOTH_ON = 8
    BLUETOOTH_OFF = 9
    FLIGHT_ON = 10
    FLIGHT_OFF = 11
    MICROPHONE_ON = 12
    MICROPHONE_OFF = 13
    CAMERA_ON = 14
    CAMERA_OFF = 15

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QApplication.translate(context, text, disambig)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName(_fromUtf8("MainWindow"))
        # MainWindow.resize(744, 126)
        MainWindow.resize(400, 500)

#        self.tipWidget = QWidget(MainWindow)
#        self.tipWidget.setGeometry(0, 0, 190, 190)
        self.tipWidget = QStackedWidget(MainWindow)
#        self.tipWidget.setGeometry(0, 0, 72, 72)
        self.tipWidget.setFixedSize(QSize(72, 72));

        self.caps_on_widget = QWidget()
        self.caps_on_layout = QHBoxLayout()
        self.caps_on_layout.setContentsMargins(0, 0, 0, 0)
        self.caps_on_layout.setSpacing(0)
        self.caps_on_bg = QLabel()
        self.caps_on_bg.setScaledContents(True)
#        capsLogo = QPixmap("res/capslockOn.png")
#        self.caps_on_bg.setPixmap(capsLogo)
        self.caps_on_layout.addWidget(self.caps_on_bg)
        self.caps_on_widget.setLayout(self.caps_on_layout)
        self.tipWidget.addWidget(self.caps_on_widget)


        self.caps_off_widget = QWidget()
        self.caps_off_layout = QHBoxLayout()
        self.caps_off_layout.setContentsMargins(0, 0, 0, 0)
        self.caps_off_layout.setSpacing(0)
        self.caps_off_bg = QLabel()
        self.caps_off_bg.setScaledContents(True)
        self.caps_off_layout.addWidget(self.caps_off_bg)
        self.caps_off_widget.setLayout(self.caps_off_layout)
        self.tipWidget.addWidget(self.caps_off_widget)

        self.num_on_widget = QWidget()
        self.num_on_layout = QHBoxLayout()
        self.num_on_layout.setContentsMargins(0, 0, 0, 0)
        self.num_on_layout.setSpacing(0)
        self.num_on_bg = QLabel()
        self.num_on_bg.setScaledContents(True)
        self.num_on_layout.addWidget(self.num_on_bg)
        self.num_on_widget.setLayout(self.num_on_layout)
        self.tipWidget.addWidget(self.num_on_widget)

        self.num_off_widget = QWidget()
        self.num_off_layout = QHBoxLayout()
        self.num_off_layout.setContentsMargins(0, 0, 0, 0)
        self.num_off_layout.setSpacing(0)
        self.num_off_bg = QLabel()
        self.num_off_bg.setScaledContents(True)
        self.num_off_layout.addWidget(self.num_off_bg)
        self.num_off_widget.setLayout(self.num_off_layout)
        self.tipWidget.addWidget(self.num_off_widget)

        self.tp_on_widget = QWidget()
        self.tp_on_layout = QHBoxLayout()
        self.tp_on_layout.setContentsMargins(0, 0, 0, 0)
        self.tp_on_layout.setSpacing(0)
        self.tp_on_bg = QLabel()
        self.tp_on_bg.setScaledContents(True)
        self.tp_on_layout.addWidget(self.tp_on_bg)
        self.tp_on_widget.setLayout(self.tp_on_layout)
        self.tipWidget.addWidget(self.tp_on_widget)

        self.tp_off_widget = QWidget()
        self.tp_off_layout = QHBoxLayout()
        self.tp_off_layout.setContentsMargins(0, 0, 0, 0)
        self.tp_off_layout.setSpacing(0)
        self.tp_off_bg = QLabel()
        self.tp_off_bg.setScaledContents(True)
        self.tp_off_layout.addWidget(self.tp_off_bg)
        self.tp_off_widget.setLayout(self.tp_off_layout)
        self.tipWidget.addWidget(self.tp_off_widget)

        self.wlan_on_widget = QWidget()
        self.wlan_on_layout = QHBoxLayout()
        self.wlan_on_layout.setContentsMargins(0, 0, 0, 0)
        self.wlan_on_layout.setSpacing(0)
        self.wlan_on_bg = QLabel()
        self.wlan_on_bg.setScaledContents(True)
        self.wlan_on_layout.addWidget(self.wlan_on_bg)
        self.wlan_on_widget.setLayout(self.wlan_on_layout)
        self.tipWidget.addWidget(self.wlan_on_widget)

        self.wlan_off_widget = QWidget()
        self.wlan_off_layout = QHBoxLayout()
        self.wlan_off_layout.setContentsMargins(0, 0, 0, 0)
        self.wlan_off_layout.setSpacing(0)
        self.wlan_off_bg = QLabel()
        self.wlan_off_bg.setScaledContents(True)
        self.wlan_off_layout.addWidget(self.wlan_off_bg)
        self.wlan_off_widget.setLayout(self.wlan_off_layout)
        self.tipWidget.addWidget(self.wlan_off_widget)

        self.bluetooth_on_widget = QWidget()
        self.bluetooth_on_layout = QHBoxLayout()
        self.bluetooth_on_layout.setContentsMargins(0, 0, 0, 0)
        self.bluetooth_on_layout.setSpacing(0)
        self.bluetooth_on_bg = QLabel()
        self.bluetooth_on_bg.setScaledContents(True)
        self.bluetooth_on_layout.addWidget(self.bluetooth_on_bg)
        self.bluetooth_on_widget.setLayout(self.bluetooth_on_layout)
        self.tipWidget.addWidget(self.bluetooth_on_widget)

        self.bluetooth_off_widget = QWidget()
        self.bluetooth_off_layout = QHBoxLayout()
        self.bluetooth_off_layout.setContentsMargins(0, 0, 0, 0)
        self.bluetooth_off_layout.setSpacing(0)
        self.bluetooth_off_bg = QLabel()
        self.bluetooth_off_bg.setScaledContents(True)
        self.bluetooth_off_layout.addWidget(self.bluetooth_off_bg)
        self.bluetooth_off_widget.setLayout(self.bluetooth_off_layout)
        self.tipWidget.addWidget(self.bluetooth_off_widget)


        self.centralWidget = QWidget(MainWindow)
        self.centralWidget.setObjectName(_fromUtf8("centralWidget"))

        # title text
        self.lb_title = QLabel(self.centralWidget)
        self.lb_title.setGeometry(QtCore.QRect(125, 37, 150, 26))
        self.lb_title.setText(_fromUtf8(""))
        self.lb_title.setObjectName(_fromUtf8("lb_title"))

        # hover
        self.mode_1 = QPushButton(self.centralWidget)
        self.mode_1.setGeometry(QtCore.QRect(0, 98, 400, 70))
        self.mode_1.setText(_fromUtf8(""))
        self.mode_1.setObjectName(_fromUtf8("mode_1"))
        self.mode_2 = QPushButton(self.centralWidget)
        self.mode_2.setGeometry(QtCore.QRect(0, 168, 400, 70))
        self.mode_2.setText(_fromUtf8(""))
        self.mode_2.setObjectName(_fromUtf8("mode_2"))
        self.mode_3 = QPushButton(self.centralWidget)
        self.mode_3.setGeometry(QtCore.QRect(0, 238, 400, 70))
        self.mode_3.setText(_fromUtf8(""))
        self.mode_3.setObjectName(_fromUtf8("mode_3"))
        self.mode_4 = QPushButton(self.centralWidget)
        self.mode_4.setGeometry(QtCore.QRect(0, 308, 400, 70))
        self.mode_4.setText(_fromUtf8(""))
        self.mode_4.setObjectName(_fromUtf8("mode_4"))

        # button widget
        self.widget_1 = QWidget(self.centralWidget)
        self.widget_1.setGeometry(QtCore.QRect(0, 98, 400, 70))
        self.widget_1.setObjectName(_fromUtf8("widget_1"))
        self.widget_2 = QWidget(self.centralWidget)
        self.widget_2.setGeometry(QtCore.QRect(0, 168, 400, 70))
        self.widget_2.setObjectName(_fromUtf8("widget_2"))
        self.widget_3 = QWidget(self.centralWidget)
        self.widget_3.setGeometry(QtCore.QRect(0, 238, 400, 70))
        self.widget_3.setObjectName(_fromUtf8("widget_3"))
        self.widget_4 = QWidget(self.centralWidget)
        self.widget_4.setGeometry(QtCore.QRect(0, 308, 400, 70))
        self.widget_4.setObjectName(_fromUtf8("widget_4"))

        # button pic
        self.bg_mode_1 = QLabel(self.widget_1)
        self.bg_mode_1.setGeometry(QtCore.QRect(24, 5, 60, 60))
        self.bg_mode_1.setText(_fromUtf8(""))
        self.bg_mode_1.setObjectName(_fromUtf8("bg_mode_1"))
        self.bg_mode_2 = QLabel(self.widget_2)
        self.bg_mode_2.setGeometry(QtCore.QRect(24, 5, 60, 60))
        self.bg_mode_2.setText(_fromUtf8(""))
        self.bg_mode_2.setObjectName(_fromUtf8("bg_mode_2"))
        self.bg_mode_3 = QLabel(self.widget_3)
        self.bg_mode_3.setGeometry(QtCore.QRect(24, 5, 60, 60))
        self.bg_mode_3.setText(_fromUtf8(""))
        self.bg_mode_3.setObjectName(_fromUtf8("bg_mode_3"))
        self.bg_mode_4 = QLabel(self.widget_4)
        self.bg_mode_4.setGeometry(QtCore.QRect(24, 5, 60, 60))
        self.bg_mode_4.setText(_fromUtf8(""))
        self.bg_mode_4.setObjectName(_fromUtf8("bg_mode_4"))

        # button text
        self.lb_mode_1 = QLabel(self.widget_1)
        self.lb_mode_1.setGeometry(QtCore.QRect(108, 25, 120, 20))
        self.lb_mode_1.setText(_fromUtf8(""))
        self.lb_mode_1.setAlignment(QtCore.Qt.AlignCenter)
        self.lb_mode_1.setObjectName(_fromUtf8("lb_mode_1"))
        self.lb_mode_2 = QLabel(self.widget_2)
        self.lb_mode_2.setGeometry(QtCore.QRect(108, 25, 120, 20))
        self.lb_mode_2.setText(_fromUtf8(""))
        self.lb_mode_2.setAlignment(QtCore.Qt.AlignCenter)
        self.lb_mode_2.setObjectName(_fromUtf8("lb_mode_2"))
        self.lb_mode_3 = QLabel(self.widget_3)
        self.lb_mode_3.setGeometry(QtCore.QRect(108, 25, 120, 20))
        self.lb_mode_3.setText(_fromUtf8(""))
        self.lb_mode_3.setAlignment(QtCore.Qt.AlignCenter)
        self.lb_mode_3.setObjectName(_fromUtf8("lb_mode_3"))
        self.lb_mode_4 = QLabel(self.widget_4)
        self.lb_mode_4.setGeometry(QtCore.QRect(108, 25, 120, 20))
        self.lb_mode_4.setText(_fromUtf8(""))
        self.lb_mode_4.setAlignment(QtCore.Qt.AlignCenter)
        self.lb_mode_4.setObjectName(_fromUtf8("lb_mode_4"))

        # button selected pic
        self.s_mode_1 = QLabel(self.widget_1)
        self.s_mode_1.setGeometry(QtCore.QRect(354, 26, 27, 18))
        self.s_mode_1.setText(_fromUtf8(""))
        self.s_mode_1.setObjectName(_fromUtf8("s_mode_1"))
        self.s_mode_2 = QLabel(self.widget_2)
        self.s_mode_2.setGeometry(QtCore.QRect(354, 26, 27, 18))
        self.s_mode_2.setText(_fromUtf8(""))
        self.s_mode_2.setObjectName(_fromUtf8("s_mode_2"))
        self.s_mode_3 = QLabel(self.widget_3)
        self.s_mode_3.setGeometry(QtCore.QRect(354, 26, 27, 18))
        self.s_mode_3.setText(_fromUtf8(""))
        self.s_mode_3.setObjectName(_fromUtf8("s_mode_3"))
        self.s_mode_4 = QLabel(self.widget_4)
        self.s_mode_4.setGeometry(QtCore.QRect(354, 26, 27, 18))
        self.s_mode_4.setText(_fromUtf8(""))
        self.s_mode_4.setObjectName(_fromUtf8("s_mode_4"))

        # button action
        self.bmode_1 = QPushButton(self.widget_1)
        self.bmode_1.setGeometry(QtCore.QRect(0, 0, 400, 70))
        self.bmode_1.setText(_fromUtf8(""))
        self.bmode_1.setObjectName(_fromUtf8("bmode_1"))
        self.bmode_2 = QPushButton(self.widget_2)
        self.bmode_2.setGeometry(QtCore.QRect(0, 0, 400, 70))
        self.bmode_2.setText(_fromUtf8(""))
        self.bmode_2.setObjectName(_fromUtf8("bmode_2"))
        self.bmode_3 = QPushButton(self.widget_3)
        self.bmode_3.setGeometry(QtCore.QRect(0, 0, 400, 70))
        self.bmode_3.setText(_fromUtf8(""))
        self.bmode_3.setObjectName(_fromUtf8("bmode_3"))
        self.bmode_4 = QPushButton(self.widget_4)
        self.bmode_4.setGeometry(QtCore.QRect(0, 0, 400, 70))
        self.bmode_4.setText(_fromUtf8(""))
        self.bmode_4.setObjectName(_fromUtf8("bmode_4"))

        # split bar 1px
        self.split_1 = QLabel(self.centralWidget)
        self.split_1.setGeometry(QtCore.QRect(0, 407, 400, 1))
        self.split_1.setText(_fromUtf8(""))
        self.split_1.setObjectName(_fromUtf8("split_1"))

        # conn to phone
        self.bg_phone = QLabel(self.centralWidget)
        self.bg_phone.setGeometry(QtCore.QRect(123, 438, 30, 30))
        self.bg_phone.setText(_fromUtf8(""))
        self.bg_phone.setObjectName(_fromUtf8("bg_phone"))

        self.lb_phone = QLabel(self.centralWidget)
        self.lb_phone.setGeometry(QtCore.QRect(165, 442, 130, 20))
        self.lb_phone.setText(_fromUtf8(""))
        self.lb_phone.setObjectName(_fromUtf8("lb_phone"))

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow", None))

