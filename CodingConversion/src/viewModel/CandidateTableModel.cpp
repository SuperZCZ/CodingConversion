/*!
 * FileName: CandidateTableModel.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-10-29
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 就绪列表模式
 */

#include "viewModel/CandidateTableModel.h"


bool CandidateTableModel::moveRows(const QModelIndex& sourceParent, int sourceRow, int count, const QModelIndex& destinationParent, int destinationChild)
{

	//qDebug() << sourceRow << "---->" << destinationChild;
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

	//使用插入法进行移动 不删除或重新插入任何item实现数据的移动

	//1.先缓存源数据 2.移动目的行及其之后的数据给源数据腾出空间 3.放入元数据
	QVector<QString> itemTextBuff;
	QVector<QIcon> itemIconBuff;
	for (int i = 0; i < count; i++)
	{
		itemTextBuff.append(item(sourceRow + i)->text());
		itemIconBuff.append(item(sourceRow + i)->icon());
	}


	if (destinationChild < sourceRow)
	{
		//向前移动 只移动item中的数据而不移动item本身

		for (int i = sourceRow - 1; i >= destinationChild; i--)
		{
			item(i + count)->setText(item(i)->text());
			item(i + count)->setIcon(item(i)->icon());
		}

		for (int i = destinationChild; i < destinationChild + count; i++)
		{
			item(i)->setText(itemTextBuff[i - destinationChild]);
			item(i)->setIcon(itemIconBuff[i - destinationChild]);
		}
		/*for (int i = 0; i < count; ++i) //这种移动方法适合移动数据而不是iterm
		{
		insertRow(destinationChild + i, item(sourceRow + i+1)->clone());
		removeRow(sourceRow + i + 1);
		}*/
	}
	else if (sourceRow < destinationChild)
	{
		//向后移动
		for (int i = sourceRow + count; i <= destinationChild && i < rowCount(); i++)
		{
			item(i - count)->setText(item(i)->text());
			item(i - count)->setIcon(item(i)->icon());
		}

		for (int i = destinationChild - count + 1; i <= destinationChild; i++)
		{
			item(i)->setText(itemTextBuff[i - (destinationChild - count + 1)]);
			item(i)->setIcon(itemIconBuff[i - (destinationChild - count + 1)]);
		}
		/*for (int i = 0; i < count; i++) //这种移动方法适合移动数据而不是item 会删除item再创建 效率没有只移动数据高
		{
		insertRow(destinationChild, item(sourceRow)->clone());

		removeRow(sourceRow);
		}*/
	}

	QAbstractItemModel::endMoveRows();  //调用该函数以向view发送移动信号
	return true;
}
