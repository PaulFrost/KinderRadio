#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>
#include <QComboBox>

namespace Ui {



class PlayerWidget;
}

class PlayerWidget : public QWidget
{
	Q_OBJECT

public:
	explicit PlayerWidget(QWidget *parent = nullptr);
	~PlayerWidget();

private:
	Ui::PlayerWidget *ui;
	QString m_temporaryTagName;

public slots:
	void setStatusText(const QString &status);
	void setCurrentTag(const QString &tag);
	void populateCbTagSelect(const QStringList &tagIds, const QString &lastTag = QString());
	void checkForNewTag();

private slots:
	void showAddNewTagDilog();

signals:
	void playPausePressed();
	void previousPressed();
	void nextPressed();
	void volUpPressed();
	void volDownPressed();

	void newTagEntered(const QString &tag);
	void tagSelected(const QString &tag);	
};

#endif // PLAYERWIDGET_H
