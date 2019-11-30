#include "tagmanager.h"
#include "filemanager.h"

#include <QDir>
#include <QDateTime>
#include <QDebug>
#include <QScopedPointer>
#include <QProcess>

const QString LOG_FILE = "./media/tags.log";
const QString DATE_FORMAT = "yyyy-MM-dd-HH-mm-ss";

TagManager::TagManager(QObject *parent) : QObject(parent)
{

}

void TagManager::loadTags()
{
	QStringList tagNameList = FileManager::registeredTags();

	foreach (const QString tagName, tagNameList) {
		QSharedPointer<Tag> tag(new Tag(tagName, FileManager::mediaDir()));
		m_Tags.insert(tagName, tag);
	}

	qDebug() << "loadTags";

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

	emit tagsLoaded(static_cast<QStringList>(m_Tags.keys()));
}

QSharedPointer<Tag> TagManager::lastTag() const
{
	return m_lastTag;
}

void TagManager::selectTag(const QString &tagId)
{
	QString logString = QDateTime::currentDateTime().toString(DATE_FORMAT);
	logString.append(": ");
	logString.append(tagId);
	logString.append("\n");

	QFile logFile("./media/tags.log");
	logFile.open(QIODevice::Append | QIODevice::WriteOnly);
	logFile.write(logString.toLocal8Bit());


	if(m_Tags.contains(tagId)){
		m_Tags[tagId]->loadSettings();
		if(m_Tags[tagId]->type().testFlag(Tag::Music)){
			qDebug()<< "Music Tag";
			emit musicTagSelected(tagId);
		}
		if(m_Tags[tagId]->type().testFlag(Tag::Script)){
			qDebug()<< "Script Tag";
			emit scriptTagSelected(tagId);
		}
		if(m_Tags[tagId]->type().testFlag(Tag::Audiobook)){
			qDebug()<< "Audiobook Tag";
			emit audioBookTagSelected(tagId);
		}
		return;
	}

	QDir mediaDir = QDir::current();
	mediaDir.cd("media");
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

	QSharedPointer<Tag> tag(new Tag(tagId, FileManager::mediaDir()));
	m_Tags.insert(tagId, tag);

	emit tagsLoaded(static_cast<QStringList>(m_Tags.keys()), tagId);
}
