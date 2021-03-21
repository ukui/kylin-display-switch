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
#include "rfkillmonitorthread.h"

#include <linux/types.h>
#include <sys/poll.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

struct rfkill_event {
    __u32 idx;
    __u8  type;
    __u8  op;
    __u8  soft, hard;
};

#define RFKILL_EVENT_SIZE_V1	8


RfkillMonitorthread::RfkillMonitorthread()
{

}

RfkillMonitorthread::~RfkillMonitorthread(){

}

void RfkillMonitorthread::run(){

    struct rfkill_event event;
    struct pollfd p;
    ssize_t len;
    int fd, n;

    fd = open("/dev/rfkill", O_RDONLY);
    if (fd < 0) {
        qCritical("Can not open RFKILL control device!\n");
        return;
    }

    memset(&p, 0, sizeof(p));
    p.fd = fd;
    p.events = POLLIN | POLLHUP;

    while (1) {
        n = poll(&p, 1, -1);
        if (n < 0) {
            qWarning("Failed to poll RFKILL control device!\n");
            break;
        }

        if (n == 0) {
            continue;
        }

        len = read(fd, &event, sizeof(event));
        if (len < 0) {
            qWarning("Reading of RFKILL events failed!\n");
            break;
        }

        if (len != RFKILL_EVENT_SIZE_V1){
            qDebug("Wrong size of RFKILL event!\n");
            continue;
        }

        qDebug("idx %u type %u op %u soft %u hard %u\n", event.idx, event.type, event.op, event.soft, event.hard);
        emit statusChanged();

        fflush(stdout);
    }

    close(fd);

    //线程结束
    callJobComplete();

}

void RfkillMonitorthread::callJobComplete(){
    emit jobComplete();
}
