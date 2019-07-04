#include "playerwidget.h"
#include "ui_playerwidget.h"

#include <QKeyEvent>

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

	connect(ui->cbTagSelect, &TagSelectorCB::lostFocus,
			this, &PlayerWidget::checkForNewTag);
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

void PlayerWidget::populateCbTagSelect(const QStringList &tagIds)
{
	ui->cbTagSelect->clear();
	ui->cbTagSelect->addItems(tagIds);
}

void PlayerWidget::checkForNewTag()
{
	if(m_temporaryTagName.size() > 0){
		emit newTagEntered(m_temporaryTagName);
		m_temporaryTagName.clear();
	}
}


void PlayerWidget::on_cbTagSelect_currentIndexChanged(const QString &arg1)
{
	emit tagSelected(arg1);
}

void PlayerWidget::on_cbTagSelect_currentTextChanged(const QString &arg1)
{
	m_temporaryTagName = arg1;
}

TagSelectorCB::TagSelectorCB(QWidget *parent):QComboBox (parent)
{}

void TagSelectorCB::keyPressEvent(QKeyEvent *e)
{
	QComboBox::keyPressEvent(e);
	if(e->key() == Qt::Key::Key_Enter){
		clearFocus();
	}
}

void TagSelectorCB::focusOutEvent(QFocusEvent *)
{
	emit lostFocus();
}
