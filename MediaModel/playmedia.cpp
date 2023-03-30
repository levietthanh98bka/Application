#include "playmedia.h"
#include <QToolButton>
#include <QBoxLayout>

PlayMedia::PlayMedia(QWidget *parent) : QWidget(parent)
{
    m_player = new QMediaPlayer;
    m_video = new QVideoWidget;
    m_slider = new QSliderCustom(Qt::Horizontal);
    m_labelDuration = new QLabel;
    m_fullScreenButton = new QPushButton("FullScreen");


    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    // init for control layout//
    m_playButton = new QToolButton;
    m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(m_playButton, &QAbstractButton::clicked, this, &PlayMedia::playClicked);

    m_stopButton = new QToolButton;
    m_stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    m_stopButton->setEnabled(false);

    connect(m_stopButton, &QAbstractButton::clicked, this, &PlayMedia::stop);

    m_nextButton = new QToolButton;
    m_nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));

    connect(m_nextButton, &QAbstractButton::clicked, this, &PlayMedia::next);

    m_previousButton = new QToolButton;
    m_previousButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));

    connect(m_previousButton, &QAbstractButton::clicked, this, &PlayMedia::previous);

    m_muteButton = new QToolButton;
    m_muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

    connect(m_muteButton, &QAbstractButton::clicked, this, &PlayMedia::muteClicked);

    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);

    connect(m_volumeSlider, &QSlider::valueChanged, this, &PlayMedia::onVolumeSliderValueChanged);

    m_rateBox = new QComboBox(this);
    m_rateBox->addItem("0.5x", QVariant(0.5));
    m_rateBox->addItem("1.0x", QVariant(1.0));
    m_rateBox->addItem("2.0x", QVariant(2.0));
    m_rateBox->setCurrentIndex(1);

    connect(m_rateBox, QOverload<int>::of(&QComboBox::activated), this, &PlayMedia::updateRate);

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->addStretch(1);
    controlLayout->addWidget(m_stopButton);
    controlLayout->addWidget(m_previousButton);
    controlLayout->addWidget(m_playButton);
    controlLayout->addWidget(m_nextButton);
    controlLayout->addWidget(m_muteButton);
    controlLayout->addWidget(m_volumeSlider);
    controlLayout->addWidget(m_rateBox);
    controlLayout->addStretch(1);
    controlLayout->addWidget(m_fullScreenButton);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(m_slider);
    hLayout->addWidget(m_labelDuration);

    QBoxLayout *displayLayout = new QHBoxLayout;
    displayLayout->addWidget(m_video,0);


    QBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(displayLayout);
    layout->addLayout(hLayout);
    layout->addLayout(controlLayout);

    layout->setMargin(0);

    setLayout(layout);


    // connect signal slot
    connect(m_player, &QMediaPlayer::stateChanged, this, &PlayMedia::setPlayerState);
    connect(m_player, &QMediaPlayer::volumeChanged, this, &PlayMedia::setVolume);
    connect(this, &PlayMedia::changeVolume, m_player, &QMediaPlayer::setVolume);
    connect(m_player, &QMediaPlayer::mutedChanged, this, &PlayMedia::setMuted);
    connect(this, &PlayMedia::changeRate, m_player, &QMediaPlayer::setPlaybackRate);

    connect(m_slider, &QSliderCustom::pressedEv, this, &PlayMedia::seek);

    connect(m_player, &QMediaPlayer::durationChanged, this, &PlayMedia::durationChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &PlayMedia::positionChanged);

    connect(m_fullScreenButton, &QPushButton::clicked, this, &PlayMedia::fullScreenBtnClick);

    setPlayerState(m_player->state());
    setVolume(m_player->volume());
}

int PlayMedia::volume() const
{
    qreal linearVolume =  QAudio::convertVolume(m_volumeSlider->value() / qreal(100),
                                                QAudio::LogarithmicVolumeScale,
                                                QAudio::LinearVolumeScale);

    return qRound(linearVolume * 100);
}

void PlayMedia::setVolume(int volume)
{
    qreal logarithmicVolume = QAudio::convertVolume(volume / qreal(100),
                                                    QAudio::LinearVolumeScale,
                                                    QAudio::LogarithmicVolumeScale);

    m_volumeSlider->setValue(qRound(logarithmicVolume * 100));
}

