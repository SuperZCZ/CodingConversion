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

#include <QVariant>
#include <QList>
#include <QApplication>
#include <QStyle>

#include "SignalController.h"
#include "candidate/CandidateFilterWidget.h"
#include "candidate/FilterEditWidget.h"

QList<QVariant> defFilterList;

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
	filterTabBar->setElideMode(Qt::ElideLeft);

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

	initDefFilterList();

	initConnect();
}

void CandidateFilterWidget::initConnect()
{
	ConnectInfo connectInfo[] = {
		filterTabBar,SIGNAL(tabCloseRequested(int)), this, SLOT(handleTabCloseClicked(int)),Qt::AutoConnection,
		filterSetButt,SIGNAL(clicked()),this,SLOT(settingFilter()),Qt::AutoConnection,
		signalController,SIGNAL(initDefaultSetting()),this,SLOT(resetFilterToDefault()),Qt::AutoConnection,
	};

	SignalController::setConnectInfo(connectInfo, sizeof(connectInfo) / sizeof(ConnectInfo));
}

void CandidateFilterWidget::initDefFilterList()
{
	defFilterList.clear();


	FilterItem filter_array[] =
	{
	{1,trUtf8("*.cpp")},
	{1,trUtf8("*.h")},
	{1,trUtf8("*.hpp")},
	{1,trUtf8("*.c")},
	{1,trUtf8("*.cc")},
	{1,trUtf8("*.cxx")},
	{1,trUtf8("*.mak")},
	{1,trUtf8("Makefile")},
	{1,trUtf8("*.txt")},
	{1,trUtf8("*.cmake")},
	{0,trUtf8("*.exe")},
	{0,trUtf8("*.so")},
	{0,trUtf8("*.a")},
	{0,trUtf8("*.lib")},
	{0,trUtf8("*.dll")},
	{0,trUtf8("*.obj")},
	{0,trUtf8("*.o")}
	};

	int size_item = sizeof(filter_array) / sizeof(FilterItem);

	for (int i = 0; i < size_item; i++)
	{
		defFilterList << QVariant::fromValue<FilterItem>(filter_array[i]);
	}
}

void CandidateFilterWidget::handleTabCloseClicked(int index)
{
	//qDebug() << "close " << index;
	filterTabBar->removeTab(index);
}

void CandidateFilterWidget::settingFilter()
{
	FilterEditWidget *filter_edit = new FilterEditWidget(this);
	QList<QVariant> filter_list;

	for (int i = 0; i < filterTabBar->count(); i++)
	{
		filter_list << filterTabBar->tabData(i);
	}

	filter_edit->setFilterList(filter_list);

	filter_edit->resize(500, 300);
	filter_edit->open();
}

void CandidateFilterWidget::clearAllFilter()
{
	int i = filterTabBar->count() - 1;
	for (; i >= 0; i--)
	{
		filterTabBar->removeTab(i);
	}
}

void CandidateFilterWidget::updateFilter(QList<QVariant> filter_list)
{
	clearAllFilter(); //清空所有
	for (int i = 0; i < filter_list.size(); i++)
	{
		FilterItem filter_item = filter_list[i].value<FilterItem>();
		if (filter_item.filter_name.isEmpty())
		{
			continue;
		}

		int index = filterTabBar->addTab(filter_item.filter_name);
		if (index >= 0)
		{
			if (filter_item.type == 0)
			{
				filterTabBar->setTabTextColor(index, QColor(255, 0, 0));     //红色代表不匹配
			}
			else
			{
				filterTabBar->setTabTextColor(index, QColor(255, 255, 255)); //白色代表匹配	
			}
			filterTabBar->setTabData(index, filter_list[i]);
			filterTabBar->setTabToolTip(index, filter_item.filter_name);
		}
	}
}

void CandidateFilterWidget::resetFilterToDefault()
{
	QList<QVariant> def_filters = defFilterList;
	updateFilter(def_filters);
}
