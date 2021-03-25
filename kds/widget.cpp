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

#include <QDebug>
#include <QScreen>
#include <QDBusConnection>

#include <kwindowsystem.h>

#include "expendbutton.h"

#define TITLEHEIGHT 90
#define OPTIONSHEIGHT 70
#define BOTTOMHEIGHT 60

#define FIRSTSCREENID 0
#define CLONESCREENID 1
#define EXTENEDSCREENID 2
#define OTHERSCREENID 3
#define ALLMODESID 4


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    setAttribute(Qt::WA_TranslucentBackground, true);

    /* 不在任务栏显示图标 */
    KWindowSystem::setState(winId(), NET::SkipTaskbar | NET::SkipPager);

    QScreen * pScreen = QGuiApplication::screens().at(0);
    QPoint point = pScreen->geometry().center();
    move(point.x() - width()/2, point.y() - height()/2);

    initData();

    setupComponent();
    setupConnect();

    initCurrentStatus(getCurrentStatus());

    QDBusConnection::systemBus().connect(QString(), \
                                         QString(), \
                                         "org.ukui.kds.interface", \
                                         "signalNextOption", \
                                         this, SLOT(nextSelectedOption()));

    QDBusConnection::systemBus().connect(QString(), \
                                         QString(), \
                                         "org.ukui.kds.interface", \
                                         "signalLastOption", \
                                         this, SLOT(lastSelectedOption()));

    QDBusConnection::systemBus().connect(QString(), \
                                         QString(), \
                                         "org.ukui.kds.interface", \
                                         "signalCloseApp", \
                                         this, SLOT(closeApp()));

    QDBusConnection::systemBus().connect(QString(), \
                                         QString(), \
                                         "org.ukui.kds.interface", \
                                         "signalMakeClicked", \
                                         this, SLOT(confirmCurrentOption()));


    QDBusConnection::systemBus().connect(QString(), \
                                         QString(), \
                                         "org.ukui.kds.interface", \
                                         "signalButtonClicked", \
                                         this, SLOT(receiveButtonClick(int,int)));

}

Widget::~Widget()
{
    delete ui;
}

void Widget::initData(){
    btnsGroup = new QButtonGroup;

    gtk_init(NULL, NULL);

    //Monitor init
    kScreen = mate_rr_screen_new (gdk_screen_get_default (), NULL);
//    kConfig = mate_rr_config_new_current (kScreen, NULL);

}

void Widget::setupComponent(){

    setCurrentFirstOutputTip();

    for (int i = 0; i < ALLMODESID; i++){
        ExpendButton * btn = new ExpendButton();
        btn->setFixedHeight(70);
        btnsGroup->addButton(btn, i);

        switch (i) {
#ifdef FIRSTSCREENID
        case FIRSTSCREENID:
            btn->setSign(FIRSTSCREENID % 2);
            btn->setBtnText(tr("First Screen"));
            btn->setBtnLogo(":/img/main.png");
            break;
#endif

#ifdef CLONESCREENID
        case CLONESCREENID:
            btn->setSign(CLONESCREENID % 2);
            btn->setBtnText(tr("Clone Screen"));
            btn->setBtnLogo(":/img/clone.png");
            break;
#endif

#ifdef EXTENEDSCREENID
        case EXTENEDSCREENID:
            btn->setSign(EXTENEDSCREENID % 2);
            btn->setBtnText(tr("Extend Screen"));
            btn->setBtnLogo(":/img/extend.png");
            break;
#endif

#ifdef OTHERSCREENID
        case OTHERSCREENID:
            btn->setSign(OTHERSCREENID % 2);
            btn->setBtnText(tr("Vice Screen"));
            btn->setBtnLogo(":/img/vice.png");
            break;
#endif
        default:
            break;
        }


        ui->btnsVerLayout->addWidget(btn);
    }

    int h = TITLEHEIGHT + OPTIONSHEIGHT * ALLMODESID + BOTTOMHEIGHT;
    setFixedWidth(400);
    setFixedHeight(h);


    /// QSS
    ui->titleFrame->setStyleSheet("QFrame#titleFrame{background: #A6000000; border: none; border-top-left-radius: 24px; border-top-right-radius: 24px;}");
    ui->bottomFrame->setStyleSheet("QFrame#bottomFrame{background: #A6000000; border: none; border-bottom-left-radius: 24px; border-bottom-right-radius: 24px;}");

    ui->splitFrame->setStyleSheet("QFrame#splitFrame{background: #99000000; border: none;}");

    ui->titleLabel->setStyleSheet("QLabel{color: #FFFFFF; font-size: 24px;}");
    ui->outputPrimaryTip->setStyleSheet("QLabel{color: #60FFFFFF; font-size: 18px;}");
    ui->outputName->setStyleSheet("QLabel{color: #60FFFFFF; font-size: 18px;}");
    ui->outputDisplayName->setStyleSheet("QLabel{color: #60FFFFFF; font-size: 18px;}");
}

