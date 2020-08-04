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

from PyQt5 import QtCore, QtGui, QtWidgets

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtWidgets.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtWidgets.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtWidgets.QApplication.translate(context, text, disambig)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName(_fromUtf8("MainWindow"))
        # MainWindow.resize(744, 126)
        MainWindow.resize(400, 500)

        self.tipWidget = QtWidgets.QWidget(MainWindow)
        self.tipWidget.setGeometry(0, 0, 190, 190)

        self.caps_on_widget = QtWidgets.QWidget(self.tipWidget)
        self.caps_on_widget.setGeometry(0, 0, 190, 190)
        self.caps_on_bg = QtWidgets.QLabel(self.caps_on_widget)
        self.caps_on_bg.setGeometry(0, 0, 190, 190)
        self.lb_caps_on = QtWidgets.QLabel(self.caps_on_widget)
        self.lb_caps_on.setGeometry(QtCore.QRect(45, 20, 100, 20))
        self.lb_caps_on.setAlignment(QtCore.Qt.AlignCenter)

        self.caps_off_widget = QtWidgets.QWidget(self.tipWidget)
        self.caps_off_widget.setGeometry(0, 0, 190, 190)
        self.caps_off_bg = QtWidgets.QLabel(self.caps_off_widget)
        self.caps_off_bg.setGeometry(0, 0, 190, 190)
        self.lb_caps_off = QtWidgets.QLabel(self.caps_off_widget)
        self.lb_caps_off.setGeometry(QtCore.QRect(45, 20, 100, 20))
        self.lb_caps_off.setAlignment(QtCore.Qt.AlignCenter)

        self.num_on_widget = QtWidgets.QWidget(self.tipWidget)
        self.num_on_widget.setGeometry(0, 0, 190, 190)
        self.num_on_bg = QtWidgets.QLabel(self.num_on_widget)
        self.num_on_bg.setGeometry(0, 0, 190, 190)
        self.lb_num_on = QtWidgets.QLabel(self.num_on_widget)
        self.lb_num_on.setGeometry(QtCore.QRect(40, 20, 110, 20))
        self.lb_num_on.setAlignment(QtCore.Qt.AlignCenter)

        self.num_off_widget = QtWidgets.QWidget(self.tipWidget)
        self.num_off_widget.setGeometry(0, 0, 190, 190)
        self.num_off_bg = QtWidgets.QLabel(self.num_off_widget)
        self.num_off_bg.setGeometry(0, 0, 190, 190)
        self.lb_num_off = QtWidgets.QLabel(self.num_off_widget)
        self.lb_num_off.setGeometry(QtCore.QRect(40, 20, 110, 20))
        self.lb_num_off.setAlignment(QtCore.Qt.AlignCenter)

        self.tp_on_widget = QtWidgets.QWidget(self.tipWidget)
        self.tp_on_widget.setGeometry(0, 0, 190, 190)
        self.tp_on_bg = QtWidgets.QLabel(self.tp_on_widget)
        self.tp_on_bg.setGeometry(0, 0, 190, 190)
        self.lb_tp_on = QtWidgets.QLabel(self.tp_on_widget)
        self.lb_tp_on.setGeometry(QtCore.QRect(40, 20, 110, 20))
        self.lb_tp_on.setAlignment(QtCore.Qt.AlignCenter)

        self.tp_off_widget = QtWidgets.QWidget(self.tipWidget)
        self.tp_off_widget.setGeometry(0, 0, 190, 190)
        self.tp_off_bg = QtWidgets.QLabel(self.tp_off_widget)
        self.tp_off_bg.setGeometry(0, 0, 190, 190)
        self.lb_tp_off = QtWidgets.QLabel(self.tp_off_widget)
        self.lb_tp_off.setGeometry(QtCore.QRect(40, 20, 110, 20))
        self.lb_tp_off.setAlignment(QtCore.Qt.AlignCenter)

        self.wlan_on_widget = QtWidgets.QWidget(self.tipWidget)
        self.wlan_on_widget.setGeometry(0, 0, 190, 190)
        self.wlan_on_bg = QtWidgets.QLabel(self.wlan_on_widget)
        self.wlan_on_bg.setGeometry(0, 0, 190, 190)
        self.lb_wlan_on = QtWidgets.QLabel(self.wlan_on_widget)
        self.lb_wlan_on.setGeometry(QtCore.QRect(40, 20, 110, 20))
        self.lb_wlan_on.setAlignment(QtCore.Qt.AlignCenter)

        self.wlan_off_widget = QtWidgets.QWidget(self.tipWidget)
        self.wlan_off_widget.setGeometry(0, 0, 190, 190)
        self.wlan_off_bg = QtWidgets.QLabel(self.wlan_off_widget)
        self.wlan_off_bg.setGeometry(0, 0, 190, 190)
        self.lb_wlan_off = QtWidgets.QLabel(self.wlan_off_widget)
        self.lb_wlan_off.setGeometry(QtCore.QRect(40, 20, 110, 20))
        self.lb_wlan_off.setAlignment(QtCore.Qt.AlignCenter)

        self.bluetooth_on_widget = QtWidgets.QWidget(self.tipWidget)
        self.bluetooth_on_widget.setGeometry(0, 0, 190, 190)
        self.bluetooth_on_bg = QtWidgets.QLabel(self.bluetooth_on_widget)
        self.bluetooth_on_bg.setGeometry(0, 0, 190, 190)
        self.lb_bluetooth_on = QtWidgets.QLabel(self.bluetooth_on_widget)
        self.lb_bluetooth_on.setGeometry(QtCore.QRect(40, 20, 110, 20))
        self.lb_bluetooth_on.setAlignment(QtCore.Qt.AlignCenter)

        self.bluetooth_off_widget = QtWidgets.QWidget(self.tipWidget)
        self.bluetooth_off_widget.setGeometry(0, 0, 190, 190)
        self.bluetooth_off_bg = QtWidgets.QLabel(self.bluetooth_off_widget)
        self.bluetooth_off_bg.setGeometry(0, 0, 190, 190)
        self.lb_bluetooth_off = QtWidgets.QLabel(self.bluetooth_off_widget)
        self.lb_bluetooth_off.setGeometry(QtCore.QRect(40, 20, 110, 20))
        self.lb_bluetooth_off.setAlignment(QtCore.Qt.AlignCenter)


        self.centralWidget = QtWidgets.QWidget(MainWindow)
        self.centralWidget.setObjectName(_fromUtf8("centralWidget"))

        # title text
        self.lb_title = QtWidgets.QLabel(self.centralWidget)
        self.lb_title.setGeometry(QtCore.QRect(125, 37, 150, 26))
        self.lb_title.setText(_fromUtf8(""))
        self.lb_title.setObjectName(_fromUtf8("lb_title"))

        # hover
        self.mode_1 = QtWidgets.QPushButton(self.centralWidget)
        self.mode_1.setGeometry(QtCore.QRect(0, 98, 400, 70))
        self.mode_1.setText(_fromUtf8(""))
        self.mode_1.setObjectName(_fromUtf8("mode_1"))
        self.mode_2 = QtWidgets.QPushButton(self.centralWidget)
        self.mode_2.setGeometry(QtCore.QRect(0, 168, 400, 70))
        self.mode_2.setText(_fromUtf8(""))
        self.mode_2.setObjectName(_fromUtf8("mode_2"))
        self.mode_3 = QtWidgets.QPushButton(self.centralWidget)
        self.mode_3.setGeometry(QtCore.QRect(0, 238, 400, 70))
        self.mode_3.setText(_fromUtf8(""))
        self.mode_3.setObjectName(_fromUtf8("mode_3"))
        self.mode_4 = QtWidgets.QPushButton(self.centralWidget)
        self.mode_4.setGeometry(QtCore.QRect(0, 308, 400, 70))
        self.mode_4.setText(_fromUtf8(""))
        self.mode_4.setObjectName(_fromUtf8("mode_4"))

        # button widget
        self.widget_1 = QtWidgets.QWidget(self.centralWidget)
        self.widget_1.setGeometry(QtCore.QRect(0, 98, 400, 70))
        self.widget_1.setObjectName(_fromUtf8("widget_1"))
        self.widget_2 = QtWidgets.QWidget(self.centralWidget)
        self.widget_2.setGeometry(QtCore.QRect(0, 168, 400, 70))
        self.widget_2.setObjectName(_fromUtf8("widget_2"))
        self.widget_3 = QtWidgets.QWidget(self.centralWidget)
        self.widget_3.setGeometry(QtCore.QRect(0, 238, 400, 70))
        self.widget_3.setObjectName(_fromUtf8("widget_3"))
        self.widget_4 = QtWidgets.QWidget(self.centralWidget)
        self.widget_4.setGeometry(QtCore.QRect(0, 308, 400, 70))
        self.widget_4.setObjectName(_fromUtf8("widget_4"))

        # button pic
        self.bg_mode_1 = QtWidgets.QLabel(self.widget_1)
        self.bg_mode_1.setGeometry(QtCore.QRect(24, 5, 60, 60))
        self.bg_mode_1.setText(_fromUtf8(""))
        self.bg_mode_1.setObjectName(_fromUtf8("bg_mode_1"))
        self.bg_mode_2 = QtWidgets.QLabel(self.widget_2)
        self.bg_mode_2.setGeometry(QtCore.QRect(24, 5, 60, 60))
        self.bg_mode_2.setText(_fromUtf8(""))
        self.bg_mode_2.setObjectName(_fromUtf8("bg_mode_2"))
        self.bg_mode_3 = QtWidgets.QLabel(self.widget_3)
        self.bg_mode_3.setGeometry(QtCore.QRect(24, 5, 60, 60))
        self.bg_mode_3.setText(_fromUtf8(""))
        self.bg_mode_3.setObjectName(_fromUtf8("bg_mode_3"))
        self.bg_mode_4 = QtWidgets.QLabel(self.widget_4)
        self.bg_mode_4.setGeometry(QtCore.QRect(24, 5, 60, 60))
        self.bg_mode_4.setText(_fromUtf8(""))
        self.bg_mode_4.setObjectName(_fromUtf8("bg_mode_4"))

        # button text
        self.lb_mode_1 = QtWidgets.QLabel(self.widget_1)
        self.lb_mode_1.setGeometry(QtCore.QRect(108, 25, 120, 20))
        self.lb_mode_1.setText(_fromUtf8(""))
        self.lb_mode_1.setAlignment(QtCore.Qt.AlignCenter)
        self.lb_mode_1.setObjectName(_fromUtf8("lb_mode_1"))
        self.lb_mode_2 = QtWidgets.QLabel(self.widget_2)
        self.lb_mode_2.setGeometry(QtCore.QRect(108, 25, 120, 20))
        self.lb_mode_2.setText(_fromUtf8(""))
        self.lb_mode_2.setAlignment(QtCore.Qt.AlignCenter)
        self.lb_mode_2.setObjectName(_fromUtf8("lb_mode_2"))
        self.lb_mode_3 = QtWidgets.QLabel(self.widget_3)
        self.lb_mode_3.setGeometry(QtCore.QRect(108, 25, 120, 20))
        self.lb_mode_3.setText(_fromUtf8(""))
        self.lb_mode_3.setAlignment(QtCore.Qt.AlignCenter)
        self.lb_mode_3.setObjectName(_fromUtf8("lb_mode_3"))
        self.lb_mode_4 = QtWidgets.QLabel(self.widget_4)
        self.lb_mode_4.setGeometry(QtCore.QRect(108, 25, 120, 20))
        self.lb_mode_4.setText(_fromUtf8(""))
        self.lb_mode_4.setAlignment(QtCore.Qt.AlignCenter)
        self.lb_mode_4.setObjectName(_fromUtf8("lb_mode_4"))

        # button selected pic
        self.s_mode_1 = QtWidgets.QLabel(self.widget_1)
        self.s_mode_1.setGeometry(QtCore.QRect(354, 26, 27, 18))
        self.s_mode_1.setText(_fromUtf8(""))
        self.s_mode_1.setObjectName(_fromUtf8("s_mode_1"))
        self.s_mode_2 = QtWidgets.QLabel(self.widget_2)
        self.s_mode_2.setGeometry(QtCore.QRect(354, 26, 27, 18))
        self.s_mode_2.setText(_fromUtf8(""))
        self.s_mode_2.setObjectName(_fromUtf8("s_mode_2"))
        self.s_mode_3 = QtWidgets.QLabel(self.widget_3)
        self.s_mode_3.setGeometry(QtCore.QRect(354, 26, 27, 18))
        self.s_mode_3.setText(_fromUtf8(""))
        self.s_mode_3.setObjectName(_fromUtf8("s_mode_3"))
        self.s_mode_4 = QtWidgets.QLabel(self.widget_4)
        self.s_mode_4.setGeometry(QtCore.QRect(354, 26, 27, 18))
        self.s_mode_4.setText(_fromUtf8(""))
        self.s_mode_4.setObjectName(_fromUtf8("s_mode_4"))

        # button action
        self.bmode_1 = QtWidgets.QPushButton(self.widget_1)
        self.bmode_1.setGeometry(QtCore.QRect(0, 0, 400, 70))
        self.bmode_1.setText(_fromUtf8(""))
        self.bmode_1.setObjectName(_fromUtf8("bmode_1"))
        self.bmode_2 = QtWidgets.QPushButton(self.widget_2)
        self.bmode_2.setGeometry(QtCore.QRect(0, 0, 400, 70))
        self.bmode_2.setText(_fromUtf8(""))
        self.bmode_2.setObjectName(_fromUtf8("bmode_2"))
        self.bmode_3 = QtWidgets.QPushButton(self.widget_3)
        self.bmode_3.setGeometry(QtCore.QRect(0, 0, 400, 70))
        self.bmode_3.setText(_fromUtf8(""))
        self.bmode_3.setObjectName(_fromUtf8("bmode_3"))
        self.bmode_4 = QtWidgets.QPushButton(self.widget_4)
        self.bmode_4.setGeometry(QtCore.QRect(0, 0, 400, 70))
        self.bmode_4.setText(_fromUtf8(""))
        self.bmode_4.setObjectName(_fromUtf8("bmode_4"))

        # split bar 1px
        self.split_1 = QtWidgets.QLabel(self.centralWidget)
        self.split_1.setGeometry(QtCore.QRect(0, 407, 400, 1))
        self.split_1.setText(_fromUtf8(""))
        self.split_1.setObjectName(_fromUtf8("split_1"))

        # conn to phone
        self.bg_phone = QtWidgets.QLabel(self.centralWidget)
        self.bg_phone.setGeometry(QtCore.QRect(123, 438, 30, 30))
        self.bg_phone.setText(_fromUtf8(""))
        self.bg_phone.setObjectName(_fromUtf8("bg_phone"))

        self.lb_phone = QtWidgets.QLabel(self.centralWidget)
        self.lb_phone.setGeometry(QtCore.QRect(165, 442, 130, 20))
        self.lb_phone.setText(_fromUtf8(""))
        self.lb_phone.setObjectName(_fromUtf8("lb_phone"))

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow", None))

