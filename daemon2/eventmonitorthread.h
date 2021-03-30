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
#ifndef EVENTMONITORTHREAD_H
#define EVENTMONITORTHREAD_H

#include <QObject>
#include <QMap>

class EventMonitorThread : public QObject
{
    Q_OBJECT

public:
    explicit EventMonitorThread(QMap<QString, QString> keyword);
    ~EventMonitorThread();

public:
    void run();

    void callJobComplete();

private:
    QMap<QString, QString> _keyword;

Q_SIGNALS:
    void eventMeet(int code);

    void jobComplete();

};

#endif // EVENTMONITORTHREAD_H