void Widget::setupConnect(){

    connect(btnsGroup, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), this, [=](int id){


        /* 获取旧选项 */
        for (QAbstractButton * button : btnsGroup->buttons()){
            ExpendButton * btn = dynamic_cast<ExpendButton *>(button);
//            qDebug() << "old index: " << btn->getBtnChecked();
            int index = btnsGroup->id(button);
            if (index == id && btn->getBtnChecked()){
                    goto closeapp;
            }
        }

        MateRRConfig * settingConfig;

        switch (id) {
#ifdef FIRSTSCREENID
        case FIRSTSCREENID:
            settingConfig = makePrimarySetup();
            initCurrentStatus(FIRSTSCREENID);
            break;
#endif

#ifdef CLONESCREENID
        case CLONESCREENID:
            settingConfig = makeCloneSetup();
            initCurrentStatus(CLONESCREENID);
            break;
#endif

#ifdef EXTENEDSCREENID
        case EXTENEDSCREENID:
            settingConfig = makeXineramaSetup();
            initCurrentStatus(EXTENEDSCREENID);
            break;
#endif

#ifdef OTHERSCREENID
        case OTHERSCREENID:
            settingConfig = makeOtherSetup();
            initCurrentStatus(OTHERSCREENID);
            break;
#endif

        default:
            break;
        }

        if (id >= 0 && id < ALLMODESID){
            guint32 timestamp, serverTimestamp;
            gboolean success;
            GError * error;

            error = NULL;
            if (!settingConfig || !mate_rr_config_applicable(settingConfig, kScreen, &error)){
                if (error)
                    g_error_free (error);
                goto closeapp;
            }

            mate_rr_screen_get_timestamps(kScreen, NULL, &serverTimestamp);
//            if (timestamp < serverTimestamp)
//                timestamp = serverTimestamp;

            error = NULL;

            success = mate_rr_config_apply_with_time(settingConfig, kScreen, serverTimestamp, &error);
            if (!success) {
                qDebug() << "Could not switch to the following configuration: " << error->message;
                g_error_free (error);
            }

closeapp:
            close();
        }


    });

}


int Widget::getCurrentStatus(){
    MateRRConfig * current = mate_rr_config_new_current(kScreen, NULL);

    int status;

    if (mate_rr_config_get_clone(current)){
#ifdef CLONESCREENID
        g_object_unref(current);
        current = NULL;
        return CLONESCREENID;
#endif
    } else {

        bool firstOutputActive = true;
        QList<bool> actives;

        char * firstName = _findFirstOutput(current);

        MateRROutputInfo ** outputs = mate_rr_config_get_outputs(current);

        for (int i = 0; outputs[i] != NULL; i++){
            MateRROutputInfo * info = outputs[i];

            if (mate_rr_output_info_is_connected(info)){

                char * pName = mate_rr_output_info_get_name(info);

                if (!mate_rr_output_info_is_active(info)){
                    actives.append(false);

                    if (strcmp(firstName, pName) == 0){
                        firstOutputActive = false;
                    }

                } else {
                    actives.append(true);
                }
            }
        }

        if (actives.length() < 2){
#ifdef FIRSTSCREENID
            g_object_unref(current);
            current = NULL;
            return FIRSTSCREENID;
#endif
        }

        int acs = 0, disacs = 0;

        for (bool b : actives){
            b ? acs++ : disacs++;
        }

        if (acs == actives.length()){
#ifdef EXTENEDSCREENID
            g_object_unref(current);
            current = NULL;
            return EXTENEDSCREENID;
#endif
        } else {
            if (firstOutputActive){
#ifdef FIRSTSCREENID
                g_object_unref(current);
                current = NULL;
                return FIRSTSCREENID;
#endif
            } else {
#ifdef OTHERSCREENID
                g_object_unref(current);
                current = NULL;
                return OTHERSCREENID;
#endif
            }
        }


    }

}

