/*!
 * FileName: ProcessingToolMenu.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-12-10
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 处理工具栏
 */

#ifndef _PROCESSING_TOOL_MENU_H_
#define _PROCESSING_TOOL_MENU_H_

#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "CharGlobal.h"
#include "custom/CustomWidget.h"

class ProcessingToolMenu : public PainterWidget
{
    Q_OBJECT
public:
    ProcessingToolMenu(QWidget *parent = NULL);
    virtual ~ProcessingToolMenu();
protected:
private:

    QVBoxLayout *vAllLay;
    QHBoxLayout *top_HLay;

    QComboBox *codeTypeCombox;
    QComboBox *lineEndTypeCombox;

    QPushButton *startPauseButt;  //开始暂停
    QPushButton *stopButt;        //停止按钮
    QPushButton *testButt;        //检测按钮
};


#endif
