/*!
 * FileName: FilterEditView.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-11-26
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 过滤器展示修改view
 */
#include <QDebug>
#include <QTableView>
#include <QHeaderView>

#include "view/FilterEditView.h"
#include "candidate/FilterEditWidget.h"


FilterEditView::FilterEditView(FilterEditWidget *editWieget, QWidget *parent /*= NULL*/) :QTableView(parent)
{
	setHorizontalHeader(new FilterEditHeadView(this));  //自定义表头
	this->setShowGrid(true);//显示网格
	this->setFocusPolicy(Qt::ClickFocus);
	horizontalHeader()->setSectionsClickable(false); //设置表头不可点击

	releativeEditWidget = editWieget;

	connect(this, SIGNAL(clicked(const QModelIndex &)), this, SLOT(handleItemClicked(const QModelIndex &)));
}

void FilterEditView::updateToolState()
{

}

void FilterEditView::initConnect()
{

}

void FilterEditView::resizeEvent(QResizeEvent *event)
{
	QTableView::resizeEvent(event);
}

/*bool FilterEditView::eventFilter(QObject *object, QEvent *event)
{
	return true;
}*/

void FilterEditView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	QTableView::selectionChanged(selected, deselected);
}

void FilterEditView::moveUpItem()
{

}

void FilterEditView::moveDownItem()
{

}

void FilterEditView::removeSelected()
{

}

void FilterEditView::addNewItem()
{

}

void FilterEditView::handleItemClicked(const QModelIndex &index)
{
	if (index.isValid())
	{
		if (index.column() == 0)
		{
			QString add_sub_str = index.data(Qt::EditRole).toString();
			if (add_sub_str == trUtf8("+"))
			{
				add_sub_str = trUtf8("-");
			}
			else
			{
				add_sub_str = trUtf8("+");
			}

			model()->setData(index, add_sub_str);
		}
	}
}

FilterEditHeadView::FilterEditHeadView(QWidget *parent /*= NULL*/) :QHeaderView(Qt::Horizontal, parent)
{
	left_label = new QLabel(this->viewport());
	left_label->setObjectName("FilterIconLabel");
	setObjectName("FilterEditHeadView");
}

FilterEditHeadView::~FilterEditHeadView()
{

}

void FilterEditHeadView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
	QHeaderView::paintSection(painter, rect, logicalIndex);
	if (logicalIndex == 0)
	{
		left_label->resize(rect.width() - 5, rect.height() - 5);
		left_label->move(rect.center() - left_label->rect().center());
	}
}

/*void FilterEditHeadView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex)
{
	
}*/