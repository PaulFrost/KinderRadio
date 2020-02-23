#include "gpiointerface.h"

#if defined (_RASPBERRY_PI_)

#include<wiringPi.h>

#include<QDebug>

#define PLAYPAUSE_BUTTON 2
#define NEXT_BUTTON 3
#define PREVIOUS_BUTTON 4
#define VOL_UP_BUTTON 22
#define VOL_DOWN_BUTTON 26

GPIOInterface::GPIOInterface(QObject *parent) : QObject(parent)
{
	GPIOWorker *gpioWorker = new GPIOWorker;
	gpioWorker->moveToThread(&m_gpioThread);
	connect(&m_gpioThread, &QThread::finished, gpioWorker, &QObject::deleteLater);
	connect(this, &GPIOInterface::operate, gpioWorker, &GPIOWorker::checkButtonStatus);
	connect(gpioWorker, &GPIOWorker::buttonPressed, this, &GPIOInterface::handleButtonStatus);
	m_gpioThread.start();
	emit this->operate();
}

GPIOInterface::~GPIOInterface()
{
	m_gpioThread.quit();
	m_gpioThread.wait();
}

void GPIOInterface::handleButtonStatus(int buttonId)
{

	switch (buttonId) {
	case PLAYPAUSE_BUTTON:
		qDebug() << "playpause";
		emit this->playPauseButtonPressed();
		break;
	case NEXT_BUTTON:
		qDebug() << "next";
		emit this->nextButtonPressed();
		break;
	case PREVIOUS_BUTTON:
		qDebug() << "prev";
		emit this->prevBurronPressed();
		break;
	case VOL_UP_BUTTON:
		qDebug() << "volUp";
		emit this->volUpPressed();
		break;
	case VOL_DOWN_BUTTON:
		qDebug() << "volDown";
		emit this->volDownPressed();
		break;
	}
}

void GPIOWorker::checkButtonStatus()
{
	wiringPiSetup();
	pullUpDnControl(PLAYPAUSE_BUTTON, PUD_UP);
	pullUpDnControl(NEXT_BUTTON, PUD_UP);
	pullUpDnControl(PREVIOUS_BUTTON, PUD_UP);
	pullUpDnControl(VOL_UP_BUTTON, PUD_UP);
	pullUpDnControl(VOL_DOWN_BUTTON, PUD_UP);
	//    switch (buttonId) {

	//    }

	while(1){
		if(digitalRead(PLAYPAUSE_BUTTON) == LOW){
			emit this->buttonPressed(PLAYPAUSE_BUTTON);
		}
		if(digitalRead(NEXT_BUTTON) == LOW){
			emit this->buttonPressed(NEXT_BUTTON);
		}
		if(digitalRead(PREVIOUS_BUTTON) == LOW){
			emit this->buttonPressed(PREVIOUS_BUTTON);
		}
		if(digitalRead(VOL_UP_BUTTON) == LOW){
			emit this->buttonPressed(VOL_UP_BUTTON);
		}
		if(digitalRead(VOL_DOWN_BUTTON) == LOW){
			emit this->buttonPressed(VOL_DOWN_BUTTON);
		}

		msleep(100);
	}
}

#else

void GPIOWorker::checkButtonStatus()
{

}

GPIOInterface::GPIOInterface(QObject *parent)
{
	Q_UNUSED(parent)
}

GPIOInterface::~GPIOInterface()
{

}

void GPIOInterface::handleButtonStatus(int buttonId)
{
	Q_UNUSED(buttonId)
}

#endif
