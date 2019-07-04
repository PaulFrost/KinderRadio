#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>
#include <QComboBox>

namespace Ui {



class PlayerWidget;
}

class TagSelectorCB : public QComboBox
{
	Q_OBJECT
public:
	TagSelectorCB(QWidget *parent = nullptr);
protected:
	void keyPressEvent(QKeyEvent *e);
	void focusOutEvent(QFocusEvent *);
signals:
	void lostFocus();
};

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

signals:
	void stopPressed();
	void playPressed();
	void previousPressed();
	void nextPressed();

	void newTagEntered(const QString &tag);
	void tagSelected(const QString &tag);

public slots:
	void populateCbTagSelect(const QStringList &tagIds);
	void checkForNewTag();

private slots:
	void on_cbTagSelect_currentIndexChanged(const QString &arg1);
	void on_cbTagSelect_currentTextChanged(const QString &arg1);
};

#endif // PLAYERWIDGET_H
