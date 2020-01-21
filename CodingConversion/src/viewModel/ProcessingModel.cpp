/*!
 * FileName: ProcessingModel.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2020-1-19
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 处理model
 */

#include <QDebug>
#include <QVariant>

#include "viewModel/ProcessingModel.h"
#include "view/ProcessingView.h"


ProcessingModel::ProcessingModel(QObject *parent /*= NULL*/):QStandardItemModel(parent)
{

}

ProcessingModel::~ProcessingModel()
{

}

QVariant ProcessingModel::data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const
{
	return QStandardItemModel::data(index, role);	
}
