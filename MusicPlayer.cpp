#include "MusicPlayer.h"

MusicPlayer::MusicPlayer(QString t_musicPath)
{
    musicPath = t_musicPath;
    mediaPlayer = new QMediaPlayer();
    mediaPlayer->setMedia(QUrl::fromLocalFile(musicPath));
    mediaPlayer->setVolume(100);
    paused = false;
}

bool MusicPlayer::start()
{
    mediaPlayer->play();
    return true;
}

bool MusicPlayer::pause()
{
    if (!paused) {
        mediaPlayer->pause();
    }
    else {
        mediaPlayer->play();
    }
    paused = !paused;

    return true;
}

bool MusicPlayer::stop()
{
    if (paused) {
        mediaPlayer->play();
    }
    mediaPlayer->stop();
    return true;
}
