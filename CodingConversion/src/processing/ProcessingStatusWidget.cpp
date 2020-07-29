/*!
 * FileName: ProcessingStatusWidget.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-12-10
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 处理状态栏
 */

#include <QApplication>
#include <QStyle>

#include "SignalController.h"

#include "processing/ProcessingStatusWidget.h"
#include "setting/GlobalSetting.h"


ProcessingStatusWidget::ProcessingStatusWidget(QWidget *parent /*= NULL*/) :PainterWidget(parent)
{
	hAllLay = new QHBoxLayout(this);

	progressBar = new QProgressBar;
	progressInfoLabel = new QLabel;
	statusInfoLabel = new QLabel;
	statusIconLabel = new QLabel;


	progressInfoLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	progressInfoLabel->hide();

	statusInfoLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	statusInfoLabel->setText(trUtf8("停止"));

	statusIconLabel->setProperty("status", "stoped");

	hAllLay->setContentsMargins(5, 2, 5, 2);
	hAllLay->setSpacing(5);
	hAllLay->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

	hAllLay->addWidget(progressBar, 1);
	hAllLay->addWidget(progressInfoLabel);
	hAllLay->addWidget(statusInfoLabel);
	hAllLay->addWidget(statusIconLabel);

	progressBar->setRange(0, 100);
	progressBar->setValue(50);
	progressBar->setTextVisible(false);
	progressBar->hide();

    setObjectName("ProcessingStatusWidget");
	progressBar->setObjectName("ProgressBar");
	progressInfoLabel->setObjectName("ProgressInfoLabel");
	statusInfoLabel->setObjectName("StatusInfoLabel");
	statusIconLabel->setObjectName("StatusIconLabel");

	initConnect();
}

ProcessingStatusWidget::~ProcessingStatusWidget()
{

}

void ProcessingStatusWidget::initConnect()
{
	ConnectInfo connectInfo[] = {
		{ signalController,SIGNAL(SIG_conversionStarted()),this,SLOT(handleConversionStarted()),Qt::AutoConnection },
		{ signalController,SIGNAL(SIG_conversionStoped()),this,SLOT(handleConversionStoped()),Qt::AutoConnection },
		{ signalController,SIGNAL(SIG_conversionPaused()),this,SLOT(handleConversionPaused()),Qt::AutoConnection },
		{ signalController,SIGNAL(SIG_startConversion()),this,SLOT(handleStartConversion()),Qt::AutoConnection },
		{ signalController,SIGNAL(SIG_stopConversion()),this,SLOT(handleStopConversion()),Qt::AutoConnection },
		{ signalController,SIGNAL(SIG_pauseConversion()),this,SLOT(handlePauseConversion()),Qt::AutoConnection },
	};

	SignalController::setConnectInfo(connectInfo, sizeof(connectInfo) / sizeof(ConnectInfo));
}

void ProcessingStatusWidget::handleStartConversion()
{
	statusInfoLabel->setText(trUtf8("正在启动"));
}

void ProcessingStatusWidget::handleStopConversion()
{
	QVariant hasConversionTask;
	globalSetting->getValue("hasConversionTask", hasConversionTask);
	if (hasConversionTask.toBool())
	{
		statusInfoLabel->setText(trUtf8("正在停止"));
	}
}

void ProcessingStatusWidget::handlePauseConversion()
{
	statusInfoLabel->setText(trUtf8("正在暂停"));
}

void ProcessingStatusWidget::handleConversionStoped()
{
	statusInfoLabel->setText(trUtf8("停止"));
	statusIconLabel->setProperty("status", "stoped");
	statusIconLabel->setStyle(QApplication::style());
	progressBar->setValue(0);
	progressInfoLabel->setText(trUtf8("--/--"));
	progressBar->hide();
	progressInfoLabel->hide();
}

void ProcessingStatusWidget::handleConversionPaused()
{
	statusInfoLabel->setText(trUtf8("暂停"));
	statusIconLabel->setProperty("status", "paused");
	statusIconLabel->setStyle(QApplication::style());
}

void ProcessingStatusWidget::handleConversionStarted()
{
	statusInfoLabel->setText(trUtf8("转换中"));
	statusIconLabel->setProperty("status", "conversion");
	statusIconLabel->setStyle(QApplication::style());
	progressBar->show();
	progressInfoLabel->show();
}
