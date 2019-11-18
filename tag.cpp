#include "tag.h"

const QString DEFAULT_SETTINGS = "#Uncomment the following lines to \n\n#load a script.sh file \n#script\n\nor to create an audiobook\n#audiobook";

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

			if(line.count() < 1 || line.at(0) == '#'){
				continue;
			}

			if(line.startsWith("script")){
				m_type = m_type | TagType::Script;
			}
			if(line.startsWith("audiobook")){
				m_type = m_type | TagType::Audiobook;
			}
		}
	}
	else {
		createDefaultSettings(directory);
	}
}
