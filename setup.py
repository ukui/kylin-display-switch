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
    ('bin/', ['kds']),
    ('../etc/xdg/autostart/', ['kylin-display-switch.desktop']),
    ('share/kylin-display-switch/', ['display_service.py', 'switchers_service.py', 'enums.py', 'key_service_xlib.py', 'kylin_display_switch.py', 'ui_mainwindow.py']),
    ('share/kylin-display-switch/res/', glob.glob('res/*.png')),
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