void Widget::initCurrentStatus(int id){
    //set all no checked
    for (QAbstractButton * button : btnsGroup->buttons()){
        ExpendButton * btn = dynamic_cast<ExpendButton *>(button);

        btn->setBtnChecked(false);

        if (id == btnsGroup->id(button)){
            btn->setBtnChecked(true);
            btn->setChecked(true);
        }
    }

    // status == -1
    if (id == -1){
        ExpendButton * btn1 = dynamic_cast<ExpendButton *>(btnsGroup->button(0));
//        btn1->setBtnChecked(true);
        btn1->setChecked(true);
    }
}

void Widget::setCurrentFirstOutputTip(){

    char * pName;
    char * pDisplayName;

    char * firstName;

    MateRRConfig * config = mate_rr_config_new_current(kScreen, NULL);

    MateRROutputInfo ** outputs = mate_rr_config_get_outputs (config);

    firstName = _findFirstOutput(config);

    for (int i = 0; outputs[i] != NULL; i++){
        MateRROutputInfo * info = outputs[i];

        if (mate_rr_output_info_is_connected(info)){

            pName = mate_rr_output_info_get_name(info);
            pDisplayName = mate_rr_output_info_get_display_name(info);

            if (strcmp(firstName, pName) == 0){
                ui->outputName->setText(QString(pName));
                ui->outputDisplayName->setText(""/*QString(pDisplayName)*/);
            }
        }

    }
}

void Widget::nextSelectedOption(){
    int current = btnsGroup->checkedId();
    int next;

    /* no button checked */
//    if (current == -1)
//        ;

    next = current == ALLMODESID - 1 ? 0 : current + 1;

    ExpendButton * btn = dynamic_cast<ExpendButton *>(btnsGroup->button(next));
    btn->setChecked(true);
}

void Widget::lastSelectedOption(){
    int current = btnsGroup->checkedId();
    int last;

    /* no button checked */
    if (current == -1)
        current = ALLMODESID;

    last = current == 0 ? ALLMODESID - 1 : current - 1;

    ExpendButton * btn = dynamic_cast<ExpendButton *>(btnsGroup->button(last));
    btn->setChecked(true);
}

void Widget::confirmCurrentOption(){
    int current = btnsGroup->checkedId();

    if (current == -1)
        return;

    ExpendButton * btn = dynamic_cast<ExpendButton *>(btnsGroup->button(current));
//    btn->click();
    btn->animateClick();
}

void Widget::closeApp(){
    close();
}

MateRRConfig * Widget::makeCloneSetup() {

    MateRRConfig * current;
    MateRROutputInfo ** outputs;

    int width, height;

    //获取克隆屏最佳分辨率
    _getCloneSize(&width, &height);

    current = mate_rr_config_new_current(kScreen, NULL);
    outputs = mate_rr_config_get_outputs(current);
    mate_rr_config_set_clone(current, TRUE);

    for (int i = 0; outputs[i] != NULL; i++){
        MateRROutputInfo *info = outputs[i];

        mate_rr_output_info_set_active (info, FALSE);
        if (mate_rr_output_info_is_connected(info)){
            MateRROutput * output = mate_rr_screen_get_output_by_name(kScreen, mate_rr_output_info_get_name(info));
            MateRRMode ** modes = mate_rr_output_list_modes(output);

            int bestRate = 0;
            for (int j = 0; modes[j] != NULL; j++){
                MateRRMode * mode = modes[j];
                int w, h;

                w = mate_rr_mode_get_width(mode);
                h = mate_rr_mode_get_height(mode);

                if (w == width && h == height) {
                    int r = mate_rr_mode_get_freq(mode);
                    if (r > bestRate)
                        bestRate = r;
                }
            }

            if (bestRate > 0){
                mate_rr_output_info_set_active(info, TRUE);
                mate_rr_output_info_set_rotation(info, MATE_RR_ROTATION_0);
                mate_rr_output_info_set_refresh_rate(info, bestRate);
                mate_rr_output_info_set_geometry(info, 0, 0, width, height);
            }
        }
    }

    if (_configIsAllOff(current)){
        g_object_unref(current);
        current = NULL;
    }

    return current;
}

