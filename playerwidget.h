#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>

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
public slots:
	void setStatusText(const QString &status);

signals:
	void stopPressed();
	void playPressed();
	void previousPressed();
	void nextPressed();

	void newTagEntered(const QString &tag);
	void tagSelected(const QString &tag);

public slots:
	void populateCbTagSelect(const QStringList &tagIds);

private slots:
	void on_cbTagSelect_currentIndexChanged(const QString &arg1);
	void on_cbTagSelect_currentTextChanged(const QString &arg1);
};

#endif // PLAYERWIDGET_H
