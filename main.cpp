#include <QApplication>

#if !defined (_RASPBERRY_PI_)
#pragma message("No GPIO-Support! Build this Project on a Raspberry Pi.")
#endif

#include "mediaplayer.h"
#include "playerwidget.h"
#include "tagmanager.h"
#include "rfidinterface.h"
#include "gpiointerface.h"
#include "scriptmanager.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	PlayerWidget pw;
	TagManager tagManager;
	QObject::connect(&tagManager, &TagManager::tagsLoaded, &pw, &PlayerWidget::populateCbTagSelect);
	tagManager.loadTags();

	MediaPlayer mediaPlayer;
	if(tagManager.lastTag()){
		mediaPlayer.reloadMediaDir(tagManager.lastTag()->id());
	}

	RFIDInterface rfidInterface;
	GPIOInterface gpioInterface;

	ScriptManager scriptManager;
	QObject::connect(&tagManager, &TagManager::scriptTagSelected, &scriptManager, &ScriptManager::loadScript);
	QObject::connect(&pw, &PlayerWidget::playPausePressed,&mediaPlayer, &MediaPlayer::playPause);
	QObject::connect(&pw, &PlayerWidget::previousPressed,&mediaPlayer, &MediaPlayer::previous);
	QObject::connect(&pw, &PlayerWidget::nextPressed,&mediaPlayer, &MediaPlayer::next);
	QObject::connect(&pw, &PlayerWidget::newTagEntered, &tagManager, &TagManager::selectTag);
	QObject::connect(&pw, &PlayerWidget::tagSelected, &tagManager, &TagManager::selectTag);
	QObject::connect(&pw, &PlayerWidget::newTagEntered, &mediaPlayer, &MediaPlayer::reloadMediaDir);
	QObject::connect(&pw, &PlayerWidget::volUpPressed, &mediaPlayer, &MediaPlayer::volUp);
	QObject::connect(&pw, &PlayerWidget::volDownPressed, &mediaPlayer, &MediaPlayer::volDown);
	QObject::connect(&mediaPlayer, &MediaPlayer::statusChanged, &pw, &PlayerWidget::setStatusText);
	QObject::connect(&tagManager, &TagManager::musicTagSelected, &mediaPlayer, &MediaPlayer::reloadMusicAndPlay);
	QObject::connect(&tagManager, &TagManager::audioBookTagSelected, &mediaPlayer, &MediaPlayer::reloadAudioBookAndPlay);
	QObject::connect(&rfidInterface, &RFIDInterface::tagRecognized, &tagManager, &TagManager::selectTag);
	QObject::connect(&gpioInterface, &GPIOInterface::pauseButtonPressed,&mediaPlayer, &MediaPlayer::stop);
	QObject::connect(&gpioInterface, &GPIOInterface::playButtonPressed,&mediaPlayer, &MediaPlayer::play);
	QObject::connect(&gpioInterface, &GPIOInterface::prevBurronPressed,&mediaPlayer, &MediaPlayer::previous);
	QObject::connect(&gpioInterface, &GPIOInterface::nextButtonPressed,&mediaPlayer, &MediaPlayer::next);

	if(tagManager.lastTag()){
		pw.setCurrentTag(tagManager.lastTag()->id());
	}
	pw.show();

	return a.exec();
}
