#ifndef TAG_H
#define TAG_H

#include <QObject>
#include "tagsettings.h"

class Tag : public QObject
{
	Q_OBJECT
public:
	explicit Tag(const QString &tagId = "", QObject *parent = nullptr);
	~Tag();

	QString id() const;
	void setId(const QString &id);

	TagSettings::TagType type() const;
	void setType(const TagSettings::TagType &type);

private:
	QString m_id;
	TagSettings::TagType m_type;

signals:
	void typeChanged(TagSettings::TagType type);

public slots:
};

#endif // TAG_H
