#ifndef PLAYMEDIA_H
#define PLAYMEDIA_H

#include <QWidget>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <MediaModel/playercontrols.h>
#include <QHBoxLayout>
#include <QTime>
#include <QModelIndex>
#include <QUrl>
#include <common/loghelper.h>
#include <MediaModel/videowidget.h>
#include <MediaModel/qslidercustom.h>


class PlayMedia : public QWidget
{
    Q_OBJECT
public:
    explicit PlayMedia(QWidget *parent = nullptr);

    bool isPlayerAvailable() const;
    void initialVideo();
    void initialAudio();

    void setAudio(QString filePath){
        initialAudio();
        LOG_INFO << filePath;
        filePath = "qrc:/sample/dptk.mp3";
        m_player->setMedia(QUrl::fromLocalFile("/home/thanh/Desktop/Application/sample/dptk.mp3"));
        LOG_INFO;
        m_player->play();
        LOG_INFO;
    }

    void setVideo(QString filePath){
        initialVideo();
        m_player->setVideoOutput(m_videoWidget);
        m_player->setMedia(QUrl::fromLocalFile(filePath));
        m_videoWidget->show();
        m_videoWidget->setFocus();
        m_player->play();
    }
    void showFullMode(){
        controls->hide();
        m_fullScreenButton->hide();
        m_slider->hide();
        m_labelDuration->hide();
    }
    void showNormalMode(){
        controls->show();
        m_fullScreenButton->show();
        m_slider->show();
        m_labelDuration->show();
    }
    void setFocusToVideo(){
        m_videoWidget->setFocus();
    }

signals:
    void signalFullScreenMode(bool fullScreenMode);

public slots:
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void metaDataChanged();

    void previousClicked();

    void seek(int seconds);
    void jump(const QModelIndex &index);
    void playlistPositionChanged(int);
    void backward();
    void forward();
    void volumeUp();
    void volumeDown();

    void statusChanged(QMediaPlayer::MediaStatus status);
    void stateChanged(QMediaPlayer::State state);
    void slotMuteChanged(bool mute);
    void bufferingProgress(int progress);
    void videoAvailableChanged(bool available);

    void updateDurationInfo(qint64 currentInfo);

    void slotForwardFullScreenMode();
private:
    QMediaPlayer *m_player = nullptr;
    VideoWidget *m_videoWidget = nullptr;
    PlayerControls *controls = nullptr;
    QSliderCustom *m_slider = nullptr;
    QLabel *m_labelDuration = nullptr;
    qint64 m_duration;
    QPushButton *m_fullScreenButton = nullptr;
//    QLabel *m_statusLabel = nullptr;
//    QString m_trackInfo;
//    QString m_statusInfo;

};

#endif // PLAYMEDIA_H
