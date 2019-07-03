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

private:
	QHash<QString,QString> m_TagToFolderTable;

signals:
	void tagsLoaded(const QStringList &tagList);

public slots:
	void createNewTag(const QString &tagId);
};

#endif // TAGMANAGER_H
