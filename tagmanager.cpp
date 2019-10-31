#include "tagmanager.h"

#include <QDir>
#include <QDateTime>

const QString LOG_FILE = "./media/tags.log";
const QString DATE_FORMAT = "yyyy-MM-dd-HH-mm-ss";

TagManager::TagManager(QObject *parent) : QObject(parent)
{

}

void TagManager::loadTags()
{
	QDir currentDir = QDir::current();
	currentDir.cd("media");
	QFlags<QDir::Filter> filter = (QDir::Filter::Dirs
						 | QDir::Filter::NoDotDot
						 | QDir::Filter::NoDot);

	QStringList tagNameList(currentDir.entryList(QStringList(), filter));


	foreach (const QString tagName, tagNameList) {
		QSharedPointer<Tag> tag(new Tag(tagName));
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
		return;
	}

	QDir currentPath = QDir::current();
	currentPath.cd("media");
	currentPath.mkdir(tagId);

	m_Tags.insert(tagId, QSharedPointer<Tag>(new Tag(tagId)));
}
