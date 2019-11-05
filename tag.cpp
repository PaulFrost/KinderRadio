#include "tag.h"

const QString DEFAULT_SETTINGS = "#shuffle\n#wifion #or wifioff\n#restart\n#shutdown";

Tag::Tag(const QString &tagId, QDir mediaDir) : QObject(),
	m_id(tagId)
{
	mediaDir.cd(tagId);
	loadSettings(mediaDir);
}

Tag::~Tag()
{

}

QString Tag::id() const
{
	return m_id;
}

void Tag::setId(const QString &id)
{
	m_id = id;
}

QFlags<Tag::TagType> Tag::type() const
{
	return m_type;
}

void Tag::createDefaultSettings(const QDir directory)
{
	QFile file(directory.filePath(".kradio"));
	if(file.open(QIODevice::WriteOnly)){
		file.write(DEFAULT_SETTINGS.toLocal8Bit());
		file.flush();
		file.close();
	}
}

void Tag::loadSettings(const QDir directory)
{
	QFile file(directory.filePath(".kradio"));
	if(file.open(QIODevice::ReadOnly)){
		QString allLines = file.readAll();
		QStringList lines = allLines.split("\n");

		foreach(QString line, lines){
			line = line.simplified();
			if(line.at(0) == '#'){
				continue;
			}

			if(line.startsWith("shuffle")){
				m_type = TagType::Shuffle;
			}
			if(line.startsWith("wifion")){
				m_type = m_type | TagType::WifiOn;
			}
			if(line.startsWith("wifioff") && !m_type.testFlag(TagType::WifiOn)){
				m_type = m_type | TagType::WifiOff;
			}
			if(line.startsWith("restart")){
				m_type = TagType::Restart;
			}
			if(line.startsWith("shutdown")){
				m_type = TagType::Shutdown;
			}
		}
	}
	else {
		createDefaultSettings(directory);
	}
}
