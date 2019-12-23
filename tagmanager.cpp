#include "tagmanager.h"
#include "filemanager.h"

#include <QDir>
#include <QDateTime>
#include <QDebug>
#include <QSharedPointer>
#include <QScopedPointer>
#include <QProcess>

const QString LOG_FILE = FileManager::mediaDirAbsolute() + "/tags.log";
const QString DATE_FORMAT = "yyyy-MM-dd-HH-mm-ss";

TagManager::TagManager(QObject *parent) : QObject(parent)
{

}

void TagManager::loadTags()
{
	QStringList tagIdNameList = FileManager::registeredTags();

	QStringList tagIdList;
	QStringList tagNameList;

	foreach (const QString tagIdName, tagIdNameList) {
		QStringList idNameList = tagIdName.split("--");
		QString id = idNameList[0];
		QString name = "";



		if(idNameList.count() > 1){
			name = idNameList[1];
			qDebug()<<id<<name;
		}
		tagIdList << id;
		tagNameList << name;
		QSharedPointer<Tag> tag(new Tag(id, name, FileManager::mediaDir()));

		m_Tags.insert(id, tag);
	}
	QString tagId;

	QFile file(LOG_FILE);
	if(file.open(QIODevice::ReadOnly)){
		QString fileContent = file.readAll();
		QStringList lines = fileContent.split("\n");

		QString line;
		int i = lines.count()-1;
		while(i >= 0){
			line = lines[i];
			if(line.size() > DATE_FORMAT.size()+2){
				break;
			}
			i--;
		}
		tagId = line.mid(DATE_FORMAT.size() + 2);

		if(m_Tags.contains(tagId)){
			QSharedPointer<Tag> tag = m_Tags[tagId];
			if(tag->type().testFlag(Tag::TagType::Music) || tag->type().testFlag(Tag::TagType::Audiobook)){
				m_lastTag = tag;
			}
		}
	}

	emit tagsLoaded(tagIdList, tagNameList);
}

QSharedPointer<Tag> TagManager::lastTag() const
{
	return m_lastTag;
}

void TagManager::selectTag(const QString &tagId)
{
	qDebug()<< Q_FUNC_INFO << tagId << m_Tags.keys();

	QString logString = QDateTime::currentDateTime().toString(DATE_FORMAT);
	logString.append(": ");
	logString.append(tagId);
	logString.append("\n");

	QFile logFile(LOG_FILE);
	logFile.open(QIODevice::Append | QIODevice::WriteOnly);
	logFile.write(logString.toLocal8Bit());


	if(m_Tags.contains(tagId)){
		qDebug()<< "m_Tags.contains " << tagId;
		QSharedPointer<Tag> tag = m_Tags[tagId];
		tag->loadSettings();
		if(tag->type().testFlag(Tag::Music)){
			qDebug()<< "Music Tag" << tagId;
			emit musicTagSelected(tag->directoryName());
		}
		if(tag->type().testFlag(Tag::Script)){
			qDebug()<< "Script Tag" << tagId;
			emit scriptTagSelected(tag->directoryName());
		}
		if(tag->type().testFlag(Tag::Audiobook)){
			qDebug()<< "Audiobook Tag" << tagId;
			emit audioBookTagSelected(tag->directoryName());
		}
		return;
	}

	QDir mediaDir = FileManager::mediaDir();
	mediaDir.mkdir(tagId);

#if defined (_RASPBERRY_PI_)
	QStringList chmodArgs;
	chmodArgs << "-R";
	chmodArgs << "0777";
	chmodArgs << mediaDir.absolutePath();

	QScopedPointer<QProcess> process(new QProcess);
	process->setProgram("chmod");
	process->setArguments(chmodArgs);
	process->start(QIODevice::ReadOnly);
	process->waitForFinished(10000);
#endif

	QSharedPointer<Tag> tag(new Tag(tagId, "", FileManager::mediaDir()));
	m_Tags.insert(tagId, tag);

	emit newTagAdded(tagId);
}
