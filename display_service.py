#!/usr/bin/python
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
from enums import DisplayMode


class DisplayService:

    current_display_mode = DisplayMode.MODE_ONLY_ORI

    def switch_display(self, target_mode):
        XRANDR_STATUS = "xrandr -q"

        handle = subprocess.Popen(XRANDR_STATUS, shell=True, stdout=subprocess.PIPE)
        handle.wait()
        res = handle.communicate()[0]
        res = res.decode()
        lines = res.splitlines()

        monitors = []
        modes = {}
        modes_one = []
        current_monitor = None
        flag = False

        for line in lines:
            if(line.find(" connected") != -1):
                monitor_name = line[: line.find(" ")]

                monitors.append(monitor_name)

                # find monitor again，add to dict
                if(flag == True):
                    modes[current_monitor] = modes_one

                current_monitor = monitor_name
                flag = True

                modes_one = []
            else:
                if(flag == True and line.startswith(" ") == True):
                    modes_one.append(line.strip().split(" ")[0])

        # add last monitor's mode
        modes[current_monitor] = modes_one

        print("found monitors :", monitors, " select mode :", target_mode)

        if(len(monitors) < 2):
            handle = subprocess.Popen("xrandr --output " + monitors[0] + " --auto", shell=True, stdout=subprocess.PIPE)
            handle.wait()
            self.current_display_mode = DisplayMode.MODE_ONLY_ORI
            print("only one monitor, do nothing.")
            return (self.current_display_mode, False)

        else:
            modes_0 = modes[monitors[0]]
            modes_1 = modes[monitors[1]]
            MAX_MODE_MONITOR_0 = modes_0[0]
            MAX_MODE_MONITOR_1 = modes_1[0]
            BEST_CLONE_MODE = None

            is_find = False
            for mode_0 in modes_0:
                for mode_1 in modes_1:
                    if(mode_0 == mode_1):
                        BEST_CLONE_MODE = mode_0
                        is_find = True
                        break
                if(is_find == True):
                    break

            print("max display mode :", MAX_MODE_MONITOR_0, ",", MAX_MODE_MONITOR_1)
            print("best clone mode :", BEST_CLONE_MODE)

            XRANDR_ORIONLY = "xrandr --output " + monitors[0] + " --auto --output " + monitors[1] + " --off"
            XRANDR_CLONE = "xrandr --output " + monitors[0] + " --mode " + BEST_CLONE_MODE + " --output " + monitors[1] + " --mode " + BEST_CLONE_MODE + " --same-as " + monitors[0]
            XRANDR_EXTEND = "xrandr --output " + monitors[0] + " --auto --output " + monitors[1] + " --right-of " + monitors[0] + " --auto"
            XRANDR_OUTONLY = "xrandr --output " + monitors[0] + " --off --output " + monitors[1] + " --auto"

            try:
                if(target_mode == DisplayMode.MODE_ONLY_ORI):
                    handle = subprocess.Popen(XRANDR_ORIONLY, shell=True, stdout=subprocess.PIPE)
                    handle.wait()
                    self.current_display_mode = DisplayMode.MODE_ONLY_ORI

                elif(target_mode == DisplayMode.MODE_CLONE):
                    handle = subprocess.Popen(XRANDR_CLONE, shell=True, stdout=subprocess.PIPE)
                    handle.wait()
                    self.current_display_mode = DisplayMode.MODE_CLONE

                elif(target_mode == DisplayMode.MODE_EXTEND):
                    handle = subprocess.Popen(XRANDR_EXTEND, shell=True, stdout=subprocess.PIPE)
                    handle.wait()
                    handle = subprocess.Popen(XRANDR_EXTEND, shell=True, stdout=subprocess.PIPE)
                    handle.wait()
                    self.current_display_mode = DisplayMode.MODE_EXTEND

                elif(target_mode == DisplayMode.MODE_ONLY_OUT):
                    handle = subprocess.Popen(XRANDR_OUTONLY, shell=True, stdout=subprocess.PIPE)
                    handle.wait()
                    self.current_display_mode = DisplayMode.MODE_ONLY_OUT

                return (self.current_display_mode, True)
            except Exception as e:
                print(e)
                return (self.current_display_mode, False)


def main():
    # app = QApplication(sys.argv)

    w = DisplayService()
    w.switch_display(1)

    # sys.exit(app.exec_())


if __name__ == '__main__':
    main()
