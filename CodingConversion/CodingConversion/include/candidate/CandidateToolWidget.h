/*!
 * FileName: CandidateToolWidget.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-9-13
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 候选队列工具栏
 */

#ifndef _CANDIDATE_TOOL_WIDGET_H_
#define _CANDIDATE_TOOL_WIDGET_H_

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>

#include "CharGlobal.h"
#include "custom/CustomWidget.h"

class CandidateToolWidget :public PainterWidget
{
	Q_OBJECT
public:
	CandidateToolWidget(QWidget* parent = NULL);
	virtual ~CandidateToolWidget() {}
protected:
private:
	QHBoxLayout* hAllLay;

	QLabel* toolInfoLabel;
	QPushButton* addButt;
	QPushButton* delButt;
	QPushButton* upButt;
	QPushButton* downButt;
};


#endif
