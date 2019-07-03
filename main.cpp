#include <QApplication>
#include "mediaplayer.h"
#include "playerwidget.h"
#include "tagmanager.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MediaPlayer mediaPlayer;
	mediaPlayer.reloadMedia();

	PlayerWidget pw;

	TagManager tagManager;
	tagManager.loadTags();

	QObject::connect(&pw, &PlayerWidget::stopPressed,&mediaPlayer, &MediaPlayer::stop);
	QObject::connect(&pw, &PlayerWidget::playPressed,&mediaPlayer, &MediaPlayer::play);
	QObject::connect(&pw, &PlayerWidget::previousPressed,&mediaPlayer, &MediaPlayer::previous);
	QObject::connect(&pw, &PlayerWidget::nextPressed,&mediaPlayer, &MediaPlayer::next);

	QObject::connect(&mediaPlayer, &MediaPlayer::statusChanged, &pw, &PlayerWidget::setStatusText);

	QObject::connect(&tagManager, &TagManager::tagsLoaded, &pw, &PlayerWidget::populateCbTagSelect);
	QObject::connect(&pw, &PlayerWidget::newTagEntered, &tagManager, &TagManager::createNewTag);
	pw.show();

	return a.exec();
}
