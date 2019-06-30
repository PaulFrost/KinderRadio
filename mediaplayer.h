#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>


class MediaPlayer : public QObject
{
	Q_OBJECT
public:
	explicit MediaPlayer(QObject *parent = nullptr);

private:
	 QMediaPlayer *m_mediaPlayer;
	 QMediaPlaylist *m_mediaPlaylist;
signals:
	 void statusChanged(const QString &status);

public slots:
	 void reloadMedia();
	 void play();
	 void stop();
	 void previous();
	 void next();

	 void onError(QMediaPlayer::Error error);
	 void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
};

#endif // MEDIAPLAYER_H
