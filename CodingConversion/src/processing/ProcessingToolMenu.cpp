/*!
 * FileName: ProcessingToolMenu.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-12-10
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 处理工具栏
 */


#include "processing/ProcessingToolMenu.h"



ProcessingToolMenu::ProcessingToolMenu(QWidget *parent /*= NULL*/) :PainterWidget(parent)
{
    vAllLay = new QVBoxLayout(this);
    top_HLay = new QHBoxLayout;

    codeTypeCombox = new QComboBox;
    lineEndTypeCombox = new QComboBox;


    codeTypeCombox->addItem("1");
    codeTypeCombox->addItem("2");

    lineEndTypeCombox->addItem("1");
    lineEndTypeCombox->addItem("2");

    startPauseButt = new QPushButton(trUtf8("1"));
    stopButt = new QPushButton(trUtf8("2"));
	testButt = new QPushButton(trUtf8("3"));


    top_HLay->setSpacing(10);
    top_HLay->setAlignment(Qt::AlignCenter);
	top_HLay->setContentsMargins(10, 0, 10, 0);
    top_HLay->addWidget(codeTypeCombox);
    top_HLay->addWidget(lineEndTypeCombox);
    top_HLay->addStretch(1);
    top_HLay->addWidget(startPauseButt);
    top_HLay->addWidget(stopButt);
    top_HLay->addWidget(testButt);

    vAllLay->setSpacing(0);
    vAllLay->setMargin(0);
    vAllLay->addLayout(top_HLay);

    setObjectName("ProcessingToolMenu");
}

ProcessingToolMenu::~ProcessingToolMenu()
{

}
