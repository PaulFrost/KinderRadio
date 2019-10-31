#include "tagsettings.h"

const QString DEFAULT_SETTINGS = "#shuffle\n#wifion #or wifioff\n#restart\n#shutdown";

TagSettings::TagSettings(QObject *parent) : QObject(parent)
{

}

void TagSettings::createDefaultSettings(const QDir directory)
{
	QFile file(directory.filePath(".kradio"));
	if(file.open(QIODevice::WriteOnly)){
		file.write(DEFAULT_SETTINGS.toLocal8Bit());
		file.flush();
		file.close();
	}
}

QFlags<TagSettings::TagType> TagSettings::loadSettings(const QDir directory)
{
	QFlags<TagSettings::TagType> resFlags;

	QFile file(directory.filePath(".kradio"));
	if(file.open(QIODevice::ReadOnly)){

		QString allLines = file.readAll();
		QStringList lines = allLines.split("\n");

		foreach(QString line, lines){
			line = line.simplified();
			if(line.at(0) == '#'){
				continue;
			}

			if(line == "shuffle"){
				resFlags = TagType::Shuffle;
			}
			if(line == "wifion"){
				resFlags = resFlags | TagType::WifiOn;
			}
			if(line == "wifioff" && !resFlags.testFlag(TagType::WifiOn)){
				resFlags = resFlags | TagType::WifiOff;
			}
			if(line == "restart"){
				resFlags = TagType::Restart;
			}
			if(line == "shutdown"){
				resFlags = TagType::Shutdown;
			}
		}
	}
	return resFlags;
}
