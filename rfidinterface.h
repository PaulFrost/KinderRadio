#ifndef RFIDINTERFACE_H
#define RFIDINTERFACE_H

#include <QObject>

class RFIDInterface : public QObject
{
	Q_OBJECT
public:
	explicit RFIDInterface(QObject *parent = nullptr);

signals:

public slots:
};

#endif // RFIDINTERFACE_H
