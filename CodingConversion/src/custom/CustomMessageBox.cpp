/*!
 * FileName: CustomMessageBox.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2018-10-24
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 自定义消息提示框
 */



#include <QStyle>
#include <QTextEdit>
#include <QApplication>
#include <QString>

#include "custom/CustomMessageBox.h"

struct ButtonInfo
{
	QString text;
	QMessageBox::StandardButton buttflag;
	bool is_accept;
};

static ButtonInfo buttonsList[] = {
	{ QObject::trUtf8("确定"),QMessageBox::Ok,true },
	{ QObject::trUtf8("打开"),QMessageBox::Open,true },
	{ QObject::trUtf8("保存"),QMessageBox::Save,true },
	{ QObject::trUtf8("取消"),QMessageBox::Cancel,false },
	{ QObject::trUtf8("关闭"),QMessageBox::Close,false },
	{ QObject::trUtf8("丢弃"),QMessageBox::Discard,false },
	{ QObject::trUtf8("应用"),QMessageBox::Apply,true },
	{ QObject::trUtf8("重置"),QMessageBox::Reset,true },
	{ QObject::trUtf8("恢复默认"),QMessageBox::RestoreDefaults,true },
	{ QObject::trUtf8("帮助"),QMessageBox::Help,true },
	{ QObject::trUtf8("全部保存"),QMessageBox::SaveAll,true },
	{ QObject::trUtf8("是"),QMessageBox::Yes,true },
	{ QObject::trUtf8("全是"),QMessageBox::YesToAll,true },
	{ QObject::trUtf8("否"),QMessageBox::No,false },
	{ QObject::trUtf8("全否"),QMessageBox::NoToAll,false },
	{ QObject::trUtf8("退出"),QMessageBox::Abort,false },
	{ QObject::trUtf8("重试"),QMessageBox::Retry,true },
	{ QObject::trUtf8("忽略"),QMessageBox::Ignore,false }
};

static int buttonListSize = 18;

CustomMessageBox::CustomMessageBox(QWidget *parent) :CustomDialog(parent)
{
	//创建没有文本和任何按钮的messagebox
	whichClicked = QMessageBox::NoButton;

	createTitleBar(this,true,true,false,false,true,false); 

	setObjectName("CustomMessageBox");

	this->setContentsMargins(1, 1, 1, 1);

	QFile qss(":/qss/CustomMessageBox.qss");
	qss.open(QFile::ReadOnly);
	QString qssStr = qss.readAll();
	qss.close();

	setStyleSheet(qssStr);
	setStyle(QApplication::style());  //这句来更新样式  使当前的
	resize(350, 200); //设置默认大小
}





