#include "rfidinterface.h"

#if defined(_RASPBERRY_PI_)

#include "MFRC522.h"

#include <unistd.h>
#include <QDebug>

RFIDInterface::RFIDInterface(QObject *parent) : QObject(parent)
{
	NFCWorker *nfcWorker = new NFCWorker;
	nfcWorker->moveToThread(&m_nfcThread);
	connect(&m_nfcThread, &QThread::finished, nfcWorker, &QObject::deleteLater);
	connect(this, &RFIDInterface::operate, nfcWorker, &NFCWorker::searchForTags);
	connect(nfcWorker, &NFCWorker::tagFound, this, &RFIDInterface::handleTag, Qt::DirectConnection);
	m_nfcThread.start();
	emit this->operate();
}

RFIDInterface::~RFIDInterface()
{
	m_nfcThread.quit();
	m_nfcThread.wait();
}

void RFIDInterface::handleTag(const QString &tagId)
{
	qDebug()<< Q_FUNC_INFO << tagId;
	emit this->tagRecognized(tagId);
}

void NFCWorker::searchForTags() {
	QString result;

	MFRC522 mfrc;

	mfrc.PCD_Init();

	while(1){

//		sleep(1);
//		qDebug() << "Waking up...";
//		byte bufferATQA[2];
//		byte bufferSize = sizeof(bufferATQA);
//		int tmp= mfrc.PICC_WakeupA(bufferATQA, &bufferSize);
//		QString debugStr("wakeup returned ");
//		if (tmp==MFRC522::STATUS_OK) {
//			debugStr.append("OK");
//		}
//		else {
//			debugStr.append("Error ").append(QString::number(tmp));
//		}
//		qDebug()<< debugStr;

//		continue;

		// Look for a card

		if(!mfrc.PICC_IsNewCardPresent())
			continue;

//		qDebug()<< "PICC_IsNewCardPresent";

		if( !mfrc.PICC_ReadCardSerial())
			continue;

		result.clear();
		qDebug()<< "found new Tag...";

		// Print UID
		for(byte i = 0; i < mfrc.uid.size; ++i){
			if(mfrc.uid.uidByte[i] < 0x10){
				result.append(" 0");
				result.append(QString().sprintf("%X",mfrc.uid.uidByte[i]));
			}
			else{
				result.append(" ");
				result.append(QString().sprintf("%X",mfrc.uid.uidByte[i]));
			}
		}
		emit tagFound(result.simplified().replace(" ", "-"));
		sleep(1);
	}
}

#else


RFIDInterface::RFIDInterface(QObject *parent)
{
	Q_UNUSED(parent)
}

RFIDInterface::~RFIDInterface()
{

}

void RFIDInterface::handleTag(const QString &tagId)
{
	Q_UNUSED(tagId)
}

void NFCWorker::searchForTags()
{

}


#endif
