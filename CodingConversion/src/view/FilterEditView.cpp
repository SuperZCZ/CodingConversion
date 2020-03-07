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

#include "CharGlobal.h"
#include "SignalController.h"
#include "view/FilterEditView.h"
#include "QStandardItem"
#include "candidate/FilterEditWidget.h"



FilterEditView::FilterEditView(FilterEditWidget *editWieget, QWidget *parent /*= NULL*/) :QTableView(parent)
{
	setHorizontalHeader(new FilterEditHeadView(this));  //自定义表头
	this->setShowGrid(true);//显示网格
	this->setFocusPolicy(Qt::ClickFocus);
	horizontalHeader()->setSectionsClickable(false); //设置表头不可点击
	setSelectionBehavior(QAbstractItemView::SelectRows); //设置整行选中

	releativeEditWidget = editWieget;

	installEventFilter(this);
	initConnect();
}

void FilterEditView::initConnect()
{
	ConnectInfo connectInfo[] = {
		{ this,SIGNAL(clicked(const QModelIndex &)),this,SLOT(handleItemClicked(const QModelIndex &)),Qt::AutoConnection },
	};

	SignalController::setConnectInfo(connectInfo, sizeof(connectInfo) / sizeof(ConnectInfo));
}

void FilterEditView::resizeEvent(QResizeEvent *event)
{
	QTableView::resizeEvent(event);
	QMargins old_m = viewportMargins();
	setViewportMargins(old_m.left(), old_m.top(), old_m.right(), 15); //保证tableview最下方有空白区域
}

bool FilterEditView::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QEvent::MouseButtonPress)
	{
		if (object == this)
		{
			QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
			if (mouseEvent->button() == Qt::LeftButton)
			{
				QRect pos_rect(mouseEvent->pos(), QSize(1, 1));
				if (!viewport()->rect().intersects(pos_rect))
				{
					this->clearSelection();
				}
			}
		}
	}

	return QTableView::eventFilter(object, event);
}

void FilterEditView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	updateToolState(); //更新删除 上下移动按钮状态
	//qDebug() << "----------------------";
	QTableView::selectionChanged(selected, deselected);
}

void FilterEditView::moveUpItem()
{
	QModelIndex item_index = selectionModel()->selectedIndexes().at(0);
	QStandardItemModel *tableModel = static_cast<QStandardItemModel *>(model());
	tableModel->moveRow(item_index.parent(), item_index.row(), item_index.parent(), item_index.row() - 1);  //向上移动其实是移动到上一个的前面
	updateToolState();
}

void FilterEditView::moveDownItem()
{
	QModelIndex item_index = selectionModel()->selectedIndexes().at(0);
	QStandardItemModel *tableModel = static_cast<QStandardItemModel *>(model());
	tableModel->moveRow(item_index.parent(), item_index.row(), item_index.parent(), item_index.row() + 1); //向下移动其实是移动到下一个的后面
	updateToolState();
}

void FilterEditView::removeSelected()
{
	//反向删除
	int row_count = model()->rowCount();
	int row_now = row_count - 1;
	int last_remove_row = -1;
	for (; row_now >= 0; row_now--)
	{
		if (rowHaveItemSelected(row_now))
		{
			model()->removeRow(row_now);
			last_remove_row = row_now;
		}
	}

	if ((last_remove_row - 1) >= 0)
	{
		selectionModel()->select(model()->index(last_remove_row - 1, 0), QItemSelectionModel::Select | QItemSelectionModel::Rows); //整行选中
	}
	else if (last_remove_row == 0)
	{
		selectionModel()->select(model()->index(0, 0), QItemSelectionModel::Select | QItemSelectionModel::Rows);    //整行选中
	}
}

void FilterEditView::addNewItem()
{
	QList<QStandardItem *> item_list;
	item_list << new QStandardItem(trUtf8("+")); //默认是匹配运算
	item_list << new QStandardItem(trUtf8(""));  //匹配正则默认为空

	QStandardItemModel *tableModel = static_cast<QStandardItemModel *>(model());
	tableModel->appendRow(item_list);
}


void FilterEditView::updateToolState()
{
	bool removeEnabled = false;
	bool upEnabled = false;
	bool downEnabled = false;


	QModelIndexList now_selected = selectedIndexes();
	//qDebug() << now_selected;
	if (now_selected.size() > 0)
	{
		//是否选中单行
		if (isSelectOneRow(now_selected))
		{
			//启用
			if (model()->rowCount() == 1)
			{
				upEnabled = false;
				downEnabled = false;
			}
			else if (now_selected.first().row() == 0)
			{
				upEnabled = false;
				downEnabled = true;
			}
			else if (now_selected.first().row() == (model()->rowCount() - 1))
			{
				upEnabled = true;
				downEnabled = false;
			}
			else
			{
				upEnabled = true;
				downEnabled = true;
			}
		}
		else
		{
			//禁用上下移动
			upEnabled = false;
			downEnabled = false;
		}
		//启用删除
		removeEnabled = true;
	}
	else
	{
		//禁用remove
		removeEnabled = false;
	}
	//qDebug() << removeEnabled << " " << upEnabled << " " << downEnabled;

	releativeEditWidget->setRemoveButtEnabled(removeEnabled);
	releativeEditWidget->setUpButtEnabled(upEnabled);
	releativeEditWidget->setDownButtEnabled(downEnabled);
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

bool FilterEditView::isSelectOneRow(const QModelIndexList &selected_index_list)
{
	bool res = false;
	int first_row = -1;
	int i = 0;
	for (i = 0; i < selected_index_list.size(); i++)
	{
		if (i == 0)
		{
			first_row = selected_index_list[i].row();
		}
		else
		{
			if (first_row != selected_index_list[i].row())
			{
				break;
			}
		}
	}
	if (i >= selected_index_list.size())
	{
		res = true;
	}
	return res;
}

bool FilterEditView::rowHaveItemSelected(int row)
{
	bool res = false;
	for (int i = 0; i < model()->columnCount(); i++)
	{
		if (selectionModel()->isSelected(model()->index(row, i)))
		{
			res = true;
			break;
		}
	}
	return res;
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