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
#include <QScreen>
#include <QProcess>

#include <QVBoxLayout>
#include <QLabel>

#include <QDebug>

#define POSBOTTOM 50

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);

    setAttribute(Qt::WA_TranslucentBackground, true);


    pMappingTable = new MappingTable;

    iface = new QDBusInterface("org.ukui.kds", \
                               "/", \
                               "org.ukui.kds.interface", \
                               QDBusConnection::systemBus());

    setupComponent();

    createTrayIcon();

    pTimer = new QTimer(this);
    pTimer->setInterval(2500);

    connect(pTimer, &QTimer::timeout, this, [=]{
        pTimer->stop();
        this->hide();
    });

    connect(QApplication::desktop(), &QDesktopWidget::resized, this, [=]{
        setMKTgeometry();
    });
    connect(QApplication::desktop(), &QDesktopWidget::screenCountChanged, this, [=]{
        setMKTgeometry();
    });
    connect(QApplication::desktop(), &QDesktopWidget::primaryScreenChanged, this, [=]{
        setMKTgeometry();
    });


    QDBusConnection::systemBus().connect(QString(), QString(), "org.ukui.kds.interface", "signalShowTips", this, SLOT(showTipsOnDesktop(int)));
}

Widget::~Widget()
{
    delete ui;

    delete pMappingTable;
}

void Widget::setMKTgeometry(){
    QScreen * pScreen = QGuiApplication::screens().at(0);
    QRect rect = pScreen->geometry();
    move(rect.right() - (rect.width() * 1 / 4), (rect.bottom() + 1) - POSBOTTOM - height());
}

void Widget::setupComponent(){

    setMKTgeometry();

    for (int i = 0; i < MappingTable::HandleKeys; i++){
        QString funWord = pMappingTable->keyCodeToString(i);
        QString logoStr = QString(":/img/%1.png").arg(funWord);

        QFrame * baseFrame = new QFrame;
        QVBoxLayout * mainVerLayout = new QVBoxLayout(baseFrame);
        mainVerLayout->setSpacing(0);
        mainVerLayout->setMargin(0);

        QLabel * logoLabel = new QLabel(baseFrame);
        logoLabel->setScaledContents(true);
        logoLabel->setPixmap(QPixmap(logoStr));


        mainVerLayout->addWidget(logoLabel);

        baseFrame->setLayout(mainVerLayout);

        ui->stackedWidget->addWidget(baseFrame);
    }
}

void Widget::createTrayIcon(){

    QDBusReply<int> reply = iface->call("getCurrentFlightMode");
    /* 未获取到当前飞行模式状态 */
    if (!reply.isValid())
        return;

    int current = reply.value();

    /* 获取飞行模式出错 | 计算机没有无线设备 */
    if (current == -1)
        return;

    trayIcon = new QSystemTrayIcon;
    if (current){
        trayIcon->setIcon(QIcon::fromTheme("airplane-mode-symbolic"));
    } else {
        trayIcon->setIcon(QIcon::fromTheme("airplane-mode-closed-symbolic"));
    }

    trayIcon->setVisible(true);
    trayIcon->setToolTip(tr("Airplane Mode"));

    connect(trayIcon, &QSystemTrayIcon::activated, this, [=](QSystemTrayIcon::ActivationReason reason){
        switch (reason) {
        case QSystemTrayIcon::Trigger:
            flightToggleClick();
            break;
        case QSystemTrayIcon::DoubleClick:
            break;
        case QSystemTrayIcon::MiddleClick:
            break;
        default:
            ;
        }
    });
}

void Widget::showTipsOnDesktop(int index){
    this->hide();
    ui->stackedWidget->setCurrentIndex(index);
    this->show();
    pTimer->start();
}

void Widget::flightToggleClick(){

    QDBusReply<int> reply = iface->call("getCurrentFlightMode");
    if (reply.isValid()){
        int current = reply.value();

        if (current == -1){
//            qWarning("Error Occur When Get Current FlightMode");
            return;
        }

        QDBusReply<QString> reply2 = iface->call("toggleFlightMode", !current);
        if (reply.isValid()){
            QString result = reply2.value();
            if (result == QString("blocked")){
//                qDebug("Enable Flight Mode!\n");
                trayIcon->setIcon(QIcon::fromTheme("airplane-mode-symbolic"));
                showTipsOnDesktop(MappingTable::FlightOn);
            } else if (result == QString("unblocked")){
//                qDebug("Disable Flight Mode!\n");
                trayIcon->setIcon(QIcon::fromTheme("airplane-mode-closed-symbolic"));
                showTipsOnDesktop(MappingTable::FlightOff);
            } else {
//                qWarning("%s", result.toLatin1().data());
            }
        } else {
//            qWarning("Toggle Flight Mode Failed!");
        }


    } else {
//        qWarning("Get Current FlightMode Failed!");
    }
}
