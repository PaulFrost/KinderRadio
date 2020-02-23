#include "scriptmanager.h"
#include "filemanager.h"

#include <QDir>
#include <QDebug>
#include <QProcess>
#include <QScopedPointer>

ScriptManager::ScriptManager(QObject *parent) : QObject(parent)
{
	QDir mediaDir = FileManager::mediaDir();

	foreach(const QString &tagName, FileManager::registeredTags()){
		if(mediaDir.cd(tagName)){
			if(mediaDir.exists("script.sh")){
				QString scriptFilePath = mediaDir.filePath("script.sh");
				m_scriptList.insert(tagName,scriptFilePath);

				qDebug()<< "Script file found: " << scriptFilePath;
			}
			mediaDir.cdUp();
		}
	}
}

void ScriptManager::loadScript(const QString &tagId)
{
	QString scriptFilePath = m_scriptList.value(tagId, "");

	if(!scriptFilePath.isEmpty()){
		qDebug() << "Starting process of tag " << tagId;
		QScopedPointer<QProcess> process(new QProcess(this));

		process->start("sh", QStringList(scriptFilePath), QIODevice::ReadOnly);
		process->waitForFinished(10000);
		qDebug() << process->readAll();
	}

}
