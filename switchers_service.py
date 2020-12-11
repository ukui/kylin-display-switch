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

import sys
import subprocess


class SwitchersService:

    def get_capslock_status(self):
        handle = subprocess.Popen("xset q", shell=True, stdout=subprocess.PIPE)
        handle.wait()
        res = handle.communicate()[0]
        res = res.decode()
        capslock = res[res.find("Caps Lock:") + 9:]
        capslock = capslock[capslock.find("o"):capslock.find("o") + 2]

        if capslock == "on":
            return True
        else:
            return False

    def get_numlock_status(self):
        handle = subprocess.Popen("xset q", shell=True, stdout=subprocess.PIPE)
        handle.wait()
        res = handle.communicate()[0]
        res = res.decode()
        numlock = res[res.find("Num Lock:") + 9:]
        numlock = numlock[numlock.find("o"):numlock.find("o") + 2]

        if numlock == "on":
            return True
        else:
            return False


def main():
    w = SwitchersService()
    print(w.get_capslock_status())


if __name__ == '__main__':
    main()
