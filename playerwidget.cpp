#include "playerwidget.h"
#include "ui_playerwidget.h"

#include <QKeyEvent>
#include <QInputDialog>
#include <QDebug>
#include <QVariant>
#include <QApplication>

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
	connect(ui->pbVolUp, &QPushButton::clicked,
			this, &PlayerWidget::volUpPressed);
	connect(ui->pbVolDown, &QPushButton::clicked,
			this, &PlayerWidget::volDownPressed);

	connect(ui->cbTagSelect, SIGNAL(currentIndexChanged(int)),
			this, SLOT(cbTagIndexChanged(int)));
}

PlayerWidget::~PlayerWidget()
{
	delete ui;
	qApp->exit();
}

void PlayerWidget::setStatusText(const QString &status)
{
	ui->leStatus->setText(status);
}

void PlayerWidget::setCurrentTag(const QString &tag)
{
	ui->cbTagSelect->setCurrentIndex(ui->cbTagSelect->findData(tag));
}

void PlayerWidget::setCurrentTagSilently(const QString &tag)
{
	disconnect(ui->cbTagSelect, SIGNAL(currentIndexChanged(int)),
			   this, SLOT(cbTagIndexChanged(int)));
	ui->cbTagSelect->setCurrentIndex(ui->cbTagSelect->findData(tag));
	connect(ui->cbTagSelect, SIGNAL(currentIndexChanged(int)),
			this, SLOT(cbTagIndexChanged(int)));
}

void PlayerWidget::populateCbTagSelect(const QStringList &tagIds, const QStringList &tagNames, const QString &lastTag)
{
	ui->cbTagSelect->clear();

	for(int i = 0; i < tagNames.count(); ++i){
		if(tagNames[i].isEmpty()){
			ui->cbTagSelect->addItem(tagIds[i], QVariant::fromValue(tagIds[i]));
		}
		else{
			ui->cbTagSelect->addItem(tagNames[i], QVariant::fromValue(tagIds[i]));
		}
	}

	if(!lastTag.isEmpty()){
		ui->cbTagSelect->setCurrentIndex(ui->cbTagSelect->findData(lastTag));
	}
}

void PlayerWidget::addNewTag(const QString &tagId)
{
	ui->cbTagSelect->addItem(tagId, QVariant::fromValue(tagId));
	ui->cbTagSelect->setCurrentText(tagId);
}

void PlayerWidget::checkForNewTag()
{
	if(m_temporaryTagName.size() > 0){
		emit newTagEntered(m_temporaryTagName);
		m_temporaryTagName.clear();
	}
}

void PlayerWidget::cbTagIndexChanged(int index)
{
	emit tagSelected(ui->cbTagSelect->itemData(index).toString());
}

void PlayerWidget::showAddNewTagDilog()
{
	bool ok = false;
	QString tagName = QInputDialog::getText(Q_NULLPTR, "Add tag", "Enter new tag id", QLineEdit::Normal, QString(), &ok);
	if(ok){
		emit this->newTagEntered(tagName);
	}
}
