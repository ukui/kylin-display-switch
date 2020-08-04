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

import os
import sys
import glob
from distutils.core import setup
from subprocess import call

import DistUtilsExtra.command.build_extra
import DistUtilsExtra.command.build_i18n
import DistUtilsExtra.command.clean_i18n

def datafilelist(installbase, sourcebase):
    datafileList = []
    for root, subFolders, files in os.walk(sourcebase):
        fileList = []
        for f in files:
            fileList.append(os.path.join(root, f))
        datafileList.append((root.replace(sourcebase, installbase), fileList))
    return datafileList

data_files=[
    ("share/man/man1",
     glob.glob("man/*.1")
    ),
    ('bin/', ['kds']),
    ('../etc/xdg/autostart/', ['kylin-display-switch.desktop']),
    ('share/kylin-display-switch/', ['display_service.py', 'switchers_service.py', 'mediakey_service.py','enums.py', 'key_service_xlib.py', 'key_service_dbus.py', 'super_kylin_display_switch.py', 'kylin_display_switch.py', 'ui_mainwindow.py']),
    ('share/kylin-display-switch/res/', glob.glob('res/*.png')),
    ('../etc/dbus-1/system.d/', ['dbus/conf/com.kylin.display.switch.conf']),
    ('share/dbus-1/system-services/', ['dbus/conf/com.kylin.display.switch.service']),
    ('../lib/systemd/system/', ['conf/kylin-display-switch.service']),
    ('share/glib-2.0/schemas/', ['dbus/conf/org.kylin.display.switch.gschema.xml']),
    ('bin/', ['dbus/kdsSysDbusLauncher']),
]


data_files.extend(datafilelist('share/locale', 'build/mo'))

cmdclass = {
             "build" : DistUtilsExtra.command.build_extra.build_extra,
             "build_i18n" :  DistUtilsExtra.command.build_i18n.build_i18n,
             "clean": DistUtilsExtra.command.clean_i18n.clean_i18n,
}

setup(name="kylin-display-switch",
      version="1.0.1",
      author="shine",
      author_email="huangsheng@kylinos.cn",
      url="https://github.com/ukui/kylin-display-switch",
      license="GPLv3",
      data_files=data_files,
      cmdclass = cmdclass,
      install_requires=[
          'python3-pyqt5',
          'python3-xlib'
      ],
      )
