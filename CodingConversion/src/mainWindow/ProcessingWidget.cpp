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

	textEdit = new QTextEdit;

	textEdit->setMaximumHeight(100);
	vAllLay->addWidget(textEdit);

	setObjectName("processingWidget");
	loadStyleSheet(":/qss/ProcessingWidget.qss");
}
