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


    QRect screenRect = QApplication::desktop()->screenGeometry(QApplication::desktop()->primaryScreen());

//    QRect rect = m->availableGeometry();
    move(screenRect.width() * 3 /4, screenRect.height() - POSBOTTOM - this->height());

    pMappingTable = new MappingTable;

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

    delete pMappingTable;
}

void Widget::setupComponent(){

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

void Widget::showTipsOnDesktop(int index){
    this->hide();
    ui->stackedWidget->setCurrentIndex(index);
    this->show();
    pTimer->start();
}
