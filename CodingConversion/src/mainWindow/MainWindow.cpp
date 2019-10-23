/*!
 * FileName: MainWindow.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-9-9
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 主窗体widget
 */
#include <QApplication>
#include <QDesktopWidget>
#include "mainWindow/MainWindow.h"
#include "custom/CustomTooltipsMessage.h"

const static int LEFT_WIDGET_STRETCH = 327;
const static int RIGHT_WIDGET_STRETCH = 740;

MainTopTitleWidget::MainTopTitleWidget(QWidget* relative_widget, QWidget* parent /*= NULL*/) :PainterWidget(parent)
{
	hAllLay = new QHBoxLayout(this);

	titleLabelWidget = new CustomWidget(this, true);
	titleBarWidget = new CustomWidget(this, true);
	titleSettingWidget = new QWidget;

	setting_HLay = new QHBoxLayout(titleSettingWidget);
	settingButt = new QPushButton;
	settingMenu = new QMenu(this);

	settingButt->setMenu(settingMenu);

	settingMenu->addAction(trUtf8("关于"));

	titleLabelWidget->createTitleBar(relative_widget, true, true, false, false, false, true);
	titleBarWidget->createTitleBar(relative_widget, false, false, true, true, true, true);
	titleLabelWidget->widgetTitleBar()->setTitleText(trUtf8("Coding Conversion"));
	titleLabelWidget->widgetTitleBar()->setEnableDoubleClickMax(true);
	
	setting_HLay->setMargin(0);
	setting_HLay->setSpacing(0);
	setting_HLay->addWidget(settingButt);

	hAllLay->setMargin(0);
	hAllLay->setSpacing(0);
	hAllLay->addWidget(titleLabelWidget);
	hAllLay->addWidget(titleSettingWidget);
	hAllLay->addWidget(titleBarWidget);

	titleLabelWidget->setObjectName("titleLabelWidget");
	titleSettingWidget->setObjectName("titleSettingWidget");
	titleBarWidget->setObjectName("titleBarWidget");
	settingButt->setObjectName("settingButt");
	
}

MainWindow::MainWindow(QWidget* parent /*= NULL*/) :CustomWidget(parent, true)
{
	QFile qss(":/qss/MainWidget.qss");
	qss.open(QFile::ReadOnly);
	QString qssStr = qss.readAll();
	qss.close();
	setStyleSheet(qssStr);

	topTitleWidget = new MainTopTitleWidget(this);

	mainWinodwSplitter = new QSplitter(Qt::Horizontal);

	candidateWidget = new CandidateWidget(this);
	processingWidget = new ProcessingWidget(this);

	mainWinodwSplitter->addWidget(candidateWidget);
	mainWinodwSplitter->addWidget(processingWidget);


	vAllLay->setMenuBar(topTitleWidget);
	vAllLay->addWidget(mainWinodwSplitter);

	setWindowIcon(QIcon(":/images/logo.png"));

	setContentsMargins(1, 0, 1, 1);

	setObjectName("MainWindow");
	topTitleWidget->setObjectName("topTitleWidget");
	mainWinodwSplitter->setObjectName("mainWinodwSplitter");

	initConnect();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initConnect()
{
	ConnectInfo connectInfo[] = {
		signalController,SIGNAL(SIG_popupTooltipsMessage(QString, QString,ToolTipsType)),this,SLOT(popupToolTipsMessage(QString, QString,ToolTipsType)),Qt::AutoConnection,
	};

	SignalController::setConnectInfo(connectInfo, sizeof(connectInfo) / sizeof(ConnectInfo));
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
	QList<int> splitter_sizes;
	splitter_sizes << LEFT_WIDGET_STRETCH << RIGHT_WIDGET_STRETCH;

	mainWinodwSplitter->setSizes(splitter_sizes);
	CustomWidget::resizeEvent(event);
}

void MainWindow::popupToolTipsMessage(QString text, QString title, ToolTipsType msg_type)
{
	QPoint show_pos = QApplication::desktop()->screenGeometry().center();
	if (this->isVisible())
	{
		//如果当前主窗体可见则显示在主窗体中间
		show_pos =  mapToGlobal(this->rect().center());
	}

	switch (msg_type)
	{
	case INFORMATION_TOOLTIPS:
		CustomTooltipsMessage::information(text, title, show_pos);
		break;
	case CRITICAL_TOOLTIPS:
		CustomTooltipsMessage::critical(text, title, show_pos);
		break;
	case SUCCESS_TOOLTIPS:
		CustomTooltipsMessage::success(text, title, show_pos);
		break;
	case WARNING_TOOLTIPS:
		CustomTooltipsMessage::warning(text, title, show_pos);
		break;
	default:
		qDebug() << "ToolTips " << text << "  " << title << msg_type;
		break;
	}
}

