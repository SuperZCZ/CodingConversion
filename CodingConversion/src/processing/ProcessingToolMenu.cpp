/*!
 * FileName: ProcessingToolMenu.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-12-10
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 处理工具栏
 */
#include <QMetaType>
#include <QListView>

#include "SignalController.h"
#include "custom/CustomMessageBox.h"
#include "processing/ProcessingToolMenu.h"

ProcessingToolMenu::ProcessingToolMenu(QWidget *parent /*= NULL*/) :PainterWidget(parent)
{
	have_conversion = false;

    vAllLay = new QVBoxLayout(this);
    top_HLay = new QHBoxLayout;

	codeLabel = new QLabel(trUtf8("编码:"));
	lineLabel = new QLabel(trUtf8("行尾:"));

    codeTypeCombox = new QComboBox;
    lineEndTypeCombox = new QComboBox;

	codeTypeCombox->setView(new QListView);
	lineEndTypeCombox->setView(new QListView);
	codeTypeCombox->setFixedWidth(100);
	lineEndTypeCombox->setFixedWidth(125);

    startPauseButt = new QPushButton;
    stopButt = new QPushButton;
	modeButt = new QPushButton;

	startPauseButt->setCheckable(true);
	modeButt->setCheckable(true);
	startPauseButt->setToolTip(trUtf8("开始/暂停"));
	stopButt->setToolTip(trUtf8("停止"));
	modeButt->setToolTip(trUtf8("转换/检测"));

    top_HLay->setSpacing(10);
    top_HLay->setAlignment(Qt::AlignCenter);
	top_HLay->setContentsMargins(10, 0, 10, 0);
	top_HLay->addWidget(codeLabel);
    top_HLay->addWidget(codeTypeCombox);
	top_HLay->addSpacing(20);
	top_HLay->addWidget(lineLabel);
    top_HLay->addWidget(lineEndTypeCombox);
    top_HLay->addStretch(1);
    top_HLay->addWidget(startPauseButt);
    top_HLay->addWidget(stopButt);
    top_HLay->addWidget(modeButt);

    vAllLay->setSpacing(0);
    vAllLay->setMargin(0);
    vAllLay->addLayout(top_HLay);

	initCodeTypeList();
	initLineEndTypeList();

    setObjectName("ProcessingToolMenu");
	startPauseButt->setObjectName("startPauseButt");
	stopButt->setObjectName("stopButt");
	modeButt->setObjectName("modeButt");
	
	lineEndTypeCombox->setObjectName("TypeComboBox");
	codeTypeCombox->setObjectName("TypeComboBox");
	initConnect();
}

ProcessingToolMenu::~ProcessingToolMenu()
{

}

void ProcessingToolMenu::initConnect()
{
	ConnectInfo connectInfo[] = {
		{ startPauseButt,SIGNAL(clicked()),this,SLOT(handleStartPauseClicked()),Qt::AutoConnection },
		{ stopButt,SIGNAL(clicked()),this,SLOT(handleStopClicked()),Qt::AutoConnection },
		{ modeButt,SIGNAL(clicked()),this,SLOT(handleModeSwitchClicked()),Qt::AutoConnection },
		{ this,SIGNAL(SIG_startConversion()),signalController,SIGNAL(SIG_startConversion()),Qt::AutoConnection },
		{ this,SIGNAL(SIG_stopConversion()),signalController,SIGNAL(SIG_stopConversion()),Qt::AutoConnection },
		{ this,SIGNAL(SIG_pauseConversion()),signalController,SIGNAL(SIG_pauseConversion()),Qt::AutoConnection },
		{ signalController,SIGNAL(SIG_conversionStarted()),this,SLOT(handleConversionStart()),Qt::AutoConnection },
		{ signalController,SIGNAL(SIG_conversionStoped()),this,SLOT(handleConversionStop()),Qt::AutoConnection },
		{ signalController,SIGNAL(SIG_conversionPaused()),this,SLOT(handleConversionPause()),Qt::AutoConnection },
	};

	SignalController::setConnectInfo(connectInfo, sizeof(connectInfo) / sizeof(ConnectInfo));
}

void ProcessingToolMenu::initCodeTypeList()
{
	CodecType codec_array[] =
	{
		{trUtf8("UTF-8"),chardet::TextCodecInfo(trUtf8("UTF-8"))},
		{trUtf8("UTF-8-BOM"),chardet::TextCodecInfo(trUtf8("UTF-8"),true,trUtf8(""),QByteArray("\xEF\xBB\xBF",3))},
		{trUtf8("GB18030"),chardet::TextCodecInfo(trUtf8("GB18030"))},
	};

	int array_size = sizeof(codec_array) / sizeof(CodecType);

	for (int i = 0; i < array_size; i++)
	{
		codeTypeCombox->addItem(codec_array[i].displayName);
		codeTypeCombox->setItemData(i, QVariant::fromValue<CodecType>(codec_array[i]));
	}
}

void ProcessingToolMenu::initLineEndTypeList()
{
	LineEndType line_end_array[] = 
	{
		{trUtf8("--"),0},
		{trUtf8("Windows (CR LF)"),1},
		{trUtf8("Unix (LF)"),2},
		{trUtf8("Macintosh (CR)"),3}
	};

	int array_size = sizeof(line_end_array) / sizeof(LineEndType);

	for (int i = 0; i < array_size; i++)
	{
		lineEndTypeCombox->addItem(line_end_array[i].displayName);
		lineEndTypeCombox->setItemData(i, line_end_array[i].endType);
	}
}


void ProcessingToolMenu::handleStartPauseClicked()
{
	startPauseButt->setDisabled(true);
	if (startPauseButt->isChecked())
	{
		emit SIG_startConversion();
	}
	else
	{
		emit SIG_pauseConversion();
	}
}

void ProcessingToolMenu::handleStopClicked()
{
	if (have_conversion)
	{
		stopButt->setDisabled(true);
		startPauseButt->setDisabled(true);

		QString info_str = trUtf8("转换");
		if (modeButt->isChecked())
		{
			info_str = trUtf8("检测");
		}

		QMessageBox::StandardButton res_butt = CustomMessageBox::question(this, trUtf8("终止任务"), trUtf8("确认终止当前%1任务？").arg(info_str), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		if (res_butt == QMessageBox::Yes)
		{
			emit SIG_stopConversion();
		}
	}
	emit SIG_stopConversion();
}

void ProcessingToolMenu::handleModeSwitchClicked()
{
	if (have_conversion)
	{
		CustomMessageBox::warning(this, trUtf8("警告"), trUtf8("请先终止当前任务后再切换模式!"), QMessageBox::Yes, QMessageBox::Yes);
	}
	else
	{
		modeButt->setChecked(!modeButt->isChecked());
	}
}

void ProcessingToolMenu::handleConversionStart()
{
	startPauseButt->setEnabled(true);
	startPauseButt->setChecked(true);
}

void ProcessingToolMenu::handleConversionPause()
{
	startPauseButt->setEnabled(true);
	startPauseButt->setChecked(false);
}

void ProcessingToolMenu::handleConversionStop()
{
	startPauseButt->setEnabled(true);
	startPauseButt->setChecked(false);
	stopButt->setEnabled(true);
}
