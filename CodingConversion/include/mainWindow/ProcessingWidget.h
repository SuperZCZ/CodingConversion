/*!
 * FileName: ProcessingWidget.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-9-10
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 处理窗体
 */

#ifndef _PROCESSING_WIDGET_H_
#define _PROCESSING_WIDGET_H_
#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QTableView>

#include "CharGlobal.h"
#include "custom/CustomWidget.h"
#include "processing/ProcessingToolMenu.h"
#include "processing/ProcessingStatusWidget.h"

class ProcessingWidget :public PainterWidget
{
	Q_OBJECT
public:
	ProcessingWidget(QWidget* parent = NULL);
	virtual ~ProcessingWidget() {}
protected:
private:
	QVBoxLayout* vAllLay;

    ProcessingToolMenu *tool_menu;
    QTableView *processingTableView;
    ProcessingStatusWidget *status_widget;
};

#endif
