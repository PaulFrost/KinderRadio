#ifndef TAG_H
#define TAG_H

#include <QObject>
#include <QFlag>
#include <QDir>
#include "filemanager.h"

class Tag : public QObject
{
	Q_OBJECT
public:

	enum TagType{None = 0x0,
				 Music = 0x1,
				 Script = 0x2,
				 Audiobook = 0x4};

	explicit Tag(const QString &tagId = "", const QString &name = "", QDir mediaDir = FileManager::mediaDir());
	~Tag();

	QString id() const;
	void setId(const QString &id);

	QFlags<TagType> type() const;

	QString name() const;
	void setName(const QString &name);
	QString directoryName() const;

private:
	QString m_id;
	QString m_name;
	QFlags<TagType> m_type;
	QDir m_tagDir;

	void createDefaultSettings(const QDir directory);
	void loadSettings(const QDir directory);

signals:
	void typeChanged(TagType type);

public slots:
	void loadSettings();
};

#endif // TAG_H
