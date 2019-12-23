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
	void populateCbTagSelect(const QStringList &tagIds, const QStringList &tagNames, const QString &lastTag = QString());
	void addNewTag(const QString &tagId);
	void checkForNewTag();
	void cbTagIndexChanged(int index);
	void setCurrentTagSilently(const QString &tag);

private slots:
	void showAddNewTagDilog();

signals:
	void playPausePressed();
	void previousPressed();
	void nextPressed();
	void volUpPressed();
	void volDownPressed();

	void newTagEntered(const QString &tag);
	void tagSelected(const QString &tagIdName);

protected:
	void closeEvent(QCloseEvent *event) override;
};

#endif // PLAYERWIDGET_H
