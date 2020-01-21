/*!
 * FileName: ProcessingViewDelegate.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2020-1-19
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 处理视图委托
 */
#include <QDebug>
#include <QPainter>
#include <QApplication>

#include "viewDelegate/ProcessingViewDelegate.h"
#include "view/ProcessingView.h"
#include "charDetect/CharDetect.h"

ProcessingViewDelegate::ProcessingViewDelegate(QObject *parent /*= NULL*/) :QStyledItemDelegate(parent)
{
	emptyStyleButt = new QPushButton(static_cast<QWidget *>(parent));
	errorStyleButt = new QPushButton(static_cast<QWidget *>(parent));
	convertStyleButt = new QPushButton(static_cast<QWidget *>(parent));
	checkStyleButt = new QPushButton(static_cast<QWidget *>(parent));
	codecInfoStyleButt = new QPushButton(static_cast<QWidget *>(parent));
	
	emptyStyleButt->hide();
	errorStyleButt->hide();
	convertStyleButt->hide();
	checkStyleButt->hide();
	codecInfoStyleButt->hide();

	emptyStyleButt->setObjectName("emptyStyleButt");
	errorStyleButt->setObjectName("errorStyleButt");
	convertStyleButt->setObjectName("convertStyleButt");
	checkStyleButt->setObjectName("checkStyleButt");
	codecInfoStyleButt->setObjectName("codecInfoStyleButt");
}

ProcessingViewDelegate::~ProcessingViewDelegate()
{

}

void ProcessingViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.isValid())
	{
		QStyleOptionViewItem custom_opt = option;
		initStyleOption(&custom_opt, index);

		//清除具有焦点时的边框
		if (custom_opt.state & QStyle::State_HasFocus)
		{
			custom_opt.state ^= QStyle::State_HasFocus;
		}

		//绘制鼠标浮动整行背景颜色 使用这种办法不能在外部qss中设置hover状态属性否则会覆盖以下函数的修改
		paintHoverBackground(painter, custom_opt, index);

		int colum = index.column();
		switch (colum)
		{
		case COLUMN_FILE_PATH:
			paintPathItem(painter, custom_opt, index);
			break;
		case COLUMN_SRC_CODEC_INFO:
			paintCodecInfo(painter, custom_opt, index);
			break;
		case COLUMN_DST_CODEC_INFO:
			paintCodecInfo(painter, custom_opt, index);
			break;
		case COLUMN_STATUS_INFO:
			paintStatusItem(painter, custom_opt, index);
			break;
		default:
			paintPathItem(painter, custom_opt, index);
			break;
		}
	}
	else
	{
		QStyledItemDelegate::paint(painter, option, index);
	}
}

void ProcessingViewDelegate::paintHoverBackground(QPainter *painter, QStyleOptionViewItem &option, const QModelIndex &index) const
{
	volatile static int row_hover = -1;
	static QColor hover_back_color(229, 243, 255);
	ProcessingView *processView = static_cast<ProcessingView *>(parent());
	int befor_hover = row_hover;

	//更新当前浮动行
	if (processView != NULL)
	{
		QWidget *view_widget = processView->viewport();
		if (view_widget != NULL)
		{
			QPoint mouse_view_pos = view_widget->mapFromGlobal(QCursor::pos());
			QRect view_rect = view_widget->rect();
			QRect mouse_rect(mouse_view_pos, QSize(1, 1));
			if (!view_rect.intersects(mouse_rect))
			{
				//光标不在viewport中
				row_hover = -1;
			}
			else
			{
				//光标还在viewport中
				QModelIndex mouse_index = processView->indexAt(mouse_view_pos);
				if (!mouse_index.isValid())
				{
					row_hover = -1;
				}
				else
				{
					row_hover = mouse_index.row();
				}
			}
		}
	}

	if (row_hover != befor_hover && befor_hover != -1)
	{
		//先清空之前的浮动颜色
		if (processView != NULL)
		{
			for (int i = 0; i < processView->model()->columnCount(); i++)
			{
				processView->update(processView->model()->index(befor_hover, i));
			}
		}
	}

	if (option.state&QStyle::State_MouseOver&&row_hover != -1)
	{
		painter->fillRect(option.rect, hover_back_color);
		if (processView != NULL)
		{
			for (int i = 0; i < processView->model()->columnCount(); i++)
			{
				if (i == index.column())
				{
					continue;
				}
				processView->update(processView->model()->index(row_hover, i));
			}
		}
	}
	else
	{
		if (index.isValid() && index.row() == row_hover)  //其余列也属于鼠标浮动行的也进行背景着色
		{
			painter->fillRect(option.rect, hover_back_color);
		}
	}
}

