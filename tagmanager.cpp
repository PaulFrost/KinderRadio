#include "tagmanager.h"

#include <QDir>
#include <QDateTime>

const QString DATE_FORMAT = "yyyy-MM-dd-HH-mm-ss";

TagManager::TagManager(QObject *parent) : QObject(parent)
{

}

void TagManager::loadTags()
{
	QDir currentDir = QDir::current();
	QFlags<QDir::Filter> filter = (QDir::Filter::Dirs
						 | QDir::Filter::NoDotDot
						 | QDir::Filter::NoDot);

	QStringList nameFilterList;

	QStringList list = currentDir.entryList(nameFilterList, filter);

	QString tagId;
	foreach(QString dirName, list){
		tagId = dirName.mid(DATE_FORMAT.size() + 1);
		m_TagToFolderTable.insert(tagId, dirName);
	}

	emit tagsLoaded(m_TagToFolderTable.keys());
}

void TagManager::createNewTag(const QString &tagId)
{
	if(m_TagToFolderTable.contains(tagId)){
		/// @todo Rename Folder
		return;
	}

	QString dirName = QDateTime::currentDateTime().toString(DATE_FORMAT);
	dirName.append("-");
	dirName.append(tagId);

	QDir::current().mkdir(dirName);

	m_TagToFolderTable.insert(tagId, dirName);
}
