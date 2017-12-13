# -*- coding: utf-8 -*-

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName(_fromUtf8("MainWindow"))
        MainWindow.resize(744, 126)

        self.tipWidget = QtGui.QWidget(MainWindow)
        self.tipWidget.setGeometry(0, 0, 190, 190)

        self.caps_on_widget = QtGui.QWidget(self.tipWidget)
        self.caps_on_widget.setGeometry(0, 0, 190, 190)
        self.caps_on_bg = QtGui.QLabel(self.caps_on_widget)
        self.caps_on_bg.setGeometry(0, 0, 190, 190)
        self.lb_caps_on = QtGui.QLabel(self.caps_on_widget)
        self.lb_caps_on.setGeometry(QtCore.QRect(45, 20, 100, 20))
        self.lb_caps_on.setAlignment(QtCore.Qt.AlignCenter)

        self.caps_off_widget = QtGui.QWidget(self.tipWidget)
        self.caps_off_widget.setGeometry(0, 0, 190, 190)
        self.caps_off_bg = QtGui.QLabel(self.caps_off_widget)
        self.caps_off_bg.setGeometry(0, 0, 190, 190)
        self.lb_caps_off = QtGui.QLabel(self.caps_off_widget)
        self.lb_caps_off.setGeometry(QtCore.QRect(45, 20, 100, 20))
        self.lb_caps_off.setAlignment(QtCore.Qt.AlignCenter)

        self.num_on_widget = QtGui.QWidget(self.tipWidget)
        self.num_on_widget.setGeometry(0, 0, 190, 190)
        self.num_on_bg = QtGui.QLabel(self.num_on_widget)
        self.num_on_bg.setGeometry(0, 0, 190, 190)
        self.lb_num_on = QtGui.QLabel(self.num_on_widget)
        self.lb_num_on.setGeometry(QtCore.QRect(40, 20, 110, 20))
        self.lb_num_on.setAlignment(QtCore.Qt.AlignCenter)

        self.num_off_widget = QtGui.QWidget(self.tipWidget)
        self.num_off_widget.setGeometry(0, 0, 190, 190)
        self.num_off_bg = QtGui.QLabel(self.num_off_widget)
        self.num_off_bg.setGeometry(0, 0, 190, 190)
        self.lb_num_off = QtGui.QLabel(self.num_off_widget)
        self.lb_num_off.setGeometry(QtCore.QRect(40, 20, 110, 20))
        self.lb_num_off.setAlignment(QtCore.Qt.AlignCenter)

        self.centralWidget = QtGui.QWidget(MainWindow)
        self.centralWidget.setObjectName(_fromUtf8("centralWidget"))
        self.mode_1 = QtGui.QPushButton(self.centralWidget)
        self.mode_1.setGeometry(QtCore.QRect(5, 5, 180, 116))
        self.mode_1.setText(_fromUtf8(""))
        self.mode_1.setObjectName(_fromUtf8("mode_1"))
        self.mode_2 = QtGui.QPushButton(self.centralWidget)
        self.mode_2.setGeometry(QtCore.QRect(190, 5, 180, 116))
        self.mode_2.setText(_fromUtf8(""))
        self.mode_2.setObjectName(_fromUtf8("mode_2"))
        self.mode_3 = QtGui.QPushButton(self.centralWidget)
        self.mode_3.setGeometry(QtCore.QRect(375, 5, 180, 116))
        self.mode_3.setText(_fromUtf8(""))
        self.mode_3.setObjectName(_fromUtf8("mode_3"))
        self.mode_4 = QtGui.QPushButton(self.centralWidget)
        self.mode_4.setGeometry(QtCore.QRect(560, 5, 180, 116))
        self.mode_4.setText(_fromUtf8(""))
        self.mode_4.setObjectName(_fromUtf8("mode_4"))
        self.bg_mode_1 = QtGui.QLabel(self.centralWidget)
        self.bg_mode_1.setGeometry(QtCore.QRect(63, 43, 64, 64))
        self.bg_mode_1.setText(_fromUtf8(""))
        self.bg_mode_1.setObjectName(_fromUtf8("bg_mode_1"))
        self.lb_mode_1 = QtGui.QLabel(self.centralWidget)
        self.lb_mode_1.setGeometry(QtCore.QRect(58, 18, 74, 20))
        self.lb_mode_1.setText(_fromUtf8(""))
        self.lb_mode_1.setAlignment(QtCore.Qt.AlignCenter)
        self.lb_mode_1.setObjectName(_fromUtf8("lb_mode_1"))
        self.split_1 = QtGui.QLabel(self.centralWidget)
        self.split_1.setGeometry(QtCore.QRect(187, 18, 1, 90))
        self.split_1.setText(_fromUtf8(""))
        self.split_1.setObjectName(_fromUtf8("split_1"))
        self.split_2 = QtGui.QLabel(self.centralWidget)
        self.split_2.setGeometry(QtCore.QRect(372, 18, 1, 90))
        self.split_2.setText(_fromUtf8(""))
        self.split_2.setObjectName(_fromUtf8("split_2"))
        self.split_3 = QtGui.QLabel(self.centralWidget)
        self.split_3.setGeometry(QtCore.QRect(557, 18, 1, 90))
        self.split_3.setText(_fromUtf8(""))
        self.split_3.setObjectName(_fromUtf8("split_3"))
        self.lb_mode_2 = QtGui.QLabel(self.centralWidget)
        self.lb_mode_2.setGeometry(QtCore.QRect(248, 18, 64, 20))
        self.lb_mode_2.setText(_fromUtf8(""))
        self.lb_mode_2.setAlignment(QtCore.Qt.AlignCenter)
        self.lb_mode_2.setObjectName(_fromUtf8("lb_mode_2"))
        self.lb_mode_3 = QtGui.QLabel(self.centralWidget)
        self.lb_mode_3.setGeometry(QtCore.QRect(433, 18, 64, 20))
        self.lb_mode_3.setText(_fromUtf8(""))
        self.lb_mode_3.setAlignment(QtCore.Qt.AlignCenter)
        self.lb_mode_3.setObjectName(_fromUtf8("lb_mode_3"))
        self.lb_mode_4 = QtGui.QLabel(self.centralWidget)
        self.lb_mode_4.setGeometry(QtCore.QRect(618, 18, 64, 20))
        self.lb_mode_4.setText(_fromUtf8(""))
        self.lb_mode_4.setAlignment(QtCore.Qt.AlignCenter)
        self.lb_mode_4.setObjectName(_fromUtf8("lb_mode_4"))
        self.bg_mode_2 = QtGui.QLabel(self.centralWidget)
        self.bg_mode_2.setGeometry(QtCore.QRect(248, 43, 64, 64))
        self.bg_mode_2.setText(_fromUtf8(""))
        self.bg_mode_2.setObjectName(_fromUtf8("bg_mode_2"))
        self.bg_mode_3 = QtGui.QLabel(self.centralWidget)
        self.bg_mode_3.setGeometry(QtCore.QRect(433, 43, 64, 64))
        self.bg_mode_3.setText(_fromUtf8(""))
        self.bg_mode_3.setObjectName(_fromUtf8("bg_mode_3"))
        self.bg_mode_4 = QtGui.QLabel(self.centralWidget)
        self.bg_mode_4.setGeometry(QtCore.QRect(618, 43, 64, 64))
        self.bg_mode_4.setText(_fromUtf8(""))
        self.bg_mode_4.setObjectName(_fromUtf8("bg_mode_4"))

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow", None))

