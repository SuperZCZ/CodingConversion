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


#include "CharGlobal.h"
#include "custom/CustomWidget.h"

class ProcessingWidget :public PainterWidget
{
	Q_OBJECT
public:
	ProcessingWidget(QWidget* parent = NULL);
	virtual ~ProcessingWidget() {}
protected:
private:
};

#endif
