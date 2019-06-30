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
};

#endif // PLAYERWIDGET_H
