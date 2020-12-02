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
#include <QDebug>
#include <QTimer>
#include <QDBusConnection>

#include "expendbutton.h"

enum {
    MAINSCREEN,
    CLONESCREEN,
    EXTENDSCREEN,
    VICESCREEN,
};

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);

    setAttribute(Qt::WA_TranslucentBackground, true);

    QDesktopWidget* m = QApplication::desktop();
//    QRect rect = m->screenGeometry(m->screenNumber(QCursor::pos()));
    QRect rect = m->screenGeometry(m->screenNumber(QCursor::pos()));
    move((rect.width() - this->width())/2, (rect.height() - this->height()) / 2);

    initData();

    setupComponent();
    setupConnect();

    initCurrentStatus();

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

}

Widget::~Widget()
{
    delete ui;

    if (primaryConfig){
        g_object_unref(primaryConfig);
        primaryConfig = NULL;
    }

    if (cloneConfig){
        g_object_unref(cloneConfig);
        cloneConfig = NULL;
    }

    if (extendConfig){
        g_object_unref(extendConfig);
        extendConfig = NULL;
    }

    if (viceConfig){
        g_object_unref(viceConfig);
        viceConfig = NULL;
    }
}

void Widget::initData(){
    btnsGroup = new QButtonGroup;

    //Monitor init
    kScreen = mate_rr_screen_new (gdk_screen_get_default (), NULL);
//    kConfig = mate_rr_config_new_current (kScreen, NULL);

    primaryName = NULL;

    primaryConfig = makePrimarySetup();
    cloneConfig = makeCloneSetup();
    extendConfig = makeXineramaSetup();
    viceConfig = makeOtherSetup();


}

void Widget::setupComponent(){

    btnsGroup->addButton(ui->mainBtn, MAINSCREEN);
    btnsGroup->addButton(ui->cloneBtn, CLONESCREEN);
    btnsGroup->addButton(ui->extendBtn, EXTENDSCREEN);
    btnsGroup->addButton(ui->viceBtn, VICESCREEN);

    for (QAbstractButton * button : btnsGroup->buttons()){
        ExpendButton * btn = dynamic_cast<ExpendButton *>(button);

        switch (btnsGroup->id(btn)) {
        case MAINSCREEN:
            btn->setSign(MAINSCREEN % 2);
            btn->setBtnText(tr("Main Screen"));
            btn->setBtnLogo(":/img/main.png");
            break;
        case CLONESCREEN:
            btn->setSign(CLONESCREEN % 2);
            btn->setBtnText(tr("Clone Screen"));
            btn->setBtnLogo(":/img/clone.png");
            break;
        case EXTENDSCREEN:
            btn->setSign(EXTENDSCREEN % 2);
            btn->setBtnText(tr("Extend Screen"));
            btn->setBtnLogo(":/img/extend.png");
            break;
        case VICESCREEN:
            btn->setSign(VICESCREEN % 2);
            btn->setBtnText(tr("Vice Screen"));
            btn->setBtnLogo(":/img/vice.png");
            break;
        default:
            break;
        }

        qDebug() << "current btn checked:" << btn->isChecked();
    }


    /// QSS
    ui->titleFrame->setStyleSheet("QFrame#titleFrame{background: #A6000000; border: none; border-top-left-radius: 24px; border-top-right-radius: 24px;}");
//    ui->mainBtn->setStyleSheet("QPushButton#mainBtn{background: #99000000; border: none;}"
//                               "QPushButton#mainBtn:hover{background: #000000; border: none;}");
//    ui->cloneBtn->setStyleSheet("QPushButton#cloneBtn{background: #A6000000; border: none;}"
//                                "QPushButton#cloneBtn:hover{background: #000000; border: none;}");
//    ui->extendBtn->setStyleSheet("QPushButton#extendBtn{background: #99000000; border: none;}"
//                                 "QPushButton#extendBtn:hover{background: #000000; border: none;}");
//    ui->viceBtn->setStyleSheet("QPushButton#viceBtn{background: #A6000000; border: none;}"
//                               "QPushButton#viceBtn:hover{background: #000000; border: none;}");
    ui->phoneFrame->setStyleSheet("QFrame#phoneFrame{background: #A6000000; border: none; border-bottom-left-radius: 24px; border-bottom-right-radius: 24px;}");


    ui->splitFrame->setStyleSheet("QFrame#splitFrame{background: #99000000; border: none;}");

    ui->titleLabel->setStyleSheet("QLabel{color: #FFFFFF; font-size: 24px;}");
}

