#include "playerwidget.h"
#include "ui_playerwidget.h"

#include <QKeyEvent>
#include <QInputDialog>
#include <QDebug>

PlayerWidget::PlayerWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PlayerWidget)
{
	ui->setupUi(this);

	connect(ui->pbPlayPause, &QPushButton::clicked,
			this, &PlayerWidget::playPausePressed);
	connect(ui->pbPrevious, &QPushButton::clicked,
			this, &PlayerWidget::previousPressed);
	connect(ui->pbNext, &QPushButton::clicked,
			this, &PlayerWidget::nextPressed);
	connect(ui->pbAddTag, &QPushButton::clicked,
			this, &PlayerWidget::showAddNewTagDilog);

	connect(ui->cbTagSelect, &QComboBox::currentTextChanged,
			this, &PlayerWidget::tagSelected);
}

PlayerWidget::~PlayerWidget()
{
	delete ui;
}

void PlayerWidget::setStatusText(const QString &status)
{
	ui->leStatus->setText(status);
}

void PlayerWidget::setCurrentTag(const QString &tag)
{
	ui->cbTagSelect->setCurrentText(tag);
}

void PlayerWidget::populateCbTagSelect(const QStringList &tagIds, const QString &lastTag)
{
	ui->cbTagSelect->clear();
	ui->cbTagSelect->addItems(tagIds);

	if(!lastTag.isEmpty()){
		ui->cbTagSelect->setCurrentText(lastTag);
	}
}

void PlayerWidget::checkForNewTag()
{
	if(m_temporaryTagName.size() > 0){
		emit newTagEntered(m_temporaryTagName);
		m_temporaryTagName.clear();
	}
}

void PlayerWidget::showAddNewTagDilog()
{
	bool ok = false;
	QString tagName = QInputDialog::getText(Q_NULLPTR, "Add tag", "Enter new tag id", QLineEdit::Normal, QString(), &ok);
	if(ok){
		emit this->newTagEntered(tagName);
	}
}
