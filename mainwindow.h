/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QScrollArea>
#include <QLabel>
#include <QDebug>
#include <QRegExp>
#include <codeeditor.h>
#include <loghelper.h>
#include <QStandardPaths>
#include <QImageReader>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QUrl>
#include <playmedia.h>
#include <QStatusBar>
#include <QMenuBar>
#include <QDockWidget>



QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPlainTextEdit;
class QSessionManager;
QT_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    void loadFileText(const QString &fileName);
    void loadImage(const QString &fileName);
    void loadVideo(const QString &fileName);
    void loadAudio(const QString &fileName);
    enum TYPE_FILE {
        IMAGE,
        MUSIC,
        VIDEO,
        MP3,
        TEXT
    };
signals:
    void typeFileChanged();
protected:
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void documentWasModified();
    void about();
#ifndef QT_NO_SESSIONMANAGER
    void commitData(QSessionManager &);
#endif
    void slotTypeFileChanged();

    void slotFullScreenMode(bool modeView){
        //modeView == true -> fullScreenMode
        LOG_INFO << "mode view: " << modeView;
        if(modeView){
            player->showFull();
            statusBar()->hide();
            menuBar()->hide();
            showFullScreen();
        }else{
            player->myShowNormal();
            statusBar()->show();
            menuBar()->show();
            showNormal();
        }
    }



private:
    void createActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    bool saveFileText(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    void getTypeFile(QString str);
    void updateActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);


    TYPE_FILE m_typeFile;

    CodeEditor *m_textEdit;
    QImage m_image;
    QString m_curFile;
    QLabel *m_label;
    QScrollArea* m_scrollArea;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *pasteAct;
    double scaleFactor;

    PlayMedia *player;

    // QWidget interface

};
//! [0]

#endif