MateRRConfig * Widget::makePrimarySetup(){

    char * firstName;

    /* Turn on the first screen, disable everything else */
    MateRRConfig * current = mate_rr_config_new_current(kScreen, NULL);
    MateRROutputInfo ** outputs = mate_rr_config_get_outputs(current);

    mate_rr_config_set_clone(current, FALSE);

    /* found first output */
    firstName = _findFirstOutput(current);

    for (int i = 0; outputs[i] != NULL; i++){
        MateRROutputInfo * info = outputs[i];

        char * pName = mate_rr_output_info_get_name(info);

        if (strcmp(firstName, pName) == 0){
            if (!_turnonOutput(info, 0, 0)){
                break;
            }
        } else {
            mate_rr_output_info_set_active (info, FALSE);
        }
    }

    _setNewPrimaryOutput(current);

    if (_configIsAllOff(current)){
        g_object_unref(current);
        current = NULL;
    }

    return current;
}

MateRRConfig * Widget::makeOtherSetup(){

    char * firstName;

    /* Turn off primary output, and make all external monitors clone from (0, 0) */
    MateRRConfig * current = mate_rr_config_new_current(kScreen, NULL);
    MateRROutputInfo ** outputs = mate_rr_config_get_outputs (current);

    mate_rr_config_set_clone(current, FALSE);

    firstName = _findFirstOutput(current);

    for (int i = 0; outputs[i] != NULL; i++){
        MateRROutputInfo * info = outputs[i];

        char * pName = mate_rr_output_info_get_name(info);

        if (strcmp(firstName, pName) == 0){
            mate_rr_output_info_set_active(info, FALSE);
        } else {
            if (mate_rr_output_info_is_connected(info)){
                _turnonOutput(info, 0, 0);
            }
        }
    }

    _setNewPrimaryOutput(current);

    if (_configIsAllOff(current)){
        g_object_unref(current);
        current = NULL;
    }

    return current;
}

MateRRConfig * Widget::makeXineramaSetup(){

    /* Turn on everything that has a preferred mode, and position it from left to right */
    MateRRConfig * current = mate_rr_config_new_current(kScreen, NULL);
    MateRROutputInfo ** outputs = mate_rr_config_get_outputs (current);
    int x;

    mate_rr_config_set_clone(current, FALSE);

    //found primary output
    _setNewPrimaryOutput(current);

    x = 0;
    for (int i = 0; outputs[i] != NULL; i++){
        MateRROutputInfo * info = outputs[i];
        if (mate_rr_output_info_get_primary(info)){
            x = _turnonGetRightmostOffset(info, x);
        }
    }

    for (int i = 0; outputs[i] != NULL; i++){
        MateRROutputInfo * info = outputs[i];
        if (mate_rr_output_info_is_connected(info) && !mate_rr_output_info_get_primary(info)){
            x = _turnonGetRightmostOffset(info, x);
        }
    }

    if (_configIsAllOff(current)){
        g_object_unref(current);
        current = NULL;
    }

    return current;
}

bool Widget::_configIsAllOff(MateRRConfig *config){

    MateRROutputInfo ** outputs;

    outputs = mate_rr_config_get_outputs (config);

    for (int j = 0; outputs[j] != NULL; j++) {
        if (mate_rr_output_info_is_active (outputs[j])) {
            return false;
        }
    }

    return true;
}

bool Widget::_getCloneSize(int *width, int *height) {

    MateRRMode **modes = mate_rr_screen_list_clone_modes (kScreen);
    int bestWidth, bestHeight;

    bestWidth = 0; bestHeight = 0;

    for (int i = 0; modes[i] != NULL; ++i){
        int w, h;
        MateRRMode *mode = modes[i];

        w = mate_rr_mode_get_width (mode);
        h = mate_rr_mode_get_height (mode);

        if (w * h > bestWidth * bestHeight){
            bestWidth = w;
            bestHeight = h;
        }
    }

    if (bestWidth > 0 && bestHeight > 0){
        if (width)
            *width = bestWidth;
        if (height)
            *height = bestHeight;

        return true;
    }

    return false;
}

