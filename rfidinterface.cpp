#include "rfidinterface.h"

#if defined (_RASPBERRY_PI_)

#include "MFRC522.h"

#include <unistd.h>
#include <QDebug>

RFIDInterface::RFIDInterface(QObject *parent) : QObject(parent)
{
    NFCWorker *nfcWorker = new NFCWorker;
    nfcWorker->moveToThread(&m_nfcThread);
    connect(&m_nfcThread, &QThread::finished, nfcWorker, &QObject::deleteLater);
    connect(this, &RFIDInterface::operate, nfcWorker, &NFCWorker::searchForTags);
    connect(nfcWorker, &NFCWorker::tagFound, this, &RFIDInterface::handleTag);
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
    emit this->tagRecognized(tagId);
}

void NFCWorker::searchForTags() {
    QString result;


    MFRC522 mfrc;

    mfrc.PCD_Init();

    while(1){
        // Look for a card
        if(!mfrc.PICC_IsNewCardPresent())
            continue;

        if( !mfrc.PICC_ReadCardSerial())
            continue;

        result.clear();

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
