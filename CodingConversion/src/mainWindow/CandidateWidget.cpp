/*!
 * FileName: CandidateWidget.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-9-10
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 候选窗体
 */

#include <QApplication>
#include <QStyle>
#include <QHeaderView>
#include <QScrollBar>
#include <QItemSelection>

#include "SignalController.h"
#include "mainWindow/CandidateWidget.h"
#include "viewDelegate/CandidateFileDelegate.h"

CandidateWidget::CandidateWidget(QWidget* parent /*= NULL*/) :PainterWidget(parent)
{
	vAllLay = new QVBoxLayout(this);
	toolWidget = new CandidateToolWidget;
	candidateTableView = new CandidateTableView(toolWidget, this);
	filterWidget = new CandidateFilterWidget;
	stateWidget = new PainterWidget;

	tableModel = new CandidateTableModel(candidateTableView);
	candidateTableView->setModel(tableModel);
	candidateTableView->verticalHeader()->hide();
	candidateTableView->horizontalHeader()->hide();
	candidateTableView->setShowGrid(false);
	candidateTableView->setFocusPolicy(Qt::ClickFocus);
	//candidateTableView->setGridStyle(Qt::SolidLine);
	//candidateTableView->setMouseTracking(true);


	tableModel->setColumnCount(1);
	candidateTableView->verticalHeader()->setDefaultSectionSize(30);
	candidateTableView->setItemDelegateForColumn(0, new CandidateFileDelegate(tableModel));
	candidateTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	vAllLay->setSpacing(0);
	vAllLay->setMargin(0);
	vAllLay->addWidget(toolWidget);
	vAllLay->addWidget(candidateTableView);
	vAllLay->addWidget(filterWidget);
	vAllLay->addWidget(stateWidget);


	setObjectName("candidateWidget");
	
	toolWidget->setObjectName("toolWidget");
	candidateTableView->setObjectName("candidateTableView");
	filterWidget->setObjectName("filterWidget");
	stateWidget->setObjectName("stateWidget");

	setContentsMargins(0, 0, 1, 0);

	loadStyleSheet(":/qss/CandidateWidget.qss");

	initConnect();
}

void CandidateWidget::initConnect()
{
	ConnectInfo connectInfo[] = {
		   signalController,SIGNAL(SIG_removeSelected()),this,SLOT(removeSelected()),Qt::AutoConnection,
		   signalController,SIGNAL(SIG_moveUpItem()),this,SLOT(moveUpItem()),Qt::AutoConnection,
		   signalController,SIGNAL(SIG_moveDownItem()),this,SLOT(moveDownItem()),Qt::AutoConnection,
		   signalController,SIGNAL(SIG_addNewFilesOrDirs(QStringList)),this,SLOT(addNewFilesOrDirs(QStringList)),Qt::AutoConnection,
	};

	SignalController::setConnectInfo(connectInfo, sizeof(connectInfo) / sizeof(ConnectInfo));
}

void CandidateWidget::moveUpItem()
{
	
	QModelIndex item_index = candidateTableView->selectionModel()->selectedIndexes().at(0);
	tableModel->moveRow(item_index.parent(), item_index.row(), item_index.parent(), item_index.row() - 1);  //向上移动其实师移动到上一个的前面
	candidateTableView->updateToolState();
}

void CandidateWidget::moveDownItem()
{
	
	QModelIndex item_index = candidateTableView->selectionModel()->selectedIndexes().at(0);
	tableModel->moveRow(item_index.parent(), item_index.row(), item_index.parent(), item_index.row() + 1); //向下移动其实师移动到下一个的后面
	candidateTableView->updateToolState();
}

void CandidateWidget::removeSelected()
{
	//反向删除
	int row_count = tableModel->rowCount();
	int row_now = row_count - 1;
	int last_remove_row = -1;
	for (; row_now >= 0; row_now--)
	{
		if (candidateTableView->selectionModel()->isSelected(tableModel->index(row_now,0)))
		{
			tableModel->removeRow(row_now);
			last_remove_row = row_now;
		}
	}

	if ((last_remove_row-1) >= 0)
	{
		candidateTableView->selectionModel()->select(tableModel->index(last_remove_row - 1, 0), QItemSelectionModel::Select);
	}
	else if (last_remove_row == 0)
	{
		candidateTableView->selectionModel()->select(tableModel->index(0, 0), QItemSelectionModel::Select);
	}
}

void CandidateWidget::addNewFilesOrDirs(QStringList fileList)
{
	for (int i = 0; i < fileList.size(); i++)
	{
		if (!fileList[i].isEmpty())
		{
			QStandardItem* newItem = new QStandardItem(fileList[i]);
			newItem->setToolTip(fileList[i]);

			QFileInfo fileInfo(fileList[i]);
			if (fileInfo.isFile())
			{
				newItem->setIcon(QIcon(":/images/file_type_icon.png"));
			}
			else if (fileInfo.isDir())
			{
				newItem->setIcon(QIcon(":/images/folder_type_icon.png"));
			}
			else
			{
				newItem->setIcon(QIcon(":/images/file_type_icon.png"));
			}
			tableModel->appendRow(newItem);
		}
	}
}




CandidateTableView::CandidateTableView(CandidateToolWidget* toolWieget, QWidget* parent /*= NULL*/) :QTableView(parent)
{
	releativeToolWidget = toolWieget;
	installEventFilter(this);
}

void CandidateTableView::resizeEvent(QResizeEvent* event)
{
	QTableView::resizeEvent(event);
	QMargins old_m = viewportMargins();
	setViewportMargins(old_m.left(), old_m.top(), old_m.right(), 30); //保证tableview最下方有空白区域
}

bool CandidateTableView::eventFilter(QObject* object, QEvent* event)
{
	if (event->type() == QEvent::MouseButtonPress)
	{
		if (object == this)
		{
			QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
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

void CandidateTableView::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
	updateToolState(); //更新删除 上下移动按钮状态

	//qDebug() << "--------------------";
	QTableView::selectionChanged(selected, deselected);
}

void CandidateTableView::updateToolState()
{
	bool removeEnabled = false;
	bool upEnabled = false;
	bool downEnabled = false;


	QModelIndexList now_selected = selectedIndexes();
	if (now_selected.size() > 0)
	{
		if (now_selected.size() == 1)
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

	releativeToolWidget->setRemoveButtEnabled(removeEnabled);
	releativeToolWidget->setUpButtEnabled(upEnabled);
	releativeToolWidget->setDownButtEnabled(downEnabled);
}

bool CandidateTableModel::moveRows(const QModelIndex& sourceParent, int sourceRow, int count, const QModelIndex& destinationParent, int destinationChild)
{

	//qDebug() << sourceRow << "---->" << destinationChild;
	if (destinationChild < 0 || destinationChild >= rowCount())
	{
		return false;
	}

	if (destinationChild < sourceRow)
	{
		if (!QAbstractItemModel::beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild))
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
		/*for (int i = 0; i < count; i++) //这种移动方法适合移动数据而不是item
		{
			insertRow(destinationChild, item(sourceRow)->clone());
			removeRow(sourceRow);
		}*/
	}

	QAbstractItemModel::endMoveRows();
	return true;
}
