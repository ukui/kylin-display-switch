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
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QButtonGroup>

#ifdef signals
#undef signals
#endif

extern "C" {
#define MATE_DESKTOP_USE_UNSTABLE_API
#include <libmate-desktop/mate-rr.h>
#include <libmate-desktop/mate-rr-config.h>
#include <libmate-desktop/mate-rr-labeler.h>
}


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public:
    void setupComponent();
    void setupConnect();

    MateRRConfig * makeCloneSetup();
    MateRRConfig * makePrimarySetup();
    MateRRConfig * makeOtherSetup();
    MateRRConfig * makeXineramaSetup();

    void initCurrentStatus();

private:
    Ui::Widget *ui;
    QButtonGroup * btnsGroup;

private:
    MateRRScreen * kScreen;
//    MateRRConfig * kConfig;

private:
    bool _getCloneSize(int * width, int * height);
    bool _isLaptop(MateRROutputInfo * info);
    bool _findPrimaryOutput(MateRRConfig * config);
    bool _turnonOutput(MateRROutputInfo * info, int x, int y);
    MateRRMode * _findBestMode(MateRROutput * output);
    int _turnonGetRightmostOffset(MateRROutputInfo * info, int x);
    bool _configIsAllOff(MateRRConfig * config);

    char * primaryName;
};

#endif // WIDGET_H
