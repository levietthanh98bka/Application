#include "playmedia.h"

PlayMedia::PlayMedia(QWidget *parent) : QWidget(parent)
{
    m_player = new QMediaPlayer;
//    QPalette pal = QPalette();

    // set black background
    // Qt::black / "#000000" / "black"
//    pal.setColor(QPalette::Window, Qt::black);

//    setAutoFillBackground(true);
//    setPalette(pal);

    connect(m_player, &QMediaPlayer::durationChanged, this, &PlayMedia::durationChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &PlayMedia::positionChanged);
    connect(m_player, QOverload<>::of(&QMediaPlayer::metaDataChanged), this, &PlayMedia::metaDataChanged);
//    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, this, &Player::playlistPositionChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &PlayMedia::statusChanged);
    connect(m_player, &QMediaPlayer::bufferStatusChanged, this, &PlayMedia::bufferingProgress);
    connect(m_player, &QMediaPlayer::videoAvailableChanged, this, &PlayMedia::videoAvailableChanged);
//    connect(m_player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &PlayMedia::displayErrorMessage);
    connect(m_player, &QMediaPlayer::stateChanged, this, &PlayMedia::stateChanged);

    m_videoWidget = new VideoWidget(this);
    m_player->setVideoOutput(m_videoWidget);

    m_slider = new QSlider(Qt::Horizontal);

    m_labelDuration = new QLabel();
    connect(m_slider, &QSlider::sliderMoved, this, &PlayMedia::seek);

    controls = new PlayerControls;
    controls->setState(m_player->state());
    controls->setVolume(m_player->volume());
    controls->setMuted(controls->isMuted());

    connect(controls, &PlayerControls::play, m_player, &QMediaPlayer::play);
    connect(controls, &PlayerControls::pause, m_player, &QMediaPlayer::pause);
    connect(controls, &PlayerControls::stop, m_player, &QMediaPlayer::stop);
//    connect(controls, &PlayerControls::next, m_playlist, &QMediaPlaylist::next);
//    connect(controls, &PlayerControls::previous, this, &QMediaPlayer::previousClicked);
    connect(controls, &PlayerControls::changeVolume, m_player, &QMediaPlayer::setVolume);
    connect(controls, &PlayerControls::changeMuting, m_player, &QMediaPlayer::setMuted);
    connect(controls, &PlayerControls::changeRate, m_player, &QMediaPlayer::setPlaybackRate);
    connect(controls, &PlayerControls::stop, m_videoWidget, QOverload<>::of(&QVideoWidget::update));

    connect(m_player, &QMediaPlayer::stateChanged, controls, &PlayerControls::setState);
    connect(m_player, &QMediaPlayer::volumeChanged, controls, &PlayerControls::setVolume);
    connect(m_player, &QMediaPlayer::mutedChanged, controls, &PlayerControls::setMuted);

    m_fullScreenButton = new QPushButton(tr("FullScreen"));
    m_fullScreenButton->setCheckable(true);

    displayLayout = new QHBoxLayout;
    displayLayout->addWidget(m_videoWidget);
//    QWidget *m_playlistView = new QWidget;
//    displayLayout->addWidget(m_playlistView);

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
//    controlLayout->addWidget(openButton);
    controlLayout->addStretch(1);
    controlLayout->addWidget(controls);
    controlLayout->addStretch(1);
    controlLayout->addWidget(m_fullScreenButton);


    layout = new QVBoxLayout;
    layout->addLayout(displayLayout);
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(m_slider);
    hLayout->addWidget(m_labelDuration);
    layout->addLayout(hLayout);
    layout->addLayout(controlLayout);

    setLayout(layout);
    layout->setMargin(0);

    if (!isPlayerAvailable()) {
        QMessageBox::warning(this, tr("Service not available"),
                             tr("The QMediaPlayer object does not have a valid service.\n"\
                                "Please check the media service plugins are installed."));

        controls->setEnabled(false);
//        m_playlistView->setEnabled(false);

//        m_fullScreenButton->setEnabled(false);
    }
//    connect(m_videoWidget,&VideoWidget::modeViewChanged, this, &PlayMedia::slotModeViewChanged);

    metaDataChanged();


}

bool PlayMedia::isPlayerAvailable() const
{
    return m_player->isAvailable();
}

void PlayMedia::durationChanged(qint64 duration)
{
    m_duration = duration / 1000;
    m_slider->setMaximum(m_duration);
}

void PlayMedia::positionChanged(qint64 progress)
{
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

void PlayMedia::statusChanged(QMediaPlayer::MediaStatus status)
{

}

void PlayMedia::stateChanged(QMediaPlayer::State state)
{

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

