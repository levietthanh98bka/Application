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
#include <QCheckBox>
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

    void fullScreenBtnClick();
    void repeateChanged(bool state);

    void setFocusToVideo();

    void reqFullMode();
    void reqNormalMode();
    void reqBackward();
    void reqForward();
    void reqVolumeUp();
    void reqVolumeDown();

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
    QCheckBox *m_repeatCB = nullptr;
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
    bool m_isRepeat = false;
};

#endif // PLAYMEDIA_H
