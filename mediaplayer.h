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
	void reloadMedia(const QString &mediaPath);
	void reloadMediaAndPlay(const QString &mediaPath);
	void play();
	void playPause();
	void stop();
	void previous();
	void next();

	void onError(QMediaPlayer::Error error);
	void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
	void onStateChanged(QMediaPlayer::State state);
};

#endif // MEDIAPLAYER_H
