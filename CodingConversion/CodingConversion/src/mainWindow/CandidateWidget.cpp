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

	setObjectName("candidateWidget");
	loadStyleSheet(":/qss/CandidateWidget.qss");
}
