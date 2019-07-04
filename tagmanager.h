#ifndef TAGMANAGER_H
#define TAGMANAGER_H

#include <QObject>
#include <QHash>

class TagManager : public QObject
{
	Q_OBJECT
public:
	explicit TagManager(QObject *parent = nullptr);
	void loadTags();

	QString lastTag() const;

private:
	QStringList m_Tags;
	QString m_lastTag;

signals:
	void tagsLoaded(const QStringList &tagList);

public slots:
	void selectTag(const QString &tagId);
};

#endif // TAGMANAGER_H
