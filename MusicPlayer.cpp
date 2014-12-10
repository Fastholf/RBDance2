#include "MusicPlayer.h"

MusicPlayer::MusicPlayer(QString t_musicPath)
{
    qDebug() << "Method name";

    musicPath = t_musicPath;
    mediaPlayer = new QMediaPlayer();
    mediaPlayer->setMedia(QUrl::fromLocalFile(musicPath));
    mediaPlayer->setVolume(100);
    paused = false;
}

bool MusicPlayer::start()
{
    qDebug() << "Method name";

    mediaPlayer->play();
    return true;
}

bool MusicPlayer::pause()
{
    qDebug() << "Method name";

    if (!paused) {
        mediaPlayer->pause();
    }
    else {
        mediaPlayer->play();
    }
    paused = !paused;

    return true;
}

bool MusicPlayer::isPaused()
{
    return paused;
}

bool MusicPlayer::stop()
{
    qDebug() << "Method name";

    if (paused) {
        mediaPlayer->play();
    }
    mediaPlayer->stop();
    return true;
}
