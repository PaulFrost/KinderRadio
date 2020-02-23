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
	void newTagAdded(const QString &tagId);
	void tagsLoaded(const QStringList &tagIds, const QStringList &tagNames, const QString &lastTag = QString());

	void musicTagSelected(const QString &tagIdName);
	void scriptTagSelected(const QString &tagIdName);
	void audioBookTagSelected(const QString &tagIdName);

public slots:
	void selectTag(const QString &tagId);
};

#endif // TAGMANAGER_H
