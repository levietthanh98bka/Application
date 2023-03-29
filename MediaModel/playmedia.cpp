#include "playmedia.h"

PlayMedia::PlayMedia(QWidget *parent) : QWidget(parent)
{
    m_player = new QMediaPlayer;
}

bool PlayMedia::isPlayerAvailable() const
{
    return m_player->isAvailable();
}

void PlayMedia::initialVideo()
{
    //---------------------------------------------------------------------------------------------------
    m_videoWidget = new VideoWidget(this);
    m_player->setVideoOutput(m_videoWidget);
    m_slider = new QSliderCustom(Qt::Horizontal);
    m_labelDuration = new QLabel();
    controls = new PlayerControls;
    m_fullScreenButton = new QPushButton(tr("FullScreen"));
    m_fullScreenButton->setCheckable(true);
    //---------------------------------------------------------------------------------------------------


    connect(m_player, &QMediaPlayer::durationChanged, this, &PlayMedia::durationChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &PlayMedia::positionChanged);
//    connect(m_player, QOverload<>::of(&QMediaPlayer::metaDataChanged), this, &PlayMedia::metaDataChanged);
//    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, this, &Player::playlistPositionChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &PlayMedia::statusChanged);
//    connect(m_player, &QMediaPlayer::bufferStatusChanged, this, &PlayMedia::bufferingProgress);
    connect(m_player, &QMediaPlayer::videoAvailableChanged, this, &PlayMedia::videoAvailableChanged);
//    connect(m_player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &PlayMedia::displayErrorMessage);
    connect(m_player, &QMediaPlayer::stateChanged, this, &PlayMedia::stateChanged);
    connect(m_player, &QMediaPlayer::mutedChanged, this, &PlayMedia::slotMuteChanged);

    connect(m_player, &QMediaPlayer::stateChanged, controls, &PlayerControls::setState);
    connect(m_player, &QMediaPlayer::volumeChanged, controls, &PlayerControls::setVolume);
    connect(m_player, &QMediaPlayer::mutedChanged, controls, &PlayerControls::setMuted);
    //---------------------------------------------------------------------------------------------------

    connect(m_videoWidget,&VideoWidget::signalFullScreenVideo,this,&PlayMedia::slotForwardFullScreenMode);
    connect(m_videoWidget,&VideoWidget::signalPause,controls, &PlayerControls::pause);
    connect(m_videoWidget,&VideoWidget::signalPlay,controls, &PlayerControls::play);
    connect(m_videoWidget, &VideoWidget::signalMute, controls, &PlayerControls::changeMuting);
    connect(m_videoWidget,&VideoWidget::signalBackward,this,&PlayMedia::backward);
    connect(m_videoWidget,&VideoWidget::signalForward,this,&PlayMedia::forward);
    connect(m_videoWidget,&VideoWidget::signalVolumeUp,this,&PlayMedia::volumeUp);
    connect(m_videoWidget,&VideoWidget::signalVolumeDown,this,&PlayMedia::volumeDown);
    //---------------------------------------------------------------------------------------------------

//    connect(m_slider,&QSlider::sliderReleased,m_slider,&QSlider::sliderMoved);
//    m_slider->setTracking(false);
    connect(m_slider, &QSliderCustom::pressedEv, this, &PlayMedia::seek);
//    connect(m_slider, &QSlider::sliderMoved, this, &PlayMedia::seek);
    //---------------------------------------------------------------------------------------------------

    connect(controls, &PlayerControls::play, m_player, &QMediaPlayer::play);
    connect(controls, &PlayerControls::pause, m_player, &QMediaPlayer::pause);
    connect(controls, &PlayerControls::stop, m_player, &QMediaPlayer::stop);
//    connect(controls, &PlayerControls::next, m_playlist, &QMediaPlaylist::next);
//    connect(controls, &PlayerControls::previous, this, &QMediaPlayer::previousClicked);
    connect(controls, &PlayerControls::changeVolume, m_player, &QMediaPlayer::setVolume);
    connect(controls, &PlayerControls::changeMuting, m_player, &QMediaPlayer::setMuted);
    connect(controls, &PlayerControls::changeRate, m_player, &QMediaPlayer::setPlaybackRate);
    connect(controls, &PlayerControls::stop, m_videoWidget, QOverload<>::of(&QVideoWidget::update));
    //---------------------------------------------------------------------------------------------------

    connect(m_fullScreenButton, &QPushButton::clicked, this, &PlayMedia::slotForwardFullScreenMode);
    //--------------------------------- Layout view -----------------------------------------------------

    QHBoxLayout *displayLayout = new QHBoxLayout;
    displayLayout->addWidget(m_videoWidget);
//    QWidget *m_playlistView = new QWidget;
//    displayLayout->addWidget(m_playlistView);

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->addStretch(1);
    controlLayout->addWidget(controls);
    controlLayout->addStretch(1);
    controlLayout->addWidget(m_fullScreenButton);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(m_slider);
    hLayout->addWidget(m_labelDuration);

    QBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(displayLayout);
    layout->addLayout(hLayout);
    layout->addLayout(controlLayout);

    layout->setMargin(0);

    setLayout(layout);
    //-------------------------------- End define layout view ------------------------------------------//
    //-------------------------------- Default state ---------------------------------------------------//
    controls->setState(m_player->state());
    controls->setVolume(m_player->volume());
    controls->setMuted(controls->isMuted());

    if (!isPlayerAvailable()) {
        QMessageBox::warning(this, tr("Service not available"),
                             tr("The QMediaPlayer object does not have a valid service.\n"\
                                "Please check the media service plugins are installed."));

        controls->setEnabled(false);
//        m_playlistView->setEnabled(false);
//        m_fullScreenButton->setEnabled(false);
    }
    //    metaDataChanged();
}