void Widget::setupConnect(){

    connect(btnsGroup, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), this, [=](int id){

        MateRRConfig * current;

        switch (id) {
        case MAINSCREEN:
            current = makePrimarySetup();
            setCurrentStatus(MAINSCREEN);
            break;
        case CLONESCREEN:
            current = makeCloneSetup();
            setCurrentStatus(CLONESCREEN);
            break;
        case EXTENDSCREEN:
            current = makeXineramaSetup();
            setCurrentStatus(EXTENDSCREEN);
            break;
        case VICESCREEN:
            current = makeOtherSetup();
            setCurrentStatus(VICESCREEN);
            break;
        default:
            break;
        }

        if (id >= MAINSCREEN && id <= VICESCREEN){
            guint32 timestamp, serverTimestamp;
            gboolean success;
            GError * error;

            mate_rr_screen_get_timestamps(kScreen, NULL, &serverTimestamp);
//            if (timestamp < serverTimestamp)
//                timestamp = serverTimestamp;

            error = NULL;

            success = mate_rr_config_apply_with_time(current, kScreen, serverTimestamp, &error);
            if (!success) {
                qDebug() << "Could not switch to the following configuration: " << error->message;
                g_error_free (error);
            }

            qDebug() << "success: " << success;
        }


    });
}


void Widget::initCurrentStatus(){
    MateRRConfig * current = mate_rr_config_new_current(kScreen, NULL);

    int status;

    if (mate_rr_config_equal(current, primaryConfig)){
        qDebug() << "init status:" << MAINSCREEN;
        status = MAINSCREEN;
    } else if (mate_rr_config_equal(current, cloneConfig)){
        qDebug() << "init status:" << CLONESCREEN;
        status = CLONESCREEN;
    } else if (mate_rr_config_equal(current, extendConfig)){
        qDebug() << "init status:" << EXTENDSCREEN;
        status = EXTENDSCREEN;
    } else if (mate_rr_config_equal(current, viceConfig)){
        qDebug() << "init status:" << VICESCREEN;
        status = VICESCREEN;
    } else {
        qDebug() << "init status:" << -1;
        status = -1;
    }

    g_object_unref(current);
    current = NULL;

    setCurrentStatus(status);

}

void Widget::setCurrentStatus(int id){
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
        ExpendButton * btn1 = dynamic_cast<ExpendButton *>(btnsGroup->button(MAINSCREEN));
        btn1->setBtnChecked(true);
        btn1->setChecked(true);
    }

}

void Widget::nextSelectedOption(){
    int current = btnsGroup->checkedId();
    int next;

    /* no button checked */
    if (current == -1)
        ;

    next = current == VICESCREEN ? MAINSCREEN : current + 1;

    ExpendButton * btn = dynamic_cast<ExpendButton *>(btnsGroup->button(next));
    btn->setChecked(true);
}

void Widget::lastSelectedOption(){
    int current = btnsGroup->checkedId();
    int last;

    /* no button checked */
    if (current == -1)
        current = VICESCREEN + 1;

    last = current == MAINSCREEN ? VICESCREEN : current - 1;

    ExpendButton * btn = dynamic_cast<ExpendButton *>(btnsGroup->button(last));
    btn->setChecked(true);
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

    /* Turn on the laptop, disable everything else */
    MateRRConfig * current = mate_rr_config_new_current(kScreen, NULL);
    MateRROutputInfo ** outputs = mate_rr_config_get_outputs(current);

    mate_rr_config_set_clone(current, FALSE);

    //found primary output
    _findPrimaryOutput(current);

    for (int i = 0; outputs[i] != NULL; i++){
        MateRROutputInfo * info = outputs[i];
        if (mate_rr_output_info_get_primary(info)){
            if (!_turnonOutput(info, 0, 0)){
                break;
            }
        } else {
            mate_rr_output_info_set_active (info, FALSE);
        }
    }

    if (_configIsAllOff(current)){
        g_object_unref(current);
        current = NULL;
    }

    return current;
}

MateRRConfig * Widget::makeOtherSetup(){

    /* Turn off primary output, and make all external monitors clone from (0, 0) */
    MateRRConfig * current = mate_rr_config_new_current(kScreen, NULL);
    MateRROutputInfo ** outputs = mate_rr_config_get_outputs (current);

    mate_rr_config_set_clone(current, FALSE);

    //found primary output
    _findPrimaryOutput(current);

    for (int i = 0; outputs[i] != NULL; i++){
        MateRROutputInfo * info = outputs[i];

        if (mate_rr_output_info_get_primary(info)){
            mate_rr_output_info_set_active(info, FALSE);
        } else {
            if (mate_rr_output_info_is_connected(info)){
                _turnonOutput(info, 0, 0);
            }
        }
    }

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
    _findPrimaryOutput(current);

    x = 0;
    for (int i = 0; outputs[i] != NULL; i++){
        MateRROutputInfo * info = outputs[i];
        if (mate_rr_output_info_get_primary(info))
            x = _turnonGetRightmostOffset(info, x);
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

bool Widget::_findPrimaryOutput(MateRRConfig *config){
    MateRROutputInfo ** outputs = mate_rr_config_get_outputs (config);

    for (int i = 0; outputs[i] != NULL; i++){
        MateRROutputInfo * info = outputs[i];

        if (mate_rr_output_info_get_primary(info)){
            primaryName = mate_rr_output_info_get_name(info);
            return true;
        }

        //
        if (primaryName && strcmp(primaryName, mate_rr_output_info_get_name(info)) == 0){
            mate_rr_output_info_set_primary(info, TRUE);
            return true;
        }
    }

    //top left output is primary
    mate_rr_config_ensure_primary(config);

    return true;
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
