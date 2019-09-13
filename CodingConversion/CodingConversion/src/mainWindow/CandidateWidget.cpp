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

#include "mainWindow/CandidateWidget.h"

CandidateWidget::CandidateWidget(QWidget* parent /*= NULL*/) :PainterWidget(parent)
{
	vAllLay = new QVBoxLayout(this);
	toolWidget = new CandidateToolWidget;
	candidateTableView = new QTableView;
	filterWidget = new CandidateFilterWidget;
	stateWidget = new PainterWidget;


	vAllLay->setSpacing(0);
	vAllLay->setMargin(0);
	vAllLay->addWidget(toolWidget);
	vAllLay->addWidget(candidateTableView);
	vAllLay->addWidget(filterWidget);
	vAllLay->addWidget(stateWidget);


	setObjectName("candidateWidget");
	toolWidget->setObjectName("toolWidget");
	candidateTableView->setObjectName("candidateTableView");
	filterWidget->setObjectName("filterWidget");
	stateWidget->setObjectName("stateWidget");

	setContentsMargins(0, 0, 1, 0);

	loadStyleSheet(":/qss/CandidateWidget.qss");
}
