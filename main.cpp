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
#include "simplesound.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	PlayerWidget pw;
	TagManager tagManager;
	QObject::connect(&tagManager, &TagManager::tagsLoaded, &pw, &PlayerWidget::populateCbTagSelect);
	tagManager.loadTags();

	MediaPlayer mediaPlayer;
	if(tagManager.lastTag()){
		mediaPlayer.reloadMediaDir(tagManager.lastTag()->directoryName());
	}

	RFIDInterface rfidInterface;
	GPIOInterface gpioInterface;
	ScriptManager scriptManager;
	SimpleSound simpleSound;

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
	QObject::connect(&rfidInterface, &RFIDInterface::tagRecognized, &tagManager, &TagManager::selectTag, Qt::DirectConnection);
	QObject::connect(&rfidInterface, &RFIDInterface::tagRecognized, &pw, &PlayerWidget::setCurrentTagSilently);
	QObject::connect(&gpioInterface, &GPIOInterface::pauseButtonPressed,&mediaPlayer, &MediaPlayer::stop);
	QObject::connect(&gpioInterface, &GPIOInterface::playPauseButtonPressed,&mediaPlayer, &MediaPlayer::playPause);
	QObject::connect(&gpioInterface, &GPIOInterface::prevBurronPressed,&mediaPlayer, &MediaPlayer::previous);
	QObject::connect(&gpioInterface, &GPIOInterface::nextButtonPressed,&mediaPlayer, &MediaPlayer::next);
	QObject::connect(&gpioInterface, &GPIOInterface::volUpPressed, &mediaPlayer, &MediaPlayer::volUp);
	QObject::connect(&gpioInterface, &GPIOInterface::volDownPressed, &mediaPlayer, &MediaPlayer::volDown);

	if(tagManager.lastTag()){
		pw.setCurrentTag(tagManager.lastTag()->id());
	}
	pw.show();

	mediaPlayer.play(); //Autoplay

	SimpleSound::playStartupSound();
	QObject::connect(&tagManager, &TagManager::audioBookTagSelected, &simpleSound, &SimpleSound::playBell);
	QObject::connect(&tagManager, &TagManager::musicTagSelected, &simpleSound, &SimpleSound::playBell);
	QObject::connect(&tagManager, &TagManager::scriptTagSelected, &simpleSound, &SimpleSound::playBell);
	QObject::connect(&tagManager, &TagManager::newTagAdded, &simpleSound, &SimpleSound::playNewTagEntered);

	return a.exec();
}