void PlayMedia::initialAudio()
{
    //---------------------------------------------------------------------------------------------------
    m_slider = new QSliderCustom(Qt::Horizontal);
    m_labelDuration = new QLabel();
    controls = new PlayerControls;
    m_fullScreenButton = new QPushButton(tr("FullScreen"));
    m_fullScreenButton->setCheckable(true);
    //---------------------------------------------------------------------------------------------------

    connect(m_player, &QMediaPlayer::durationChanged, this, &PlayMedia::durationChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &PlayMedia::positionChanged);
//    connect(m_player, QOverload<>::of(&QMediaPlayer::metaDataChanged), this, &PlayMedia::metaDataChanged);
//    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, this, &Player::playlistPositionChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &PlayMedia::statusChanged);
//    connect(m_player, &QMediaPlayer::bufferStatusChanged, this, &PlayMedia::bufferingProgress);
//    connect(m_player, &QMediaPlayer::videoAvailableChanged, this, &PlayMedia::videoAvailableChanged);
//    connect(m_player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &PlayMedia::displayErrorMessage);
//    connect(m_player, &QMediaPlayer::stateChanged, this, &PlayMedia::stateChanged);
    connect(m_player, &QMediaPlayer::mutedChanged, this, &PlayMedia::slotMuteChanged);

    connect(m_player, &QMediaPlayer::stateChanged, controls, &PlayerControls::setState);
    connect(m_player, &QMediaPlayer::volumeChanged, controls, &PlayerControls::setVolume);
    connect(m_player, &QMediaPlayer::mutedChanged, controls, &PlayerControls::setMuted);
    //---------------------------------------------------------------------------------------------------

//    connect(m_slider,&QSlider::sliderReleased,m_slider,&QSlider::sliderMoved);
//    m_slider->setTracking(false);
    connect(m_slider, &QSliderCustom::pressedEv, this, &PlayMedia::seek);
//    connect(m_slider, &QSlider::sliderMoved, this, &PlayMedia::seek);
    //---------------------------------------------------------------------------------------------------

    connect(controls, &PlayerControls::play, m_player, &QMediaPlayer::play);
    connect(controls, &PlayerControls::pause, m_player, &QMediaPlayer::pause);
    connect(controls, &PlayerControls::stop, m_player, &QMediaPlayer::stop);
//    connect(controls, &PlayerControls::next, m_playlist, &QMediaPlaylist::next);
//    connect(controls, &PlayerControls::previous, this, &QMediaPlayer::previousClicked);
    connect(controls, &PlayerControls::changeVolume, m_player, &QMediaPlayer::setVolume);
    connect(controls, &PlayerControls::changeMuting, m_player, &QMediaPlayer::setMuted);
    connect(controls, &PlayerControls::changeRate, m_player, &QMediaPlayer::setPlaybackRate);
    //---------------------------------------------------------------------------------------------------

    connect(m_fullScreenButton, &QPushButton::clicked, this, &PlayMedia::slotForwardFullScreenMode);
    //--------------------------------- Layout view -----------------------------------------------------

    QHBoxLayout *displayLayout = new QHBoxLayout;
    QWidget *audioWidget = new QWidget;
    displayLayout->addWidget(audioWidget);
//    QWidget *m_playlistView = new QWidget;
//    displayLayout->addWidget(m_playlistView);

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->addStretch(1);
    controlLayout->addWidget(controls);
    controlLayout->addStretch(1);
    controlLayout->addWidget(m_fullScreenButton);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(m_slider);
    hLayout->addWidget(m_labelDuration);

    QBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(displayLayout);
    layout->addLayout(hLayout);
    layout->addLayout(controlLayout);

    layout->setMargin(0);

    setLayout(layout);
    //-------------------------------- End define layout view ------------------------------------------//
    //-------------------------------- Default state ---------------------------------------------------//
    controls->setState(m_player->state());
    controls->setVolume(m_player->volume());
    controls->setMuted(controls->isMuted());

    if (!isPlayerAvailable()) {
        QMessageBox::warning(this, tr("Service not available"),
                             tr("The QMediaPlayer object does not have a valid service.\n"\
                                "Please check the media service plugins are installed."));

        controls->setEnabled(false);
//        m_playlistView->setEnabled(false);
//        m_fullScreenButton->setEnabled(false);
    }
    //    metaDataChanged();
}

