#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <QObject>
#include <QHash>


class ScriptManager : public QObject
{
	Q_OBJECT
public:
	explicit ScriptManager(QObject *parent = nullptr);

private:
	QHash<QString, QString> m_scriptList;

signals:

public slots:
	void loadScript(const QString &tagId);
};

#endif // SCRIPTMANAGER_H
