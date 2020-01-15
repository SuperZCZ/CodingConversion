/*!
 * FileName: FilterEditView.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-11-26
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 过滤器展示修改view
 */

#ifndef _FILTER_EDIT_VIEW_H_
#define _FILTER_EDIT_VIEW_H_

#include <QLabel>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QCheckBox>

#include "CharGlobal.h"

class FilterEditWidget;


class FilterEditHeadView :public QHeaderView
{
	Q_OBJECT
public:
	FilterEditHeadView(QWidget *parent = NULL);
	virtual ~FilterEditHeadView();
protected:
	virtual void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;

private:
	QLabel *left_label;
};

class FilterEditView :public QTableView
{
	Q_OBJECT
public:
	FilterEditView(FilterEditWidget *editWieget, QWidget *parent = NULL);
	virtual ~FilterEditView() {}

	void updateToolState();
	void initConnect();
protected:
	void resizeEvent(QResizeEvent *event);
	virtual bool eventFilter(QObject *object, QEvent *event);
	virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

public slots:

	void moveUpItem();
	void moveDownItem();
	void removeSelected();
	void addNewItem();

	void handleItemClicked(const QModelIndex &index);
private:
	FilterEditWidget *releativeEditWidget;

	bool isSelectOneRow(const QModelIndexList &selected_index_list);
	bool rowHaveItemSelected(int row);
};

#endif
