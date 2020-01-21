/*!
 * FileName: ProcessingView.h
 *
 * Author:   ZhangChaoZe
 * Date:     2020-1-19
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 处理视图
 */
#ifndef _PROCESSING_VIEW_H_
#define _PROCESSING_VIEW_H_

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>

#include "CharGlobal.h"
#include "viewDelegate/ProcessingViewDelegate.h"
#include "viewModel/ProcessingModel.h"

class ProcessingView :public QTableView
{
	Q_OBJECT
public:
	ProcessingView(QWidget *parent = NULL);
	virtual ~ProcessingView();

	void initTestData();
	int hoverRow();
protected:
	virtual void leaveEvent(QEvent *event);
private:
	ProcessingModel *processingModel;
	ProcessingViewDelegate *viewDelegate;
};




#endif
