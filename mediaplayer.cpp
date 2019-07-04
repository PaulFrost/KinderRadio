#include "mediaplayer.h"

#include <QDebug>
#include <QDir>
#include <QMediaContent>

MediaPlayer::MediaPlayer(QObject *parent) : QObject(parent),
	m_mediaPlayer(new QMediaPlayer(this)),
	m_mediaPlaylist(new QMediaPlaylist(this))
{
	connect(m_mediaPlayer, &QMediaPlayer::mediaStatusChanged,
			this, &MediaPlayer::onMediaStatusChanged);
	connect(m_mediaPlayer, SIGNAL(error(QMediaPlayer::Error)),
			this, SLOT(onError(QMediaPlayer::Error)));


	m_mediaPlayer->setPlaylist(m_mediaPlaylist);
	m_mediaPlayer->setVolume(100);
}

void MediaPlayer::reloadMedia(const QString &mediaPath)
{
	if(mediaPath.isEmpty()){
		return;
	}

	m_mediaPlaylist->clear();

	QString pwd = QDir::currentPath();


	QDir dir(QDir::currentPath());
	dir.cd("media");
	if(!dir.cd(mediaPath)){
		return;
	}

	qDebug() << "Loading files from" << dir.path();
	dir.setFilter(QDir::Files);

	QFileInfoList list = dir.entryInfoList();
	int mediaCount = 0;
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);

		m_mediaPlaylist->addMedia(QUrl::fromLocalFile(fileInfo.filePath()));
		if(m_mediaPlaylist->mediaCount() > mediaCount){
			qDebug() << QString("'%1' loaded").arg(fileInfo.fileName());
			mediaCount++;
		}
	}

	m_mediaPlaylist->shuffle();
	m_mediaPlaylist->setCurrentIndex(0);
}

void MediaPlayer::reloadMediaAndPlay(const QString &mediaPath)
{
	reloadMedia(mediaPath);
	play();
}

void MediaPlayer::play()
{
	qDebug() << Q_FUNC_INFO;
	m_mediaPlayer->play();
}

void MediaPlayer::stop()
{
	qDebug() << Q_FUNC_INFO;
	m_mediaPlayer->stop();
}

void MediaPlayer::previous()
{
	m_mediaPlaylist->previous();
}

void MediaPlayer::next()
{
	m_mediaPlaylist->next();
}

void MediaPlayer::onError(QMediaPlayer::Error error)
{
	emit statusChanged("Error: " + m_mediaPlayer->errorString());
	qWarning() << error;
}

void MediaPlayer::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
	qDebug() << status;

	switch (status) {
	case QMediaPlayer::MediaStatus::InvalidMedia:
		emit statusChanged("Invalid Media!");
		break;
	case QMediaPlayer::MediaStatus::LoadingMedia:
		emit statusChanged("Loading Media...");
		break;
	case QMediaPlayer::MediaStatus::NoMedia:
		if(m_mediaPlaylist->mediaCount() > 0){
			play();
		}
		else {
			emit statusChanged("No Media available!");
		}
		break;
	case QMediaPlayer::MediaStatus::BufferedMedia:
	{
		QString fileName = m_mediaPlaylist->currentMedia().canonicalUrl().fileName();
		emit statusChanged(QString("Playing: %1").arg(fileName));
	}
		break;
	default:
		break;
	}
	if(status==QMediaPlayer::MediaStatus::LoadedMedia){
		emit statusChanged("Ready to play");
	}

}
