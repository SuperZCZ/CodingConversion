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
#include "mainWindow/MainWindow.h"

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
	topTitleWidget = new MainTopTitleWidget(this);

	mainWinodwSplitter = new QSplitter(Qt::Horizontal);

	candidateWidget = new CandidateWidget;
	processingWidget = new ProcessingWidget;


	mainWinodwSplitter->addWidget(candidateWidget);
	mainWinodwSplitter->addWidget(processingWidget);

	vAllLay->setMenuBar(topTitleWidget);
	vAllLay->addWidget(mainWinodwSplitter);

	setWindowIcon(QIcon(":/images/logo.png"));

	setContentsMargins(1, 0, 1, 1);

	setObjectName("MainWindow");
	topTitleWidget->setObjectName("topTitleWidget");
	mainWinodwSplitter->setObjectName("mainWinodwSplitter");

	QFile qss(":/qss/MainWidget.qss");
	qss.open(QFile::ReadOnly);
	QString qssStr = qss.readAll();
	qss.close();
	setStyleSheet(qssStr);
}

MainWindow::~MainWindow()
{

}

void MainWindow::resizeEvent(QResizeEvent* event)
{
	QList<int> splitter_sizes;
	splitter_sizes << LEFT_WIDGET_STRETCH << RIGHT_WIDGET_STRETCH;

	mainWinodwSplitter->setSizes(splitter_sizes);
	CustomWidget::resizeEvent(event);
}

