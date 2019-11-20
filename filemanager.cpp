#include "filemanager.h"

QDir FileManager::mediaDir()
{
	QDir mediaDir = QDir::current();
	mediaDir.cd("media");
	return mediaDir;
}

QDir FileManager::tagDir(const QString &tagId)
{
	QDir mediaDir = FileManager::mediaDir();
	mediaDir.cd(tagId);
	return mediaDir;
}

QStringList FileManager::registeredTags()
{
	QFlags<QDir::Filter> filter = (QDir::Filter::Dirs
						 | QDir::Filter::NoDotDot
						 | QDir::Filter::NoDot);

	return FileManager::mediaDir().entryList(QStringList(), filter);
}

FileManager::FileManager()
{

}
