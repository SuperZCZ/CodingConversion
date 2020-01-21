/*!
 * FileName: ProcessingWidget.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-9-10
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 处理窗体
 */
#include <QApplication>
#include <QStyle>
#include <QDebug>

#include "mainWindow/ProcessingWidget.h"

ProcessingWidget::ProcessingWidget(QWidget* parent /*= NULL*/) :PainterWidget(parent)
{

	vAllLay = new QVBoxLayout(this);

	tool_menu = new ProcessingToolMenu;
	processingView = new ProcessingView;
    status_widget = new ProcessingStatusWidget;

    vAllLay->setSpacing(0);
	vAllLay->setContentsMargins(0, 0, 0, 0);

	vAllLay->addWidget(tool_menu);
    vAllLay->addWidget(processingView, 1);
    vAllLay->addWidget(status_widget);

	setObjectName("processingWidget");
    processingView->setObjectName("processingView");

	loadStyleSheet(":/qss/ProcessingWidget.qss");
}
