#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QString>
#include <QMediaPlayer>

class MusicPlayer
{
private:
    QString musicPath;
    QMediaPlayer *mediaPlayer;
    bool paused;

public:
    MusicPlayer(QString t_musicPath);
    bool start();
    bool pause();
    bool stop();
};

#endif // MUSICPLAYER_H
