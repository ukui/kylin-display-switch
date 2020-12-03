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
#include "widget.h"
#include <QApplication>
#include "qtsingleapplication.h"

#include <QDebug>

int main(int argc, char *argv[])
{

    QString id = QString("kds" + QLatin1String(getenv("DISPLAY")));

    QtSingleApplication app(id, argc, argv);
    if (app.isRunning()){
        app.sendMessage("hello world!");
        return 0; /* EXIT_SUCCESS */
    }

    Widget w;
//    QObject::connect(&app, SIGNAL(messageReceived(const QString&, NULL)), &w, SLOT(msgReceiveAnotherOne(QString)));
    QObject::connect(&app, &QtSingleApplication::messageReceived, &w, &Widget::msgReceiveAnotherOne);
    w.show();

    return app.exec();

}
