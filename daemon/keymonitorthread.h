/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#ifndef KEYMONITORTHREAD_H
#define KEYMONITORTHREAD_H

#include <QThread>
#include <X11/Xlib.h>
#include <X11/extensions/record.h>


class KeyMonitorThread : public QThread
{
    Q_OBJECT

public:
    explicit KeyMonitorThread(QObject *parent = 0);
    ~KeyMonitorThread();

public:
    void run();
    void callJobComplete();

signals:
    void keyPress(KeySym keysym, KeyCode keyCode);
    void keyRelease(KeySym keysym, KeyCode keyCode);
    void jobComplete();

protected:
    static void callback(XPointer trash, XRecordInterceptData* data);
    void handleRecordEvent(XRecordInterceptData *);

private:
    Display * display;

};

#endif // KEYMONITORTHREAD_H
