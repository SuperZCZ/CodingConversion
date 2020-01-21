/*!
 * FileName: ProcessingView.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2020-1-19
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 处理视图
 */
#include <QDebug>
#include <QStandardItem>
#include <QHeaderView>
#include <QCursor>
#include <QBrush>

#include "view/ProcessingView.h"
#include "charDetect/CharDetect.h"

ProcessingView::ProcessingView(QWidget *parent /*= NULL*/) :QTableView(parent)
{
	processingModel = new ProcessingModel(this);
	QAbstractItemModel *src_model = model();
	if (src_model != NULL)
	{
		delete src_model;
	}
	setModel(processingModel);
	viewDelegate = new ProcessingViewDelegate(this);
	setItemDelegate(viewDelegate);


	//隐藏横纵的表头
	horizontalHeader()->hide();
	verticalHeader()->hide();
	setSelectionMode(QAbstractItemView::ExtendedSelection); //扩展选中方式
	setSelectionBehavior(QAbstractItemView::SelectRows);    //整行选中
	setShowGrid(false);
	setWordWrap(false); //不自动换行
	setMouseTracking(true);

	processingModel->setColumnCount(4);
	setColumnWidth(1, 110);
	setColumnWidth(2, 50);
	setColumnWidth(3, 110);
	horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	verticalHeader()->setDefaultSectionSize(35);

	initTestData();
}

ProcessingView::~ProcessingView()
{

}

void ProcessingView::initTestData()
{
	for (int i = 0; i < 50; i++)
	{
		QStandardItem *item1 = new QStandardItem;
		QStandardItem *item2 = new QStandardItem;
		QStandardItem *item3 = new QStandardItem;
		QStandardItem *item4 = new QStandardItem;

		chardet::TextCodecInfo src_codec, dst_codec;

		src_codec.codec = "UTF-16BE";
		src_codec.state_type = 1;
		src_codec.extend_info = trUtf8("");
		src_codec.bom_info.has_bom = false;

		dst_codec.codec = "UTF-16";
		dst_codec.state_type = -1;
		dst_codec.extend_info = trUtf8("异常原因");
		dst_codec.bom_info.has_bom = true;
		dst_codec.bom_info.bom_header = QByteArray("\xFF\xFE");
		dst_codec.bom_info.endian = "Little Endian";

		item1->setEditable(false);
		item2->setEditable(false);
		item3->setEditable(false);
		item4->setEditable(false);

		item1->setText(trUtf8("E:/GitHub/CodingConversion/CodingConversion/src/view/ProcessingView.cpp"));
		item2->setData(QVariant::fromValue<chardet::TextCodecInfo>(src_codec));
		item3->setText(trUtf8("Convert"));
		item4->setData(QVariant::fromValue<chardet::TextCodecInfo>(dst_codec));

		processingModel->setItem(i, 0, item1);
		processingModel->setItem(i, 1, item2);
		processingModel->setItem(i, 2, item3);
		processingModel->setItem(i, 3, item4);
	}


}

int ProcessingView::hoverRow()
{
	int res = -1;
	QWidget *view_widget = viewport();
	if (view_widget != NULL)
	{
		QModelIndex mouse_index = indexAt(view_widget->mapFromGlobal(QCursor::pos()));
		if (mouse_index.isValid())
		{
			res = mouse_index.row();
		}
	}
	return res;
}

void ProcessingView::leaveEvent(QEvent *event)
{
	update();  //显示更新一下视图 防止鼠标离开后hover背景不清空
	QTableView::leaveEvent(event);
}
