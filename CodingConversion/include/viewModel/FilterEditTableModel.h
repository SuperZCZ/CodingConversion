/*!
 * FileName: FilterEditTableModel.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-12-9
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 过滤器设置model
 */

#ifndef _FILTER_EDIT_TABLE_MODEL_H_
#define _FILTER_EDIT_TABLE_MODEL_H_
#include <QTableView>
#include <QStandardItem>
#include <QStandardItemModel>

#include "CharGlobal.h"

class FilterEditTableModel :public QStandardItemModel
{
	Q_OBJECT
public:
    FilterEditTableModel(QObject *parent = NULL) :QStandardItemModel(parent) {}
	virtual ~FilterEditTableModel() {}

protected:
	virtual bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild);
};

#endif