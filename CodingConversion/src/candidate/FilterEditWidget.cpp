﻿/*!
 * FileName: FilterEditWidget.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-10-30
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 过滤器编辑窗体
 */

#include <QApplication>
#include <QStandardItemModel>
#include <QHeaderView>

#include "SignalController.h"
#include "candidate/FilterEditWidget.h"
#include "candidate/CandidateFilterWidget.h"
#include "viewDelegate/FilterTableViewDelegate.h"

FilterEditWidget::FilterEditWidget(QWidget *parent /*= NULL*/) :CustomDialog(parent, true)
{
	setAttribute(Qt::WA_DeleteOnClose);
	createTitleBar(this, false, true, false, false, true, true);
	titleBar->setTitleText(trUtf8("设置过滤器"));
	setWindowIcon(QIcon(":/images/logo.png"));

	toolBox_HLay = new QHBoxLayout;
	tableView_VLay = new QVBoxLayout;
	bottom_HLay = new QHBoxLayout;

	addButt = new QPushButton;
	delButt = new QPushButton;
	upButt = new QPushButton;
	downButt = new QPushButton;

	filterTableView = new FilterEditView(this, this);
	filterModel = new QStandardItemModel(filterTableView);

	filterTableView->setModel(filterModel);
	filterModel->setColumnCount(2);
	filterModel->setHeaderData(0, Qt::Horizontal, trUtf8(""));
	filterModel->setHeaderData(1, Qt::Horizontal, trUtf8("匹配正则表达式"));
	filterTableView->verticalHeader()->hide();
	filterTableView->setAutoScroll(true);
	filterTableView->setColumnWidth(0, filterTableView->horizontalHeader()->height());
	filterTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	filterTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	filterTableView->setItemDelegate(new FilterTableViewDelegate(this)); //设置委托

	cancelButt = new QPushButton(trUtf8("取消"));
	saveButt = new QPushButton(trUtf8("保存"));

	toolBox_HLay->setContentsMargins(10, 10, 10, 0);
	toolBox_HLay->addStretch(1);
	toolBox_HLay->addWidget(addButt);
	toolBox_HLay->addWidget(delButt);
	toolBox_HLay->addWidget(upButt);
	toolBox_HLay->addWidget(downButt);

	tableView_VLay->setContentsMargins(10, 0, 10, 0);
	tableView_VLay->addWidget(filterTableView);

	bottom_HLay->setContentsMargins(10, 0, 10, 10);
	bottom_HLay->setSpacing(10);
	bottom_HLay->addStretch(1);
	bottom_HLay->addWidget(cancelButt);
	bottom_HLay->addWidget(saveButt);

	vAllLay->addLayout(toolBox_HLay);
	vAllLay->addLayout(tableView_VLay);
	vAllLay->addLayout(bottom_HLay);


	vAllLay->setMargin(0);
	vAllLay->setSpacing(10);
	vAllLay->setAlignment(Qt::AlignTop);
	this->setContentsMargins(1, 1, 1, 1);

	setObjectName("FilterEditWidget");
	addButt->setObjectName("addButt");
	delButt->setObjectName("delButt");
	upButt->setObjectName("upButt");
	downButt->setObjectName("downButt");
	cancelButt->setObjectName("normalButt");
	saveButt->setObjectName("normalButt");
	filterTableView->setObjectName("filterTableView");

	saveButt->setFocus();

	QFile qss(":/qss/FilterEditWidget.qss");
	qss.open(QFile::ReadOnly);
	QString qssStr = qss.readAll();
	qss.close();
	setStyleSheet(qssStr);
	setStyle(QApplication::style());

	initConnect();
}

FilterEditWidget::~FilterEditWidget()
{

}

void FilterEditWidget::initConnect()
{
	ConnectInfo connectInfo[] = {
		cancelButt,SIGNAL(clicked()),this,SLOT(close()),Qt::AutoConnection,
		saveButt,SIGNAL(clicked()),this,SLOT(saveFilterList()),Qt::AutoConnection,
	};

	SignalController::setConnectInfo(connectInfo, sizeof(connectInfo) / sizeof(ConnectInfo));
}

void FilterEditWidget::setFilterList(QList<QVariant> filter_list)
{
	for (int i = 0; i < filter_list.size(); i++)
	{
		FilterItem filter_item = filter_list[i].value<FilterItem>();
		QStandardItem *new_item0 = NULL, *new_item1 = NULL;
		if (filter_item.type == 0)
		{
			new_item0 = new QStandardItem(trUtf8("-"));
		}
		else
		{
			new_item0 = new QStandardItem(trUtf8("+"));
		}
		new_item0->setEditable(false);

		new_item1 = new QStandardItem(filter_item.filter_name);

		filterModel->setItem(i, 0, new_item0);
		filterModel->setItem(i, 1, new_item1);
	}
}

void FilterEditWidget::saveFilterList()
{
	if (parent() != NULL)
	{
		CandidateFilterWidget *filter_widget = static_cast<CandidateFilterWidget *>(parent());
		QList<QVariant> filter_list;
		filter_widget->updateFilter(filter_list);
	}
}
