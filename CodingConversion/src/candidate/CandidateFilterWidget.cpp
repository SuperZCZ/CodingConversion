/*!
 * FileName: CandidateFilterWidget.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-9-13
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 过滤器
 */

#include "candidate/CandidateFilterWidget.h"

CandidateFilterWidget::CandidateFilterWidget(QWidget* parent /*= NULL*/) :PainterWidget(parent)
{
	vAllLay = new QVBoxLayout(this);

	buttHLay = new QHBoxLayout;
	filterHLay = new QHBoxLayout;

	filterSetButt = new QPushButton(trUtf8("设置过滤器"));
	filterTabBar = new QTabBar;

	filterSetButt->setToolTip(trUtf8("处理白色后缀,忽略红色后缀,拖动标签以改变顺序"));

	filterTabBar->setDrawBase(false);  //不绘制基本部分只绘制选项卡 如果不希望看到选项卡后面的下划线可以关闭
	filterTabBar->setExpanding(false); //禁止tab自动填充至整个区域
	filterTabBar->setMovable(true);    //允许移动
	filterTabBar->setTabsClosable(true); //允许关闭
	filterTabBar->setUsesScrollButtons(true); //启用左右滚动按钮
	filterTabBar->setElideMode(Qt::ElideRight);


	filterTabBar->addTab(trUtf8("*"));
	filterTabBar->addTab(trUtf8("txt"));
	
	filterTabBar->setTabTextColor(0, QColor(255, 255, 255)); //白色代表匹配
	filterTabBar->setTabTextColor(1, QColor(255, 0, 0));     //红色代表不匹配


	buttHLay->setMargin(0);
	buttHLay->setAlignment(Qt::AlignVCenter);
	buttHLay->addWidget(filterSetButt);
	buttHLay->addStretch(1);

	filterHLay->setContentsMargins(5, 0, 0, 0);
	filterHLay->setAlignment(Qt::AlignVCenter);
	filterHLay->addWidget(filterTabBar);

	vAllLay->setMargin(0);
	vAllLay->setSpacing(0);
	vAllLay->addLayout(buttHLay);
	vAllLay->addLayout(filterHLay);

	filterSetButt->setObjectName("filterSetButt");
	filterTabBar->setObjectName("filterTabBar");


	connect(filterTabBar, SIGNAL(tabCloseRequested(int)), this, SLOT(handleTabCloseClicked(int)));
}

void CandidateFilterWidget::handleTabCloseClicked(int index)
{
	qDebug() << "close " << index;
	filterTabBar->removeTab(index);
}
