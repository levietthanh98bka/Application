#ifndef PLAYMEDIA_H
#define PLAYMEDIA_H

#include <QWidget>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <common/qslidercustom.h>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QUrl>
#include <QTime>
#include <common/loghelper.h>

class PlayMedia : public QWidget
{
private:
    Q_OBJECT
public:
    PlayMedia(QWidget *parent = nullptr);
    void setVideo(QString filePath){
        m_player->setVideoOutput(m_video);
        m_player->setMedia(QUrl::fromLocalFile(filePath));
        m_video->show();
        m_video->setFocus();
        m_player->play();
    }

signals:
    void stop();
    void next();
    void previous();
    void changeVolume(int volume);
    void changeMuting(bool muting);
    void changeRate(qreal rate);

public slots:
    void playClicked();
    void muteClicked();

    QMediaPlayer::State playerState() const;
    void setPlayerState(const QMediaPlayer::State &playerState);

    int volume() const;
    void setVolume(int volume);

    bool isMuted() const;
    void setMuted(bool muted);

    qreal playbackRate() const;
    void setPlaybackRate(float rate);


    void updateRate();
    void onVolumeSliderValueChanged();

    void seek(int seconds);
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);

    void updateDurationInfo(qint64 currentInfo);

    void fullScreenBtnClick(){
        LOG_INFO << m_isFullMode;
        m_isFullMode = !m_isFullMode;
        emit showFullScreen(m_isFullMode);
    }

    void setFocusToVideo(){
        setFocus();
    }

    void reqFullMode(){
        m_slider->hide();
        m_labelDuration->hide();
        m_fullScreenButton->hide();
        m_playButton->hide();
        m_stopButton->hide();
        m_nextButton->hide();
        m_previousButton->hide();
        m_muteButton->hide();
        m_volumeSlider->hide();
        m_rateBox->hide();
    }

    void reqNormalMode(){
        m_slider->show();
        m_labelDuration->show();
        m_fullScreenButton->show();
        m_playButton->show();
        m_stopButton->show();
        m_nextButton->show();
        m_previousButton->show();
        m_muteButton->show();
        m_volumeSlider->show();
        m_rateBox->show();
    }

    void reqBackward(){
        if(m_player->position() >= 5000){
            m_player->setPosition(m_player->position() - 5000);
        }
    }

    void reqForward(){
        if(m_player->position() < m_duration*1000){
            m_player->setPosition(m_player->position() + 5000);
        }
    }

    void reqVolumeUp(){
        if(m_player->volume() <= 95)
         m_player->setVolume(m_player->volume() + 5);
        else {
            m_player->setVolume(100);
        }
    }

    void reqVolumeDown(){
        if(m_player->volume() >= 5)
         m_player->setVolume(m_player->volume() - 5);
        else {
            m_player->setVolume(0);
        }
    }

signals:
    void showFullScreen(bool isFull);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
//    void mousePressEvent(QMouseEvent *event) override;

private:
    QMediaPlayer *m_player = nullptr;
    QVideoWidget *m_video = nullptr;
    QSliderCustom *m_slider = nullptr;
    QLabel *m_labelDuration = nullptr;
    qint64 m_duration;
    QPushButton *m_fullScreenButton = nullptr;

    // control layout
    QMediaPlayer::State m_playerState = QMediaPlayer::StoppedState;
    bool m_playerMuted = false;
    QAbstractButton *m_playButton = nullptr;
    QAbstractButton *m_stopButton = nullptr;
    QAbstractButton *m_nextButton = nullptr;
    QAbstractButton *m_previousButton = nullptr;
    QAbstractButton *m_muteButton = nullptr;
    QAbstractSlider *m_volumeSlider = nullptr;
    QComboBox *m_rateBox = nullptr;



    bool m_isFullMode = false;

};

#endif // PLAYMEDIA_H
