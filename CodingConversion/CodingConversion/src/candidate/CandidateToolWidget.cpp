/*!
 * FileName: CandidateTopWidget.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-9-13
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 候选队列工具栏
 */

#include "candidate/CandidateToolWidget.h"

CandidateToolWidget::CandidateToolWidget(QWidget* parent /*= NULL*/) :PainterWidget(parent)
{
	hAllLay = new QHBoxLayout(this);

	toolInfoLabel = new QLabel(trUtf8("工作列表:"));
	addButt = new QPushButton;
	delButt = new QPushButton;
	upButt = new QPushButton;
	downButt = new QPushButton;

	hAllLay->setSpacing(5);
	hAllLay->setContentsMargins(0, 0, 5, 0);
	hAllLay->addWidget(toolInfoLabel);
	hAllLay->addStretch(1);
	hAllLay->addWidget(addButt);
	hAllLay->addWidget(delButt);
	hAllLay->addWidget(upButt);
	hAllLay->addWidget(downButt);


	toolInfoLabel->setObjectName("toolInfoLabel");
	addButt->setObjectName("addButt");
	delButt->setObjectName("delButt");
	upButt->setObjectName("upButt");
	downButt->setObjectName("downButt");

}
