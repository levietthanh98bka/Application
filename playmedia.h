#ifndef PLAYMEDIA_H
#define PLAYMEDIA_H

#include <QWidget>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <playercontrols.h>
#include <QHBoxLayout>
#include <QTime>
#include <QModelIndex>
#include <QUrl>
#include <loghelper.h>
#include <videowidget.h>


class PlayMedia : public QWidget
{
    Q_OBJECT
public:
    explicit PlayMedia(QWidget *parent = nullptr);

    bool isPlayerAvailable() const;
    void setVideo(QString filePath){
        m_player->setVideoOutput(m_videoWidget);
        m_player->setMedia(QUrl::fromLocalFile(filePath));
        m_videoWidget->show();
        m_player->play();
    }

signals:

public slots:
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void metaDataChanged();

    void previousClicked();

    void seek(int seconds);
    void jump(const QModelIndex &index);
    void playlistPositionChanged(int);

    void statusChanged(QMediaPlayer::MediaStatus status);
    void stateChanged(QMediaPlayer::State state);
    void bufferingProgress(int progress);
    void videoAvailableChanged(bool available);

    void updateDurationInfo(qint64 currentInfo);

//    void showFull(){
//        LOG_INFO;
//        showFullScreen();
//    }
//    void slotModeViewChanged(){
//        LOG_INFO;
//        showNormal();
//    }

public:
    VideoWidget *m_videoWidget = nullptr;
    QBoxLayout *displayLayout;
    QBoxLayout *layout;
    QMediaPlayer *m_player = nullptr;
    QPushButton *m_fullScreenButton = nullptr;
    PlayerControls *controls = nullptr;
    QSlider *m_slider = nullptr;
    QLabel *m_labelDuration = nullptr;
private:


    QLabel *m_statusLabel = nullptr;


    QString m_trackInfo;
    QString m_statusInfo;
    qint64 m_duration;

};

#endif // PLAYMEDIA_H
