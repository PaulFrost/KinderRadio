#include "simplesound.h"
#include "filemanager.h"
#include <QDebug>

SimpleSound::SimpleSound(QObject *parent) : QObject(parent),
	m_bellSound(FileManager::mediaDirAbsolute() + "/bell.wav", this),
	m_newTagSound(FileManager::mediaDirAbsolute() + "/newtag.wav", this)
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

void SimpleSound::playNewTagEntered()
{
	m_newTagSound.play();
}
