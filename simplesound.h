#ifndef SIMPLESOUND_H
#define SIMPLESOUND_H

#include <QObject>
#include <QSound>

class SimpleSound : public QObject
{
	Q_OBJECT
public:
	explicit SimpleSound(QObject *parent = nullptr);

	static void playStartupSound();

private:
	QSound m_bellSound;
	QSound m_newTagSound;

signals:

public slots:
	void playBell();
	void playNewTagEntered();
};

#endif // SIMPLESOUND_H
