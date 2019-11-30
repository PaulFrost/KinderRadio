#ifndef TAG_H
#define TAG_H

#include <QObject>
#include <QFlag>
#include <QDir>

class Tag : public QObject
{
	Q_OBJECT
public:

	enum TagType{None, Music, Script, Audiobook};

	explicit Tag(const QString &tagId = "", QDir mediaDir = QDir::current());
	~Tag();

	QString id() const;
	void setId(const QString &id);

	QFlags<TagType> type() const;

private:
	QString m_id;
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
