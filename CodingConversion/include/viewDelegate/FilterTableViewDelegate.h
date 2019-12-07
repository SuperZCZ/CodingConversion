/*!
 * FileName: FilterTableViewDelegate.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-11-26
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 过滤器tableview委托样式
 */

#ifndef _FILTER_TABLE_VIEW_DELEGATE_H_
#define _FILTER_TABLE_VIEW_DELEGATE_H_

#include <QPushButton>
#include <QStyledItemDelegate>

class FilterTableViewDelegate :public QStyledItemDelegate
{
	Q_OBJECT
public:
	FilterTableViewDelegate(QObject *parent = NULL);
	~FilterTableViewDelegate();
protected:
	virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
	QPushButton *filter_style_butt;
};

#endif

