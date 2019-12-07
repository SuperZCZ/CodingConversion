/*!
 * FileName: CandidateSatusBar.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-10-29
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 就绪列表状态栏
 */

#include "candidate/CandidateSatusBar.h"

CandidateSatusBar::CandidateSatusBar(QWidget *parent /*= NULL*/) :PainterWidget(parent)
{
	vAllLay = new QVBoxLayout(this);
	info_HLay = new QHBoxLayout;

	info_label = new QLabel(trUtf8("未添加任何文件"));

	info_label->adjustSize();
	info_label->setAlignment(Qt::AlignRight);

	info_HLay->setMargin(0);
	info_HLay->setSpacing(0);
	info_HLay->addStretch(1);
	info_HLay->addWidget(info_label);

	vAllLay->setMargin(0);
	vAllLay->setSpacing(0);
	vAllLay->setAlignment(Qt::AlignCenter);
	vAllLay->addLayout(info_HLay);

	info_label->setObjectName("statusInfoLabel");
}

CandidateSatusBar::~CandidateSatusBar()
{

}

void CandidateSatusBar::updateStatusText(QString text)
{
	info_label->setText(text);
}