CustomMessageBox::CustomMessageBox(int icon,
	const QString &title,
	const QString &text,
	QWidget *parent,
	QMessageBox::StandardButtons buttons /*= QMessageBox::NoButton*/,
	QMessageBox::StandardButton defaultButton /*= QMessageBox::NoButton*/) :
	CustomDialog(parent)
{
	//创建一个 指定图标  文字 标题   按钮的messgebox
	whichClicked = QMessageBox::NoButton;

	createTitleBar(this, true, true, false, false, true, false);
	setObjectName("CustomMessageBox");

	titleBar->setTitleText(title);
	setWindowIcon(QIcon(":/images/message.png"));

	QLabel *messageIconLabel = NULL;
	QTextEdit *messageTextEdit = NULL;


	centHLay = new QHBoxLayout;
	bottomHLay = new QHBoxLayout;

	bottomHLay->setContentsMargins(0, 5, 10, 5);
	bottomHLay->setSpacing(10);

	if (icon == QMessageBox::Question)
	{
		messageIconLabel = new QLabel(this);
		messageIconLabel->setObjectName("messageQuestionIcon");
	}
	else if (icon == QMessageBox::Information)
	{
		messageIconLabel = new QLabel(this);
		messageIconLabel->setObjectName("messageInformationIcon");
	}
	else if (icon == QMessageBox::Warning)
	{
		messageIconLabel = new QLabel(this);
		messageIconLabel->setObjectName("messageWarningIcon");
	}
	else if (icon == QMessageBox::Critical)
	{
		messageIconLabel = new QLabel(this);
		messageIconLabel->setObjectName("messageCriticalIcon");
	}
	else if (icon == 5)
	{
		messageIconLabel = new QLabel(this);
		messageIconLabel->setObjectName("messageSuccessIcon");
	}

	if (!text.isEmpty())
	{
		messageTextEdit = new QTextEdit(text, this);
		messageTextEdit->setObjectName("messageTextEdit");
		messageTextEdit->setReadOnly(true);
	}

	centHLay->setContentsMargins(0, 0, 0, 0);
	centHLay->setSpacing(10);
	centHLay->addStretch(1);
	if (messageIconLabel != NULL)
	{
		centHLay->addWidget(messageIconLabel, 2);
	}
	if (messageTextEdit != NULL)
	{
		centHLay->addWidget(messageTextEdit, 5);
	}
	centHLay->addStretch(1);

	bottomHLay->addStretch(1);

	for (int i = 0; i < buttonListSize; i++)
	{
		if (buttons&buttonsList[i].buttflag)
		{
			QPushButton *newButt = new QPushButton(buttonsList[i].text, this);
			newButt->setObjectName("messageButton");
			bottomHLay->addWidget(newButt);
			if (buttonsList[i].buttflag == defaultButton)
			{
				newButt->setFocus();
				newButt->setShortcut(Qt::Key_Enter); //绑定回车
			}

			connect(newButt, SIGNAL(clicked()), this, SLOT(recordClickedButton()));

			if (buttonsList[i].is_accept)
			{
				connect(newButt, SIGNAL(clicked()), this, SLOT(accept()));
			}
			else
			{
				connect(newButt, SIGNAL(clicked()), this, SLOT(reject()));
			}
			
		}
	}

	vAllLay->addStretch(1);
	vAllLay->addLayout(centHLay, 4);
	vAllLay->addStretch(1);
	vAllLay->addLayout(bottomHLay, 2);
	this->setContentsMargins(1, 1, 1, 1);

	QFile qss(":/qss/CustomMessageBox.qss");
	qss.open(QFile::ReadOnly);
	QString qssStr = qss.readAll();
	qss.close();

	setStyleSheet(qssStr);
	setStyle(QApplication::style());  //这句来更新样式  使当前的

	if (parent == NULL)
	{
		resizeMoveToDesktopCenter(350, 200); //设置默认大小
	}
	else
	{
		resize(350, 200); //设置默认大小
	}
}

CustomMessageBox::~CustomMessageBox()
{
	qDebug() << "delete CustomMessageBox";
}

void CustomMessageBox::about(QWidget *parent, const QString &title, const QString &text)
{
	CustomMessageBox aboutBox(QMessageBox::NoIcon, title, text, parent);
	aboutBox.exec();
}

QMessageBox::StandardButton CustomMessageBox::critical(QWidget *parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons /*= QMessageBox::Ok*/, QMessageBox::StandardButton defaultButton /*= QMessageBox::NoButton*/)
{
	CustomMessageBox criticalBox(QMessageBox::Critical, title, text, parent, buttons, defaultButton);
	criticalBox.exec();
	return criticalBox.getButtonClicked();
}

QMessageBox::StandardButton CustomMessageBox::information(QWidget *parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons /*= QMessageBox::Ok*/, QMessageBox::StandardButton defaultButton /*= QMessageBox::NoButton*/)
{
	CustomMessageBox informationBox(QMessageBox::Information, title, text, parent, buttons, defaultButton);
	informationBox.exec();
	return informationBox.getButtonClicked();
}

QMessageBox::StandardButton CustomMessageBox::question(QWidget *parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons /*= QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No)*/, QMessageBox::StandardButton defaultButton /*= QMessageBox::NoButton*/)
{
	CustomMessageBox questionBox(QMessageBox::Question, title, text, parent, buttons, defaultButton);
	questionBox.exec();
	return questionBox.getButtonClicked();
}

QMessageBox::StandardButton CustomMessageBox::warning(QWidget *parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons /*= QMessageBox::Ok*/, QMessageBox::StandardButton defaultButton /*= QMessageBox::NoButton*/)
{
	CustomMessageBox warningBox(QMessageBox::Warning, title, text, parent, buttons, defaultButton);
	warningBox.exec();
	return warningBox.getButtonClicked();
}

QMessageBox::StandardButton CustomMessageBox::success(QWidget *parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons /*= QMessageBox::Ok*/, QMessageBox::StandardButton defaultButton /*= QMessageBox::NoButton*/)
{
	CustomMessageBox successBox(5, title, text, parent, buttons, defaultButton);
	successBox.exec();
	return successBox.getButtonClicked();
}

void CustomMessageBox::recordClickedButton()
{
	QPushButton *clickButt = (QPushButton *)sender();
	for (int i = 0; i < buttonListSize; i++)
	{
		if (clickButt->text().compare(buttonsList[i].text) == 0)
		{
			whichClicked = buttonsList[i].buttflag;
			break;
		}
	}
}
