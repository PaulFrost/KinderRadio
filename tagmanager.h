#ifndef TAGMANAGER_H
#define TAGMANAGER_H

#include <QObject>
#include <QSharedPointer>
#include "tag.h"

class TagManager : public QObject
{
	Q_OBJECT
public:
	explicit TagManager(QObject *parent = nullptr);
	void loadTags();

	QSharedPointer<Tag> lastTag() const;

private:
	QHash<QString, QSharedPointer<Tag> > m_Tags;
	QSharedPointer<Tag> m_lastTag;

signals:
	void tagsLoaded(const QStringList &tagList);

public slots:
	void selectTag(const QString &tagId);
};

#endif // TAGMANAGER_H
