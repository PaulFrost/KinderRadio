#include "playerwidget.h"
#include "ui_playerwidget.h"

PlayerWidget::PlayerWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PlayerWidget)
{
	ui->setupUi(this);

	connect(ui->pbStop, &QPushButton::clicked,
			this, &PlayerWidget::stopPressed);
	connect(ui->pbPlay, &QPushButton::clicked,
			this, &PlayerWidget::playPressed);
	connect(ui->pbPrevious, &QPushButton::clicked,
			this, &PlayerWidget::previousPressed);
	connect(ui->pbNext, &QPushButton::clicked,
			this, &PlayerWidget::nextPressed);
}

PlayerWidget::~PlayerWidget()
{
	delete ui;
}

void PlayerWidget::setStatusText(const QString &status)
{
	ui->leStatus->setText(status);
}

void PlayerWidget::populateCbTagSelect(const QStringList &tagIds)
{
	ui->cbTagSelect->clear();
	ui->cbTagSelect->addItems(tagIds);
}


void PlayerWidget::on_cbTagSelect_currentIndexChanged(const QString &arg1)
{
	emit tagSelected(arg1);
}

void PlayerWidget::on_cbTagSelect_currentTextChanged(const QString &arg1)
{
	/// @todo Create Folder after enter is being pressed
	emit newTagEntered(arg1);
}
