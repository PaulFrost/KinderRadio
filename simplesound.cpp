#include "simplesound.h"
#include "filemanager.h"
#include <QDebug>

SimpleSound::SimpleSound(QObject *parent) : QObject(parent),
	m_bellSound(FileManager::mediaDirAbsolute() + "/bell.wav", this)
{

}

void SimpleSound::playStartupSound()
{
	qDebug() << FileManager::mediaDirAbsolute() + "/startup.wav";
	QSound::play(FileManager::mediaDirAbsolute() + "/startup.wav");
}

void SimpleSound::playBell()
{
	m_bellSound.play();
}
