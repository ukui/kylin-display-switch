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
#include "ui_widget.h"

#include <QDesktopWidget>
#include <QDBusConnection>

#include <QProcess>

#include <QDebug>

#define POSBOTTOM 5

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);

    setAttribute(Qt::WA_TranslucentBackground, true);


    QDesktopWidget* m = QApplication::desktop();
//    QRect rect = m->screenGeometry(m->screenNumber(QCursor::pos()));
    QRect rect = m->availableGeometry();
    move(rect.width() * 3 /4, rect.height() - POSBOTTOM - this->height());

    setupComponent();

    pTimer = new QTimer(this);
    pTimer->setInterval(2500);

    connect(pTimer, &QTimer::timeout, this, [=]{
        pTimer->stop();
        this->hide();
    });

    QDBusConnection::systemBus().connect(QString(), QString(), "org.ukui.kds.interface", "signalShowTips", this, SLOT(showTipsOnDesktop(int)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setupComponent(){
    ui->capsOnLabel->setStyleSheet("border-image:url(:/img/capslockOn.png); border: none; border-radius: 6px;");
    ui->capsOffLabel->setStyleSheet("border-image:url(:/img/capslockOff.png); border: none; border-radius: 6px;");
    ui->numOnLabel->setStyleSheet("border-image:url(:/img/numlockOn.png); border: none; border-radius: 6px;");
    ui->numOffLabel->setStyleSheet("border-image:url(:/img/numlockOff.png); border: none; border-radius: 6px;");
}

void Widget::showTipsOnDesktop(int index){
    this->hide();
    ui->stackedWidget->setCurrentIndex(index);
    this->show();
    pTimer->start();
}
