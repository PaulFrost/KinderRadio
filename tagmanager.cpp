#include "tagmanager.h"
#include "filemanager.h"

#include <QDir>
#include <QDateTime>

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

		m_lastTag = m_Tags[line.mid(DATE_FORMAT.size() + 2)];

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
		if(m_Tags[tagId]->type().testFlag(Tag::Music)){
			emit musicTagSelected(tagId);
		}

		if(m_Tags[tagId]->type().testFlag(Tag::Script)){
			emit scriptTagSelected(tagId);
		}

		if(m_Tags[tagId]->type().testFlag(Tag::Audiobook)){
			emit audioBookTagSelected(tagId);
		}

		return;
	}

	QDir mediaDir = QDir::current();
	mediaDir.cd("media");
	mediaDir.mkdir(tagId);

	m_Tags.insert(tagId, QSharedPointer<Tag>(new Tag(tagId, mediaDir)));
}
