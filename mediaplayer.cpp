#include "mediaplayer.h"
#include "filemanager.h"

#include <QDebug>
#include <QAudioDeviceInfo>
#include <QAudioOutputSelectorControl>
#include <QMediaService>
#include <QMediaContent>
#include <QFile>
#include <QRegularExpression>


static const char* AUDIOBOOK_SETTINGS_FILE = ".audiobook";

static QString FILE_PATTERN("^(mp3|m4a)$");
static QRegularExpression FILE_RE(FILE_PATTERN);

const int INITIAL_VOLUME = 5;
const int MAX_VOLUME = 20;

MediaPlayer::MediaPlayer(QObject *parent) : QObject(parent),
	m_mediaPlayer(new QMediaPlayer(this)),
	m_mediaPlaylist(new QMediaPlaylist(this)),
	m_isAudioBook(false)
{
	connect(m_mediaPlayer, &QMediaPlayer::mediaStatusChanged,
			this, &MediaPlayer::onMediaStatusChanged);
	connect(m_mediaPlayer, &QMediaPlayer::stateChanged,
			this, &MediaPlayer::onStateChanged);
	connect(m_mediaPlayer, SIGNAL(error(QMediaPlayer::Error)),
			this, SLOT(onError(QMediaPlayer::Error)));

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    QList<QAudioDeviceInfo> devices =  QAudioDeviceInfo::availableDevices(QAudio::Mode::AudioOutput);

	m_mediaPlayer->setPlaylist(m_mediaPlaylist);
    m_mediaPlayer->setVolume(INITIAL_VOLUME);
}

MediaPlayer::~MediaPlayer()
{
	saveAudioBookSettings();
}

void MediaPlayer::loadAudioBookSettings()
{
	if(m_isAudioBook){
		QFile settingsFile(m_currentTagDir.filePath(AUDIOBOOK_SETTINGS_FILE));
		if(settingsFile.open(QIODevice::ReadOnly)){
			bool ok;
			QString indexLine(settingsFile.readLine());
			int index = indexLine.toInt(&ok);
			if(ok){
				m_mediaPlaylist->setCurrentIndex(index);
				QString posLine(settingsFile.readLine());
				qint64 pos = posLine.toLongLong(&ok);
				if(ok){
					m_mediaPlayer->setPosition(pos);
					qDebug() << QString("Continuing audiobook at index: %1 and pos: %2 (%3)").arg(index).arg(pos).arg(m_mediaPlayer->position());
				}
				else {
					qDebug() << QString("Continuing audiobook at index: %1").arg(index);
				}
			}
			settingsFile.close();
		}
	}
}

void MediaPlayer::saveAudioBookSettings()
{
	if(m_isAudioBook){
		QFile settingsFile(m_currentTagDir.filePath(AUDIOBOOK_SETTINGS_FILE));
		if(settingsFile.open(QIODevice::WriteOnly)){
			settingsFile.write(QString("%1\n%2").arg(m_mediaPlaylist->currentIndex()).arg(m_mediaPlayer->position()).toUtf8());
			settingsFile.flush();
			settingsFile.close();
		}
	}
}

bool MediaPlayer::reloadMediaDir(const QString &mediaPath)
{
	if(mediaPath.isEmpty()){
		return false;
	}

	m_mediaPlaylist->clear();


	QDir dir(FileManager::mediaDirAbsolute());

	if(!dir.cd(mediaPath)){
		return false;
	}
	m_currentTagDir = dir;
	qDebug() << mediaPath;
	qDebug() << "Loading files from" << dir.path();
	dir.setFilter(QDir::Files | QDir::NoDot | QDir::NoDotDot);

	QFileInfoList list = dir.entryInfoList();
	int mediaCount = 0;
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);

		QRegularExpressionMatch reMatch = FILE_RE.match(fileInfo.suffix());
		if(reMatch.hasMatch()){
			qDebug() << fileInfo.filePath();
			m_mediaPlaylist->addMedia(QUrl::fromLocalFile(fileInfo.filePath()));
			if(m_mediaPlaylist->mediaCount() > mediaCount){
				qDebug() << QString("'%1' loaded").arg(fileInfo.fileName());
				mediaCount++;
			}
		}
	}

	if(!mediaCount){
		return false;
	}

	return true;
}

void MediaPlayer::reloadMusicAndPlay(const QString &mediaPath)
{
	if(m_isAudioBook){
		saveAudioBookSettings();
	}
	stop();
	m_isAudioBook = false;
	if(!reloadMediaDir(mediaPath)){
		return;
	}
	m_mediaPlaylist->shuffle();
	m_mediaPlaylist->setCurrentIndex(0);
	play();
}

void MediaPlayer::reloadAudioBookAndPlay(const QString &mediaPath)
{
	if(m_isAudioBook){
		saveAudioBookSettings();
	}
	m_isAudioBook = true;
	reloadMediaDir(mediaPath);
	loadAudioBookSettings();
	play();
}

void MediaPlayer::playPause()
{
	if(m_mediaPlayer->state() == QMediaPlayer::PlayingState){
		m_mediaPlayer->pause();
	}
	else{
		m_mediaPlayer->play();
	}
}

void MediaPlayer::play()
{
	m_mediaPlayer->play();
}

void MediaPlayer::stop()
{
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

void MediaPlayer::volUp()
{
	int incr = m_mediaPlayer->volume() > 9 ? 2 : 1;
	int vol = qMin(m_mediaPlayer->volume() + incr, MAX_VOLUME);
    m_mediaPlayer->setVolume(vol);
}

void MediaPlayer::volDown()
{
	int incr = m_mediaPlayer->volume() > 9 ? 2 : 1;
	int vol = qMax(m_mediaPlayer->volume() - incr, 0);
    m_mediaPlayer->setVolume(vol);
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
		emit statusChanged(QString("%1: %2")
						   .arg(m_mediaPlayer->state()==QMediaPlayer::State::PlayingState ? "Playing" : "Paused")
						   .arg(m_mediaPlaylist->currentMedia().canonicalUrl().fileName()));
		break;
	default:
		break;
	}
	if(status==QMediaPlayer::MediaStatus::LoadedMedia){
		emit statusChanged("Ready to play");
	}

}

void MediaPlayer::onStateChanged(QMediaPlayer::State state)
{
	switch (state) {
	case QMediaPlayer::State::PlayingState:{
		QString fileName = m_mediaPlaylist->currentMedia().canonicalUrl().fileName();
		emit statusChanged(QString("Playing: %1").arg(fileName));
		break;
	}
	case QMediaPlayer::State::PausedState:{
		QString fileName = m_mediaPlaylist->currentMedia().canonicalUrl().fileName();
		emit statusChanged(QString("Paused: %1").arg(fileName));
		break;
	}
	case QMediaPlayer::State::StoppedState:{
		emit statusChanged(QString("Stopped"));
	}
	}
}