bool Widget::_isLaptop(MateRROutputInfo * info){
    /* 返回该输出是否是笔记本屏幕 */

    MateRROutput * output;

    output = mate_rr_screen_get_output_by_name (kScreen, mate_rr_output_info_get_name (info));
    return mate_rr_output_is_laptop(output);
}

bool Widget::_setNewPrimaryOutput(MateRRConfig *config){
    MateRROutputInfo ** outputs = mate_rr_config_get_outputs (config);

    for (int i = 0; outputs[i] != NULL; i++){
        MateRROutputInfo * info = outputs[i];

        if (mate_rr_output_info_is_active(info)){

            if (mate_rr_output_info_get_primary(info)){
                return true;
            }
        }
    }

    //top left output is primary
    mate_rr_config_ensure_primary(config);

    return true;
}

char *Widget::_findFirstOutput(MateRRConfig *config){

    int firstid = -1;
    char * firstname;

    MateRROutputInfo ** outputs = mate_rr_config_get_outputs (config);

    for (int i = 0; outputs[i] != NULL; i++){
        MateRROutputInfo * info = outputs[i];

        if (mate_rr_output_info_is_connected(info)){

            MateRROutput * output = mate_rr_screen_get_output_by_name(kScreen, mate_rr_output_info_get_name(info));

            int currentid = mate_rr_output_get_id(output);

            if (firstid == -1){
                firstid = currentid;
                firstname = mate_rr_output_info_get_name(info);
                continue;
            }

            if (firstid > currentid){
                firstid = currentid;
                firstname = mate_rr_output_info_get_name(info);
            }
        }

    }

    return firstname;
}

bool Widget::_turnonOutput(MateRROutputInfo *info, int x, int y){

    MateRROutput * output = mate_rr_screen_get_output_by_name (kScreen, mate_rr_output_info_get_name (info));
    MateRRMode * mode = _findBestMode(output);

    if (mode) {
        mate_rr_output_info_set_active (info, TRUE);
        mate_rr_output_info_set_geometry (info, x, y, mate_rr_mode_get_width (mode), mate_rr_mode_get_height (mode));
        mate_rr_output_info_set_rotation (info, MATE_RR_ROTATION_0);
        mate_rr_output_info_set_refresh_rate (info, mate_rr_mode_get_freq (mode));

        return true;
    }

    return false;
}

MateRRMode * Widget::_findBestMode(MateRROutput *output){
    MateRRMode * preferred;
    MateRRMode ** modes;
    MateRRMode * bestMode;
    int bestSize;
    int bestWidth, bestHeight, bestRate;

    preferred = mate_rr_output_get_preferred_mode(output);

    if (preferred)
        return preferred;

    modes = mate_rr_output_list_modes(output);
    if (!modes)
        return NULL;

    bestSize = bestWidth = bestHeight = bestRate = 0;
    bestMode = NULL;

    for (int i = 0; modes[i] != NULL; i++){
        int w, h, r;
        int size;

        w = mate_rr_mode_get_width (modes[i]);
        h = mate_rr_mode_get_height (modes[i]);
        r = mate_rr_mode_get_freq  (modes[i]);

        size = w * h;

        if (size > bestSize){
            bestSize = size;
            bestWidth = w;
            bestHeight = h;
            bestRate = r;
            bestMode = modes[i];
        } else if (size == bestSize){
            if (r > bestRate){
                bestRate = r;
                bestMode = modes[i];
            }
        }
    }

    return bestMode;
}

int Widget::_turnonGetRightmostOffset(MateRROutputInfo *info, int x){
    if (_turnonOutput(info, x, 0)){
        int width;
        mate_rr_output_info_get_geometry (info, NULL, NULL, &width, NULL);
        x += width;
    }

    return x;
}

void Widget::msgReceiveAnotherOne(const QString &msg){
//    qDebug() << "another one " << msg;
    nextSelectedOption();
}

void Widget::receiveButtonClick(int x, int y){
//    qDebug() << "receive button press " << x << y;
    if (!this->geometry().contains(x, y)){
        close();
    }

}
