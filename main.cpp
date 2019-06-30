#include <QApplication>
#include "mediaplayer.h"
#include "playerwidget.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MediaPlayer mediaPlayer;
	mediaPlayer.reloadMedia();

	PlayerWidget pw;
	QObject::connect(&pw, &PlayerWidget::stopPressed,&mediaPlayer, &MediaPlayer::stop);
	QObject::connect(&pw, &PlayerWidget::playPressed,&mediaPlayer, &MediaPlayer::play);
	QObject::connect(&pw, &PlayerWidget::previousPressed,&mediaPlayer, &MediaPlayer::previous);
	QObject::connect(&pw, &PlayerWidget::nextPressed,&mediaPlayer, &MediaPlayer::next);

	QObject::connect(&mediaPlayer, &MediaPlayer::statusChanged, &pw, &PlayerWidget::setStatusText);

	pw.show();

	return a.exec();
}
