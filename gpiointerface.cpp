#include "gpiointerface.h"

#if defined (_RASPBERRY_PI_)

#include<wiringPi.h>

#include<QDebug>

#define RED_BUTTON 7
#define BLUE_BUTTON 0
#define GREEN_BUTTON 2
#define BLACK_BUTTON 3

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
	case RED_BUTTON:
		qDebug() << "pause";
		emit this->pauseButtonPressed();
		break;
	case BLUE_BUTTON:
		qDebug() << "next";
		emit this->nextButtonPressed();
		break;
	case GREEN_BUTTON:
		qDebug() << "play";
		emit this->playButtonPressed();
		break;
	case BLACK_BUTTON:
		qDebug() << "prev";
		emit this->prevBurronPressed();
		break;
	}
}

void GPIOWorker::checkButtonStatus()
{
	wiringPiSetup();
	pullUpDnControl(RED_BUTTON, PUD_UP);
	pullUpDnControl(BLUE_BUTTON, PUD_UP);
	pullUpDnControl(GREEN_BUTTON, PUD_UP);
	pullUpDnControl(BLACK_BUTTON, PUD_UP);
	//    switch (buttonId) {

	//    }

	while(1){
		if(digitalRead(RED_BUTTON) == LOW){
			emit this->buttonPressed(RED_BUTTON);
		}
		if(digitalRead(BLUE_BUTTON) == LOW){
			emit this->buttonPressed(BLUE_BUTTON);
		}
		if(digitalRead(GREEN_BUTTON) == LOW){
			emit this->buttonPressed(GREEN_BUTTON);
		}
		if(digitalRead(BLACK_BUTTON) == LOW){
			emit this->buttonPressed(BLACK_BUTTON);
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
