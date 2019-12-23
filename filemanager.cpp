#include "filemanager.h"

const QString MEDIA_PATH = "kinderradio-media";

QDir FileManager::mediaDir()
{
	QDir mediaDir = QDir::homePath();
	mediaDir.cd(MEDIA_PATH);
	return mediaDir;
}

QString FileManager::mediaDirAbsolute()
{
	return mediaDir().absolutePath();
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
