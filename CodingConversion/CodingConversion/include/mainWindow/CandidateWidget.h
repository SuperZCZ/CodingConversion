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

#include "custom/CustomWidget.h"
#include "CharGlobal.h"

class CandidateWidget :public PainterWidget
{
	Q_OBJECT
public:
	CandidateWidget(QWidget* parent = NULL);
	virtual ~CandidateWidget() {}
protected:
private:
};

#endif
