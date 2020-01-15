/*!
 * FileName: ProcessingStatusWidget.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-12-10
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 处理状态栏
 */

#include "processing/ProcessingStatusWidget.h"



ProcessingStatusWidget::ProcessingStatusWidget(QWidget *parent /*= NULL*/) :PainterWidget(parent)
{
    setObjectName("ProcessingStatusWidget");
}

ProcessingStatusWidget::~ProcessingStatusWidget()
{

}