void PlayMedia::durationChanged(qint64 duration)
{
    LOG_INFO << duration;
    m_duration = duration / 1000;
    m_slider->setMaximum(m_duration);
}

void PlayMedia::positionChanged(qint64 progress)
{
//    LOG_INFO << progress;
    if (!m_slider->isSliderDown())
        m_slider->setValue(progress / 1000);

    updateDurationInfo(progress / 1000);
}

void PlayMedia::metaDataChanged()
{
//    if (m_player->isMetaDataAvailable()) {
//        setTrackInfo(QString("%1 - %2")
//                .arg(m_player->metaData(QMediaMetaData::AlbumArtist).toString())
//                .arg(m_player->metaData(QMediaMetaData::Title).toString()));

//        if (m_coverLabel) {
//            QUrl url = m_player->metaData(QMediaMetaData::CoverArtUrlLarge).value<QUrl>();

//            m_coverLabel->setPixmap(!url.isEmpty()
//                    ? QPixmap(url.toString())
//                    : QPixmap());
//        }
//    }
}

void PlayMedia::previousClicked()
{
    // Go to previous track if we are within the first 5 seconds of playback
    // Otherwise, seek to the beginning.
//    if (m_player->position() <= 5000)
//        m_playlist->previous();
//    else
//        m_player->setPosition(0);
}

void PlayMedia::seek(int seconds)
{
    m_player->setPosition(seconds * 1000);

}


void PlayMedia::jump(const QModelIndex &index)
{
    if (index.isValid()) {
//        m_playlist->setCurrentIndex(index.row());
        m_player->play();
    }
}

void PlayMedia::playlistPositionChanged(int)
{

}

void PlayMedia::backward()
{
    LOG_INFO;
    if(m_player->position() >= 5000){
        m_player->setPosition(m_player->position() - 5000);
    }
}

void PlayMedia::forward()
{
    LOG_INFO;
    if(m_player->position() < m_duration*1000){
        m_player->setPosition(m_player->position() + 5000);
    }
}

void PlayMedia::volumeUp()
{
    LOG_INFO << "start " << m_player->volume();
    m_player->setVolume(m_player->volume() + 5);
    LOG_INFO << "end " << m_player->volume();
}

void PlayMedia::volumeDown()
{
    LOG_INFO << "start " << m_player->volume();
    m_player->setVolume(m_player->volume() - 5);
    LOG_INFO << "end " << m_player->volume();
}

void PlayMedia::statusChanged(QMediaPlayer::MediaStatus status)
{
    Q_UNUSED(status);
}

void PlayMedia::stateChanged(QMediaPlayer::State state)
{
    if(state == QMediaPlayer::PlayingState){
        m_videoWidget->setPlayOrPause(true);
    }else{
        m_videoWidget->setPlayOrPause(false);
    }
}

void PlayMedia::slotMuteChanged(bool mute)
{
    LOG_INFO << mute;
    if(mute != m_videoWidget->mute())
        m_videoWidget->setMute(mute);
}

void PlayMedia::bufferingProgress(int progress)
{

}

void PlayMedia::videoAvailableChanged(bool available)
{
    LOG_INFO;
    if (!available) {
        LOG_INFO;
        disconnect(m_fullScreenButton, &QPushButton::clicked, m_videoWidget, &QVideoWidget::setFullScreen);
        disconnect(m_videoWidget, &QVideoWidget::fullScreenChanged, m_fullScreenButton, &QPushButton::setChecked);
        m_videoWidget->setFullScreen(false);
    } else {
        LOG_INFO;
//        connect(m_fullScreenButton, &QPushButton::clicked, m_videoWidget, &QVideoWidget::setFullScreen);
//        connect(m_videoWidget, &QVideoWidget::fullScreenChanged, m_fullScreenButton, &QPushButton::setChecked);

//        connect(m_fullScreenButton, &QPushButton::clicked, this, &PlayMedia::showFull);
//        if (m_fullScreenButton->isChecked()){
//            LOG_INFO;
////            m_videoWidget->setFullScreen(true);
//            this->showFullScreen();
//        }
    }
//    m_colorButton->setEnabled(available);
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

void PlayMedia::slotForwardFullScreenMode(){
    m_videoWidget->setFullScreenMode(!m_videoWidget->fullScreenMode());
    signalFullScreenMode(m_videoWidget->fullScreenMode());
}

