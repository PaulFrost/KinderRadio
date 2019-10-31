#ifndef TAGSETTINGS_H
#define TAGSETTINGS_H

#include <QObject>
#include <QDir>

class TagSettings : public QObject
{
	Q_OBJECT
public:
	enum TagType{Music, WifiOn, WifiOff, Shuffle, Restart, Shutdown};

	explicit TagSettings(QObject *parent = nullptr);

	void createDefaultSettings(const QDir directory);
	QFlags<TagType> loadSettings(const QDir directory);

signals:

public slots:
};

#endif // TAGSETTINGS_H
