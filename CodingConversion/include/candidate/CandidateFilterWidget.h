/*!
 * FileName: CandidateFilterWidget.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-9-13
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 过滤器
 */

#ifndef _CANDIDATE_FILTER_WIDGET_H_
#define _CANDIDATE_FILTER_WIDGET_H_

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabBar>
#include <QPushButton>
#include <QLabel>

#include "CharGlobal.h"
#include "custom/CustomWidget.h"

class CandidateFilterWidget:public PainterWidget
{
	Q_OBJECT
public:
	CandidateFilterWidget(QWidget* parent = NULL);
	virtual ~CandidateFilterWidget() {}

	void initConnect();
protected:
private:
	QVBoxLayout* vAllLay;
	QHBoxLayout* buttHLay;
	QHBoxLayout* filterHLay;

	QPushButton* filterSetButt;
	QTabBar* filterTabBar;
public slots:
	void handleTabCloseClicked(int index);
	void settingFilter();
	void clearAllFilter();
	void updateFilter(QList<QVariant> filter_list);

};

#endif