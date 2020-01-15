/*!
 * FileName: FilterEditTableModel.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-12-9
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 过滤器设置model
 */

#include "viewModel/FilterEditTableModel.h"


bool FilterEditTableModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
	if (destinationChild < 0 || destinationChild >= rowCount())
	{
		return false;
	}

	if (destinationChild < sourceRow)
	{
		if (!QAbstractItemModel::beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild)) //调用该函数和endMoveRows以向view通知相关信号
		{
			return false;
		}
	}
	else if (destinationChild > sourceRow)
	{
		if (!QAbstractItemModel::beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild + 1))
		{
			return false;
		}
	}

	//使用插入法进行移动 不删除任何现有item实现数据的移动

	//1.先缓存源数据 2.移动目的行及其之后的数据给源数据腾出空间 3.放入元数据
	QVector<QVector<QString>> rowItemTextBuff;
	for (int i = 0; i < count; i++)
	{
		QVector<QString> rowItems;
		for (int j = 0; j < columnCount(); j++)
		{
			rowItems.append(item(sourceRow + i, j)->text()); //缓存sourceRow + i行 j列的数据
		}
		rowItemTextBuff.append(rowItems);
	}


	if (destinationChild < sourceRow)
	{
		//向前移动 只移动item中的数据而不移动item本身

		for (int i = sourceRow - 1; i >= destinationChild; i--)
		{
			for (int j = 0; j < columnCount(); j++)
			{
				item(i + count, j)->setText(item(i, j)->text());  //移动i行 j列数据
			}
		}

		for (int i = destinationChild; i < destinationChild + count; i++)
		{
			for (int j = 0; j < columnCount(); j++)
			{
				item(i, j)->setText(rowItemTextBuff[i - destinationChild][j]);  //设置i行j列数据
			}
		}
	}
	else if (sourceRow < destinationChild)
	{
		//向后移动
		for (int i = sourceRow + count; i <= destinationChild && i < rowCount(); i++)
		{
			for (int j = 0; j < columnCount(); j++)
			{
				item(i - count, j)->setText(item(i, j)->text());  //移动i行 j列数据
			}
		}

		for (int i = destinationChild - count + 1; i <= destinationChild; i++)
		{
			for (int j = 0; j < columnCount(); j++)
			{
				item(i, j)->setText(rowItemTextBuff[i - (destinationChild - count + 1)][j]);  //设置i行 j列数据
			}
		}
	}

	QAbstractItemModel::endMoveRows();  //调用该函数以向view发送移动信号
	return true;
}
