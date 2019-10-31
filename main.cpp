#include <QApplication>
#include "mediaplayer.h"
#include "playerwidget.h"
#include "tagmanager.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	PlayerWidget pw;

	TagManager tagManager;
	QObject::connect(&tagManager, &TagManager::tagsLoaded, &pw, &PlayerWidget::populateCbTagSelect);
	tagManager.loadTags();

	MediaPlayer mediaPlayer;

	if(tagManager.lastTag()){
		mediaPlayer.reloadMedia(tagManager.lastTag()->id());
	}


	QObject::connect(&pw, &PlayerWidget::stopPressed,&mediaPlayer, &MediaPlayer::stop);
	QObject::connect(&pw, &PlayerWidget::playPressed,&mediaPlayer, &MediaPlayer::play);
	QObject::connect(&pw, &PlayerWidget::previousPressed,&mediaPlayer, &MediaPlayer::previous);
	QObject::connect(&pw, &PlayerWidget::nextPressed,&mediaPlayer, &MediaPlayer::next);
	QObject::connect(&pw, &PlayerWidget::newTagEntered, &tagManager, &TagManager::selectTag);
	QObject::connect(&pw, &PlayerWidget::tagSelected, &mediaPlayer, &MediaPlayer::reloadMediaAndPlay);//@TBD: Go via TagManager
	QObject::connect(&pw, &PlayerWidget::newTagEntered, &mediaPlayer, &MediaPlayer::reloadMedia);
	QObject::connect(&mediaPlayer, &MediaPlayer::statusChanged, &pw, &PlayerWidget::setStatusText);

	if(tagManager.lastTag()){
		pw.setCurrentTag(tagManager.lastTag()->id());
	}
	pw.show();

	return a.exec();
}
