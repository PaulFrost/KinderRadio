#include "simplesound.h"
#include <QDebug>

SimpleSound::SimpleSound(QObject *parent) : QObject(parent),
	m_bellSound("./media/bell.wav", this)
{

}

void SimpleSound::playStartupSound()
{
	QSound::play("./media/startup.wav");
}

void SimpleSound::playBell()
{
	qDebug() << Q_FUNC_INFO;
	m_bellSound.play();
}
