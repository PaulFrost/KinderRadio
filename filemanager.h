#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QStringList>
#include <QDir>

class FileManager
{
public:
	static QDir mediaDir();
	static QStringList registeredTags();

private:
	FileManager();
};

#endif // FILEMANAGER_H
