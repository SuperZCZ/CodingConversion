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
#include <QMetaType>
#include <QListView>

#include "processing/ProcessingToolMenu.h"

Q_DECLARE_METATYPE(ProcessingToolMenu::CodecType)


ProcessingToolMenu::ProcessingToolMenu(QWidget *parent /*= NULL*/) :PainterWidget(parent)
{
    vAllLay = new QVBoxLayout(this);
    top_HLay = new QHBoxLayout;

	codeLabel = new QLabel(trUtf8("编码:"));
	lineLabel = new QLabel(trUtf8("行尾:"));

    codeTypeCombox = new QComboBox;
    lineEndTypeCombox = new QComboBox;

	codeTypeCombox->setView(new QListView);
	lineEndTypeCombox->setView(new QListView);
	codeTypeCombox->setFixedWidth(100);
	lineEndTypeCombox->setFixedWidth(125);

    startPauseButt = new QPushButton;
    stopButt = new QPushButton;
	testButt = new QPushButton;

	startPauseButt->setCheckable(true);
	startPauseButt->setToolTip(trUtf8("开始/暂停"));
	stopButt->setToolTip(trUtf8("停止"));
	testButt->setToolTip(trUtf8("仅检测"));

    top_HLay->setSpacing(10);
    top_HLay->setAlignment(Qt::AlignCenter);
	top_HLay->setContentsMargins(10, 0, 10, 0);
	top_HLay->addWidget(codeLabel);
    top_HLay->addWidget(codeTypeCombox);
	top_HLay->addSpacing(20);
	top_HLay->addWidget(lineLabel);
    top_HLay->addWidget(lineEndTypeCombox);
    top_HLay->addStretch(1);
    top_HLay->addWidget(startPauseButt);
    top_HLay->addWidget(stopButt);
    top_HLay->addWidget(testButt);

    vAllLay->setSpacing(0);
    vAllLay->setMargin(0);
    vAllLay->addLayout(top_HLay);

	initCodeTypeList();
	initLineEndTypeList();

    setObjectName("ProcessingToolMenu");
	startPauseButt->setObjectName("startPauseButt");
	stopButt->setObjectName("stopButt");
	testButt->setObjectName("testButt");
	
	lineEndTypeCombox->setObjectName("TypeComboBox");
	codeTypeCombox->setObjectName("TypeComboBox");
}

ProcessingToolMenu::~ProcessingToolMenu()
{

}

void ProcessingToolMenu::initCodeTypeList()
{
	CodecType codec_array[] =
	{
		{trUtf8("UTF-8"),{trUtf8("UTF-8"),{false,trUtf8(""),QByteArray()}}},
		{trUtf8("UTF-8-BOM"),{trUtf8("UTF-8"),{true,trUtf8(""),QByteArray("\xEF\xBB\xBF",3)}}},
		{trUtf8("GB18030"),{trUtf8("GB18030"),{false,trUtf8(""),QByteArray()}}},
	};

	int array_size = sizeof(codec_array) / sizeof(CodecType);

	for (int i = 0; i < array_size; i++)
	{
		codeTypeCombox->addItem(codec_array[i].displayName);
		codeTypeCombox->setItemData(i, QVariant::fromValue<CodecType>(codec_array[i]));
	}
}

void ProcessingToolMenu::initLineEndTypeList()
{
	LineEndType line_end_array[] = 
	{
		{trUtf8("--"),0},
		{trUtf8("Windows (CR LF)"),1},
		{trUtf8("Unix (LF)"),2},
		{trUtf8("Macintosh (CR)"),3}
	};

	int array_size = sizeof(line_end_array) / sizeof(LineEndType);

	for (int i = 0; i < array_size; i++)
	{
		lineEndTypeCombox->addItem(line_end_array[i].displayName);
		lineEndTypeCombox->setItemData(i, line_end_array[i].endType);
	}
}
