import os
import sys
import glob
from distutils.core import setup
from subprocess import call

PO_DIR = 'po'
MO_DIR = os.path.join('build', 'po')

for po in glob.glob(os.path.join(PO_DIR, '*.po')):
    lang = os.path.basename(po[:-3])
    mo = os.path.join(MO_DIR, lang, 'kylin-display-switch.mo')
    target_dir = os.path.dirname(mo)
    if not os.path.isdir(target_dir):
        os.makedirs(target_dir)
    try:
        return_code = call(['msgfmt', '-o', mo, po])
    except OSError:
        print('Translation not available, please install gettext')
        break
    if return_code:
        raise Warning('Error when building locales')


data_files=[
    ('bin/', ['kds']),
    ('../etc/xdg/autostart/', ['kylin-display-switch.desktop']),
    ('share/kylin-display-switch/', ['display_service.py', 'switchers_service.py', 'enums.py', 'key_service_xlib.py', 'kylin_display_switch.py', 'ui_mainwindow.py']),
    ('share/kylin-display-switch/res/', glob.glob('res/*.png')),
    ]

def find_mo_files():
    data_files = []
    for mo in glob.glob(os.path.join(MO_DIR, '*', 'kylin-display-switch.mo')):
        lang = os.path.basename(os.path.dirname(mo))
        dest = os.path.join('share', 'locale', lang, 'LC_MESSAGES')
        data_files.append((dest, [mo]))
    return data_files

data_files.extend(find_mo_files())

setup(name="kylin-display-switch",
      version="1.0.0",
      author="shine",
      author_email="shenghuang@ubuntukylin.com",
      url="www.ubuntukylin.com",
      license="GNU General Public License (GPL)",
      data_files=data_files,
      )