bool PlayMedia::isMuted() const
{
    return m_playerMuted;
}

void PlayMedia::setMuted(bool muted)
{
    LOG_INFO << muted;
    if (muted != m_playerMuted) {
        m_playerMuted = muted;

        m_muteButton->setIcon(style()->standardIcon(muted
                                                    ? QStyle::SP_MediaVolumeMuted
                                                    : QStyle::SP_MediaVolume));
    }
}

qreal PlayMedia::playbackRate() const
{
    return m_rateBox->itemData(m_rateBox->currentIndex()).toDouble();
}

void PlayMedia::setPlaybackRate(float rate)
{
    for (int i = 0; i < m_rateBox->count(); ++i) {
        if (qFuzzyCompare(rate, float(m_rateBox->itemData(i).toDouble()))) {
            m_rateBox->setCurrentIndex(i);
            return;
        }
    }

    m_rateBox->addItem(QString("%1x").arg(static_cast<double>(rate)), QVariant(rate));
    m_rateBox->setCurrentIndex(m_rateBox->count() - 1);
}

void PlayMedia::playClicked()
{
    switch (m_playerState) {
    case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
        m_player->play();
        break;
    case QMediaPlayer::PlayingState:
        m_player->pause();
        break;
    }
}

void PlayMedia::muteClicked()
{
    m_player->setMuted(!m_playerMuted);
}

QMediaPlayer::State PlayMedia::playerState() const
{
    return m_playerState;
}

void PlayMedia::setPlayerState(const QMediaPlayer::State &playerState)
{
    if (playerState != m_playerState) {
        m_playerState = playerState;

        switch (playerState) {
        case QMediaPlayer::StoppedState:
            m_stopButton->setEnabled(false);
            m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        case QMediaPlayer::PlayingState:
            m_stopButton->setEnabled(true);
            m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        case QMediaPlayer::PausedState:
            m_stopButton->setEnabled(true);
            m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        }
    }
}

void PlayMedia::updateRate()
{
    emit changeRate(playbackRate());
}

void PlayMedia::onVolumeSliderValueChanged()
{
    emit changeVolume(volume());
}

void PlayMedia::seek(int seconds)
{
    m_player->setPosition(seconds * 1000);
}

void PlayMedia::durationChanged(qint64 duration)
{
    m_duration = duration / 1000;
    m_slider->setMaximum(static_cast<int>(m_duration));
}

void PlayMedia::positionChanged(qint64 progress)
{
    if (!m_slider->isSliderDown())
        m_slider->setValue(static_cast<int>(progress / 1000));

    updateDurationInfo(progress / 1000);
}

void PlayMedia::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || m_duration) {
        QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60,
                          currentInfo % 60, (currentInfo * 1000) % 1000);
        QTime totalTime((m_duration / 3600) % 60, (m_duration / 60) % 60,
                        m_duration % 60, (m_duration * 1000) % 1000);
        QString format = "mm:ss";
        if (m_duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    m_labelDuration->setText(tStr);
}

void PlayMedia::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && m_isFullMode) {
        LOG_INFO << "Key_Escape";
        m_isFullMode = false;
        emit showFullScreen(m_isFullMode);
    }else if (event->key() == Qt::Key_F){
        LOG_INFO << "Key_F";
        fullScreenBtnClick();
    }
    else if (event->key() == Qt::Key_Space) {
        LOG_INFO << "Key_Space";
        playClicked();
    }else if(event->key() == Qt::Key_M){
        LOG_INFO << "Key M ";
        muteClicked();
    } else if(event->key() == Qt::Key_Left){
        LOG_INFO << "Key Left ";
        reqBackward();
    } else if(event->key() == Qt::Key_Right){
        LOG_INFO << "Key Right ";
        reqForward();
    } else if(event->key() == Qt::Key_Up){
        LOG_INFO << "Key Up ";
        reqVolumeUp();
    } else if(event->key() == Qt::Key_Down){
        LOG_INFO << "Key Down ";
        reqVolumeDown();
    }
    else {
        PlayMedia::keyPressEvent(event);
    }
}

void PlayMedia::mouseDoubleClickEvent(QMouseEvent *event)
{
    m_isFullMode = !m_isFullMode;
    emit showFullScreen(m_isFullMode);
    event->accept();
}
