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
#include <QStandardItem>
#include <QStandardItemModel>
#include <QCheckBox>
#include <QString>
#include <QVector>

#include "CharGlobal.h"
#include "custom/CustomWidget.h"
#include "candidate/CandidateToolWidget.h"
#include "candidate/CandidateFilterWidget.h"
#include "candidate/CandidateSatusBar.h"
#include "view/CandidateTableView.h"
#include "viewModel/CandidateTableModel.h"


class CandidateWidget :public PainterWidget
{
	Q_OBJECT
public:
	CandidateWidget(QWidget* parent = NULL);
	virtual ~CandidateWidget() {}

	void initConnect();

	QVector<QString> getCandidateList();
protected:
	
private:
	QVBoxLayout* vAllLay;

	CandidateToolWidget* toolWidget;
	CandidateTableView* candidateTableView;
	CandidateFilterWidget* filterWidget;
	CandidateSatusBar* stateBar;

	CandidateTableModel *tableModel;
};

#endif
