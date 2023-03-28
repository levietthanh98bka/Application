/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "videowidget.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <loghelper.h>

VideoWidget::VideoWidget(QWidget *parent)
    : QVideoWidget(parent)
{
//    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    QPalette p = palette();
    p.setColor(QPalette::Window, Qt::black);
    setPalette(p);
    setAttribute(Qt::WA_OpaquePaintEvent);
}

void VideoWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && m_fullScreenMode) {
        LOG_INFO << "Key_Escape";
        emit signalFullScreenVideo();
    }else if (event->key() == Qt::Key_F){
        emit signalFullScreenVideo();
        LOG_INFO << "Key_F";
    }
    else if (event->key() == Qt::Key_Space) {
        LOG_INFO << "Key_Space";
        if(m_playOrPause){
            LOG_INFO << "emit play";
            emit signalPause();
        }else{
            LOG_INFO << "emit pause";
            emit signalPlay();
        }
        event->accept();
    }else if(event->key() == Qt::Key_M){
        LOG_INFO << "Key M " << !m_mute;
        m_mute = !m_mute;
        emit signalMute(m_mute);
    } else if(event->key() == Qt::Key_Left){
        LOG_INFO << "Key Left ";
        emit signalBackward();
    } else if(event->key() == Qt::Key_Right){
        LOG_INFO << "Key Right ";
        emit signalForward();
    } else if(event->key() == Qt::Key_Up){
        LOG_INFO << "Key Up ";
        emit signalVolumeUp();
    } else if(event->key() == Qt::Key_Down){
        LOG_INFO << "Key Down ";
        emit signalVolumeDown();
    }
    else {
        QVideoWidget::keyPressEvent(event);
    }

    event->accept();
}

void VideoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    emit signalFullScreenVideo();
    event->accept();
}

bool VideoWidget::fullScreenMode() const
{
    return m_fullScreenMode;
}

void VideoWidget::setFullScreenMode(bool fullScreenMode)
{
    m_fullScreenMode = fullScreenMode;
    LOG_INFO << m_fullScreenMode;
}

void VideoWidget::mousePressEvent(QMouseEvent *event)
{
    QVideoWidget::mousePressEvent(event);
}

bool VideoWidget::mute() const
{
    return m_mute;
}

void VideoWidget::setMute(bool mute)
{
    m_mute = mute;
}

void VideoWidget::setPlayOrPause(bool playOrPause)
{
    m_playOrPause = playOrPause;
}

bool VideoWidget::playOrPause() const
{
    return m_playOrPause;
}

