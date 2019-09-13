/*!
 * FileName: CandidateWidget.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-9-10
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 候选窗体
 */

#ifndef _CANDIDATE_WIDGET_H_
#define _CANDIDATE_WIDGET_H_

#include <QWidget>
#include <QVBoxLayout>
#include <QTableView>

#include "CharGlobal.h"
#include "custom/CustomWidget.h"
#include "candidate/CandidateToolWidget.h"
#include "candidate/CandidateFilterWidget.h"

class CandidateWidget :public PainterWidget
{
	Q_OBJECT
public:
	CandidateWidget(QWidget* parent = NULL);
	virtual ~CandidateWidget() {}
protected:
private:
	QVBoxLayout* vAllLay;

	CandidateToolWidget* toolWidget;
	QTableView* candidateTableView;
	CandidateFilterWidget* filterWidget;
	PainterWidget* stateWidget;
};

#endif
