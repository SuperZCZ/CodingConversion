/*!
 * FileName: CandidateWidget.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-9-10
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 候选窗体
 */

#include <QApplication>
#include <QStyle>
#include <QHeaderView>
#include <QScrollBar>
#include <QItemSelection>
#include <QMimeData>

#include "SignalController.h"
#include "mainWindow/CandidateWidget.h"
#include "viewDelegate/CandidateFileDelegate.h"

CandidateWidget::CandidateWidget(QWidget* parent /*= NULL*/) :PainterWidget(parent)
{
	vAllLay = new QVBoxLayout(this);
	toolWidget = new CandidateToolWidget;
	candidateTableView = new CandidateTableView(toolWidget, this);
	filterWidget = new CandidateFilterWidget;
	stateBar= new CandidateSatusBar;

	tableModel = new CandidateTableModel(candidateTableView);
	candidateTableView->setModel(tableModel);
	candidateTableView->verticalHeader()->hide();
	candidateTableView->horizontalHeader()->hide();
	candidateTableView->setShowGrid(false);
	candidateTableView->setFocusPolicy(Qt::ClickFocus);
	//candidateTableView->setGridStyle(Qt::SolidLine);
	//candidateTableView->setMouseTracking(true);


	tableModel->setColumnCount(1);
	candidateTableView->verticalHeader()->setDefaultSectionSize(30);
	candidateTableView->setItemDelegateForColumn(0, new CandidateFileDelegate(candidateTableView));
	candidateTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	vAllLay->setSpacing(0);
	vAllLay->setMargin(0);
	vAllLay->addWidget(toolWidget);
	vAllLay->addWidget(candidateTableView);
	vAllLay->addWidget(filterWidget);
	vAllLay->addWidget(stateBar);


	setObjectName("candidateWidget");
	
	toolWidget->setObjectName("toolWidget");
	candidateTableView->setObjectName("candidateTableView");
	filterWidget->setObjectName("filterWidget");
	stateBar->setObjectName("stateBar");

	setContentsMargins(0, 0, 1, 0);

	loadStyleSheet(":/qss/CandidateWidget.qss");

	initConnect();
}

void CandidateWidget::initConnect()
{
	ConnectInfo connectInfo[] = {
		{ signalController,SIGNAL(SIG_removeSelected()),candidateTableView,SLOT(removeSelected()),Qt::AutoConnection },
		{ signalController,SIGNAL(SIG_moveUpItem()),candidateTableView,SLOT(moveUpItem()),Qt::AutoConnection },
		{ signalController,SIGNAL(SIG_moveDownItem()),candidateTableView,SLOT(moveDownItem()),Qt::AutoConnection },
		{ signalController,SIGNAL(SIG_addNewFilesOrDirs(QStringList)),candidateTableView,SLOT(addNewFilesOrDirs(QStringList)),Qt::AutoConnection },
		{ candidateTableView,SIGNAL(updateStatusBarText(QString)),stateBar,SLOT(updateStatusText(QString)),Qt::AutoConnection },
	};

	SignalController::setConnectInfo(connectInfo, sizeof(connectInfo) / sizeof(ConnectInfo));
}