void ProcessingViewDelegate::paintPathItem(QPainter *painter, QStyleOptionViewItem &option, const QModelIndex &index) const
{
	painter->save();

	option.textElideMode = Qt::ElideMiddle; //某些QT版本好像有点bug 设置了但不生效(Qt5.6.0可以)

	const QWidget *widget = option.widget;
	QStyle *style = widget ? widget->style() : QApplication::style();
	style->drawControl(QStyle::CE_ItemViewItem, &option, painter, widget);  //使用qss中设置的样式再次绘制
	painter->restore();
}

void ProcessingViewDelegate::paintCodecInfo(QPainter *painter, QStyleOptionViewItem &option, const QModelIndex &index) const
{
	//1.先绘制底层样式
	painter->save();
	option.text.clear(); //不需要绘制底层样式的文字
	const QWidget *widget = option.widget;
	QStyle *style = widget ? widget->style() : QApplication::style();
	style->drawControl(QStyle::CE_ItemViewItem, &option, painter, widget);
	painter->restore();


	//2.再绘制上层小控件样式
	painter->save();
	//按钮的风格选项 
	QStyleOptionButton buttOption;
	buttOption.state = option.state; //复制单元格的属性
	buttOption.state = buttOption.state^QStyle::State_HasFocus;
	

	chardet::TextCodecInfo codecInfo = index.data(Qt::UserRole + 1).value<chardet::TextCodecInfo>();

	QPushButton *styleButt = emptyStyleButt;
	if (codecInfo.state_type == 1)
	{
		buttOption.text = codecInfo.codec;
		if (codecInfo.bom_info.has_bom)
		{
			buttOption.text.append(trUtf8(" ●"));
		}
		styleButt = codecInfoStyleButt;
	}
	else if (codecInfo.state_type == -1)
	{
		if (!codecInfo.extend_info.isEmpty())
		{
			buttOption.text = codecInfo.extend_info;
		}
		else
		{
			buttOption.text = trUtf8("未知错误");
		}
		styleButt = errorStyleButt;
	}
	else
	{
		styleButt = emptyStyleButt;
	}

	//设置按钮大小和位置
	buttOption.rect = option.rect;
	buttOption.rect.setHeight(styleButt->height());  //根据qss中的大小进行设置大小
	buttOption.rect.setWidth(option.rect.width() - 20);
	buttOption.rect.moveCenter(option.rect.center()); //移动到单元格的中央

	styleButt->style()->drawControl(QStyle::CE_PushButton, &buttOption, painter, styleButt);   //使用style_butt的样式对按钮风格选项进行绘制
	painter->restore();
}

void ProcessingViewDelegate::paintStatusItem(QPainter *painter, QStyleOptionViewItem &option, const QModelIndex &index) const
{
	//1.先绘制底层样式
	painter->save();
	option.text.clear(); //不需要绘制底层样式的文字
	const QWidget *widget = option.widget;
	QStyle *style = widget ? widget->style() : QApplication::style();
	style->drawControl(QStyle::CE_ItemViewItem, &option, painter, widget);
	painter->restore();

	//2.再绘制上层小控件样式
	painter->save();
	//按钮的风格选项 
	QStyleOptionButton statusButtOption;
	statusButtOption.state = option.state; //复制单元格的属性
	statusButtOption.state = statusButtOption.state^QStyle::State_HasFocus;

	QString status_str = index.data(Qt::EditRole).toString();
	QPushButton *statusStyleButt = emptyStyleButt;
	if (status_str == trUtf8("Convert"))
	{
		statusStyleButt = convertStyleButt;
	}
	else if (status_str == trUtf8("CheckOnly"))
	{
		statusStyleButt = checkStyleButt;
	}
	else
	{
		statusStyleButt = emptyStyleButt;
	}

	//设置按钮大小和位置
	statusButtOption.rect = option.rect;
	statusButtOption.rect.setHeight(statusStyleButt->height());  //根据qss中的大小进行设置大小
	statusButtOption.rect.setWidth(statusStyleButt->width());
	statusButtOption.rect.moveCenter(option.rect.center()); //移动到单元格的中央

	statusStyleButt->style()->drawControl(QStyle::CE_PushButton, &statusButtOption, painter, statusStyleButt);   //使用style_butt的样式对按钮风格选项进行绘制
	painter->restore();
}
