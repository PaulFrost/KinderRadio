#ifndef GPIOINTERFACE_H
#define GPIOINTERFACE_H

#include <QObject>

class GPIOInterface : public QObject
{
	Q_OBJECT
public:
	explicit GPIOInterface(QObject *parent = nullptr);

signals:
	void nextPressed();

public slots:
};

#endif // GPIOINTERFACE_H
