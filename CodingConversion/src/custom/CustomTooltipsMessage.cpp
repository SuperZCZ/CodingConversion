/*!
 * FileName: CustomTooltipsMessage.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-6-24
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 气泡提示窗体
 */
#include <QApplication>
#include <QDesktopWidget>
#include <QStyleOption>
#include <QFile>
#include <QPainter>
#include <QDebug>
#include <QMessageBox>

#include "custom/CustomTooltipsMessage.h"


void CustomTooltipsMessage::critical(const QString &text, const QString &title, const QPoint &pos)
{
	CustomTooltipsMessage *msgTooltips = new CustomTooltipsMessage(QMessageBox::Critical, title, text, NULL);
	msgTooltips->show();
	if (pos.isNull())
	{
		msgTooltips->moveToDesktopCenter();
	}
	else
	{
		msgTooltips->move(pos - msgTooltips->rect().center());  //移动向量使窗体中央位于指定的pos
	}
}


void CustomTooltipsMessage::information(const QString &text, const QString &title, const QPoint &pos)
{
	CustomTooltipsMessage *msgTooltips = new CustomTooltipsMessage(QMessageBox::Information, title, text, NULL);
	msgTooltips->show();
	if (pos.isNull())
	{
		msgTooltips->moveToDesktopCenter();
	}
	else
	{
		msgTooltips->move(pos - msgTooltips->rect().center());  //移动向量使窗体中央位于指定的pos
	}
}


void CustomTooltipsMessage::warning(const QString &text, const QString &title, const QPoint &pos)
{
	CustomTooltipsMessage *msgTooltips = new CustomTooltipsMessage(QMessageBox::Warning, title, text, NULL);
	msgTooltips->show();
	if (pos.isNull())
	{
		msgTooltips->moveToDesktopCenter();
	}
	else
	{
		msgTooltips->move(pos - msgTooltips->rect().center());  //移动向量使窗体中央位于指定的pos
	}
}


void CustomTooltipsMessage::success(const QString &text, const QString &title, const QPoint &pos)
{
	CustomTooltipsMessage *msgTooltips = new CustomTooltipsMessage(5, title, text, NULL);
	msgTooltips->show();
	if (pos.isNull())
	{
		msgTooltips->moveToDesktopCenter();
	}
	else
	{
		msgTooltips->move(pos - msgTooltips->rect().center());  //移动向量使窗体中央位于指定的pos
	}
}

CustomTooltipsMessage::CustomTooltipsMessage(int messageType, const QString &title, const QString &text, QWidget *parent /*= NULL*/) :QWidget(parent)
{
	tooltipsOpacity = 1.0;

	Qt::WindowFlags wFlags = this->windowFlags();  //获取窗口类型  
	wFlags = wFlags & Qt::WindowType_Mask;               //提取窗口类型
	wFlags = wFlags | Qt::CustomizeWindowHint;         //隐藏标题栏
	wFlags = wFlags | Qt::FramelessWindowHint;         //隐藏系统提供的边框
	wFlags = wFlags | Qt::Tool; //隐藏任务栏图标
	wFlags = wFlags | Qt::WindowStaysOnTopHint; //顶置
	setWindowFlags(wFlags);
	setAttribute(Qt::WA_TranslucentBackground);  //设置背景透明
	setAttribute(Qt::WA_DeleteOnClose);

	
	hAllLay = new QHBoxLayout(this);
	iconVLay = new QVBoxLayout;
	textVLay = new QVBoxLayout;
	titleHLay = new QHBoxLayout;
	msgTextHLay = new QHBoxLayout;
	propertyAnimation = new QPropertyAnimation(this, "tooltipsOpacity", this);

	iconLabel = new QLabel;
	titleLabel = new QLabel(title);
	msgTextLabel = new QLabel(text);
	titleLabel->adjustSize();
	msgTextLabel->adjustSize();

	iconVLay->setSpacing(0);
	iconVLay->setMargin(0);
	iconVLay->setAlignment(Qt::AlignCenter);
	iconVLay->addWidget(iconLabel);

	titleHLay->setSpacing(0);
	titleHLay->setMargin(0);
	titleHLay->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	titleHLay->addWidget(titleLabel);

	msgTextHLay->setSpacing(0);
	msgTextHLay->setMargin(0);
	msgTextHLay->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	msgTextHLay->addWidget(msgTextLabel);

	textVLay->setMargin(0);
	textVLay->setSpacing(5);
	textVLay->addLayout(titleHLay);
	textVLay->addLayout(msgTextHLay);

	hAllLay->setSpacing(5);
	hAllLay->setContentsMargins(10, 10, 10, 10);
	hAllLay->addLayout(iconVLay);
	hAllLay->addLayout(textVLay, 1);

	if (title.isEmpty())
	{
		titleLabel->hide();
	}

	this->setMinimumSize(270, 65); //最小大小

	if (messageType == QMessageBox::Critical)
	{
		iconLabel->setObjectName("messageCriticalIcon");
	}
	else if (messageType == QMessageBox::Information)
	{
		iconLabel->setObjectName("messageInformationIcon");
	}
	else if (messageType == QMessageBox::Warning)
	{
		iconLabel->setObjectName("messageWarningIcon");
	}
	else if (messageType == 5)
	{
		iconLabel->setObjectName("messageSuccessIcon");
	}

	setObjectName("CustomTooltipsMessage");
	titleLabel->setObjectName("TooltipsTitle");
	msgTextLabel->setObjectName("TooltipsText");

	QFile qss(":/qss/CustomTooltipsMessage.qss");
	qss.open(QFile::ReadOnly);
	QString qssStr = qss.readAll();
	qss.close();
	setStyleSheet(qssStr);

	clock_timer.start(2700);
	propertyAnimation->setDuration(2650);
	propertyAnimation->setStartValue(1.0);
	propertyAnimation->setEndValue(0.1);
	propertyAnimation->setEasingCurve(QEasingCurve::InCubic);
	propertyAnimation->start();

	connect(&clock_timer, SIGNAL(timeout()), this, SLOT(closeTooltips()));
}


CustomTooltipsMessage::~CustomTooltipsMessage()
{
	//qDebug() << "delete CustomTooltipsMessage";
}

void CustomTooltipsMessage::paintEvent(QPaintEvent *event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(event);
}

void CustomTooltipsMessage::closeTooltips()
{
	this->close();
}


void CustomTooltipsMessage::moveToDesktopCenter()
{
	QRect rect = QApplication::desktop()->screenGeometry();   //获取默认屏幕的宽高 

	move(rect.center() - this->rect().center());
}

double CustomTooltipsMessage::getTooltipsOpacity()
{
	return tooltipsOpacity;
}

void CustomTooltipsMessage::setTooltipsOpacity(double opactiy)
{
	tooltipsOpacity = opactiy;
	setWindowOpacity(tooltipsOpacity);
}

void CustomTooltipsMessage::mouseReleaseEvent(QMouseEvent *event)
{
	clock_timer.start();
	propertyAnimation->start();
	QWidget::mouseReleaseEvent(event);
}

void CustomTooltipsMessage::mousePressEvent(QMouseEvent *event)
{
	tooltipsOpacity = 1.0;
	setWindowOpacity(tooltipsOpacity);
	clock_timer.stop();
	propertyAnimation->stop();
	QWidget::mousePressEvent(event);
}

void CustomTooltipsMessage::mouseDoubleClickEvent(QMouseEvent *event)
{
	close();
	QWidget::mouseDoubleClickEvent(event);
}
