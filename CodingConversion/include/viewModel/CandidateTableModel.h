/*!
 * FileName: CandidateTableModel.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-10-29
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 就绪列表模式
 */

#ifndef _CANDIDATE_TABLE_MODEL_H_
#define _CANDIDATE_TABLE_MODEL_H_
#include <QTableView>
#include <QStandardItem>
#include <QStandardItemModel>

#include "CharGlobal.h"

class CandidateTableModel :public QStandardItemModel
{
	Q_OBJECT
public:
	CandidateTableModel(QObject* parent = NULL) :QStandardItemModel(parent) {}
	virtual ~CandidateTableModel() {}

protected:
	virtual bool moveRows(const QModelIndex& sourceParent, int sourceRow, int count, const QModelIndex& destinationParent, int destinationChild);
};

#endif