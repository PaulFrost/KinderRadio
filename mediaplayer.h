#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QDir>

class MediaPlayer : public QObject
{
	Q_OBJECT
public:
	explicit MediaPlayer(QObject *parent = nullptr);
	~MediaPlayer();

private:
	QMediaPlayer *m_mediaPlayer;
	QMediaPlaylist *m_mediaPlaylist;

	QDir m_currentTagDir;
	bool m_isAudioBook;

	void loadAudioBookSettings();
	void saveAudioBookSettings();

signals:
	void statusChanged(const QString &status);

public slots:
	void reloadMediaDir(const QString &mediaPath);
	void reloadMusicAndPlay(const QString &mediaPath);
	void reloadAudioBookAndPlay(const QString &mediaPath);
	void play();
	void playPause();
	void stop();
	void previous();
	void next();
     void volUp();
     void volDown();

	void onError(QMediaPlayer::Error error);
	void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
	void onStateChanged(QMediaPlayer::State state);
};

#endif // MEDIAPLAYER_H
