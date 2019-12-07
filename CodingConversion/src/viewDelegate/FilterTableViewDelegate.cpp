/*!
 * FileName: FilterTableViewDelegate.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-11-26
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 过滤器tableview委托样式
 */
#include <QPainter>

#include "viewDelegate/FilterTableViewDelegate.h"

FilterTableViewDelegate::FilterTableViewDelegate(QObject* parent /*= NULL*/) :QStyledItemDelegate(parent)
{
	filter_style_butt = new QPushButton(static_cast<QWidget*>(parent));
	filter_style_butt->setObjectName("FilterStyleButt");
	filter_style_butt->hide();
	filter_style_butt->setCheckable(true);
}

FilterTableViewDelegate::~FilterTableViewDelegate()
{
	delete filter_style_butt;
}

void FilterTableViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.column() == 0)
	{
		//QStyledItemDelegate::paint(painter, option, index); //先强制绘制 默认的背景及其他元素

		//在默认的基础上绘制自定义控件
		painter->save();
		//qDebug() << index  <<   " ----- option state" << option.state;
		//按钮的风格选项  
		QStyleOptionButton PushButtOption;

		PushButtOption.state = option.state; //复制单元格的属性
		PushButtOption.state = PushButtOption.state ^ QStyle::State_HasFocus;
		if (index.data(Qt::EditRole).toString() == trUtf8("+"))
		{
			PushButtOption.state = PushButtOption.state & (~QStyle::State_Off);
			PushButtOption.state = PushButtOption.state | QStyle::State_On;
		}
		else
		{
			PushButtOption.state = PushButtOption.state & (~QStyle::State_On);
			PushButtOption.state = PushButtOption.state | QStyle::State_Off;
		}
		//PushButtOption.text = index.data(Qt::EditRole).toString();  //按钮直接使用image标识内容


		//设置按钮大小和位置
		PushButtOption.rect = option.rect;
		//PushButtOption.rect.setHeight(filter_style_butt->height());  //根据qss中的大小进行设置大小
		//PushButtOption.rect.setWidth(filter_style_butt->width());
		PushButtOption.rect.moveCenter(option.rect.center()); //移动到单元格的中央

		//绘制QCheckBox  
		filter_style_butt->style()->drawControl(QStyle::CE_PushButton, &PushButtOption, painter, filter_style_butt);   //使用style_butt的样式对按钮风格选项进行绘制

		painter->restore();
	}
	else
	{
		//去掉获取焦点后讨厌的虚线
		QStyleOptionViewItem view_opt(option);
		if (view_opt.state & QStyle::State_HasFocus)
		{
			view_opt.state ^= QStyle::State_HasFocus;
		}
		QStyledItemDelegate::paint(painter, view_opt, index);
	}
}



