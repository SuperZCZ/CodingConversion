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
	stateBar= new CandidateSatusBar;

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
	vAllLay->addWidget(stateBar);


	setObjectName("candidateWidget");
	
	toolWidget->setObjectName("toolWidget");
	candidateTableView->setObjectName("candidateTableView");
	filterWidget->setObjectName("filterWidget");
	stateBar->setObjectName("stateBar");

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
		   candidateTableView,SIGNAL(updateStatusBarText(QString)),stateBar,SLOT(updateStatusText(QString)),Qt::AutoConnection,
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
	candidateTableView->updateCountInfo();
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
		candidateTableView->mergeDuplicatesItem(); //合并可能重复的项
		candidateTableView->updateCountInfo();
	}
}




CandidateTableView::CandidateTableView(CandidateToolWidget* toolWieget, QWidget* parent /*= NULL*/) :QTableView(parent)
{
	releativeToolWidget = toolWieget;
	recursionCheckBox = new QCheckBox(trUtf8("递归处理子目录"), this);
	installEventFilter(this);

	recursionCheckBox->setObjectName("CheckBox");
	recursionCheckBox->setChecked(true);

	initConnect();
}

void CandidateTableView::initConnect()
{
	ConnectInfo connectInfo[] = {
		recursionCheckBox,SIGNAL(stateChanged(int)),this,SLOT(mergeDuplicatesItem()),Qt::AutoConnection,
	};

	SignalController::setConnectInfo(connectInfo, sizeof(connectInfo) / sizeof(ConnectInfo));
}

void CandidateTableView::resizeEvent(QResizeEvent* event)
{
	QTableView::resizeEvent(event);
	QMargins old_m = viewportMargins();
	setViewportMargins(old_m.left(), old_m.top(), old_m.right(), 30); //保证tableview最下方有空白区域
	recursionCheckBox->move(5, this->height() - 20);
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

void CandidateTableView::mergeDuplicatesItem()
{
	if (recursionCheckBox->isChecked())
	{
		//进行遍历去重操作
		int index_child = 0, index_parent = 0;
		bool should_delete = false, happened_merge = false;

		for (index_child = 0; index_child < model()->rowCount(); index_child++)
		{
			should_delete = false;
			QString child_path = model()->index(index_child, 0).data(Qt::DisplayRole).toString();
			if (!QFileInfo::exists(child_path))
			{
				//不存在的文件也应该从列表中清除
				should_delete = true;
			}
			else
			{
				for (index_parent = 0; index_parent < model()->rowCount(); index_parent++)
				{
					if (index_parent == index_child)
					{
						continue;
					}
					else
					{
						QString parent_path = model()->index(index_parent, 0).data(Qt::DisplayRole).toString();
						if (isSubpathOf(child_path, parent_path))
						{
							//如果存在其父项则子项也应被清除
							should_delete = true;
							break;
						}
					}
				}
			}
			
			if (should_delete)
			{
				model()->removeRow(index_child);
				happened_merge = true;
				index_child--;
			}
		}
		if (happened_merge)
		{
			//弹出气泡提示窗
			signalController->popupTooltipsMessage(trUtf8("已合并重复项!"), trUtf8(""), INFORMATION_TOOLTIPS);
		}
	}
}

void CandidateTableView::updateCountInfo()
{
	QString infoText = trUtf8("未添加任何文件");
	int file_num = 0;
	int dir_num = 0;
	int other_num = 0;
	int sum_count = model()->rowCount();

	for (int i = 0; i < model()->rowCount(); i++)
	{
		QFileInfo file_info(model()->index(i, 0).data(Qt::DisplayRole).toString());
		if (file_info.isFile())
		{
			file_num++;
		}
		else if (file_info.isDir())
		{
			dir_num++;
		}
		else
		{
			other_num++;
		}
	}

	if (sum_count > 0)
	{
		infoText = trUtf8("");
		if (dir_num > 0)
		{
			if (!infoText.isEmpty())
			{
				infoText.append(" ");
			}
			infoText.append(trUtf8("目录:%1个").arg(dir_num));
		}

		if (file_num > 0)
		{
			if (!infoText.isEmpty())
			{
				infoText.append(" ");
			}
			infoText.append(trUtf8("文件:%1个").arg(file_num));
		}

		if (other_num > 0)
		{
			if (!infoText.isEmpty())
			{
				infoText.append(" ");
			}
			infoText.append(trUtf8("其他:%1个").arg(other_num));
		}

		if (sum_count > 0)
		{
			if (!infoText.isEmpty())
			{
				infoText.append(" ");
			}
			infoText.append(trUtf8("总计:%1项").arg(sum_count));
		}

	}

	emit updateStatusBarText(infoText);
}

bool CandidateTableView::isSubpathOf(const QString &child_path, const QString &parent_path)
{
	bool res = false;
	QFileInfo parent_info(parent_path);
	if (child_path.size() <= 0 || parent_path.size() <= 0)
	{
		return res;
	}
	if (parent_info.isDir())
	{
		//如果是目录就判断child_path是否为parent_path子孙目录或文件
		int index = 0;
		QString dst_path = parent_path;
		QString src_path = child_path;
		if (!dst_path.endsWith("/"))
		{
			dst_path.append("/");
		}
		index = 0;
		while (index < dst_path.size() && index < src_path.size() && dst_path[index] == src_path[index])
		{
			index++;
		}
		if (index == dst_path.size() || index == src_path.size())
		{
			if (index == dst_path.size() && (index == src_path.size() || src_path[index - 1] == '/'))
			{
				//匹配到完整的目的路径
				//  /home/  ---  /home/ | /home/file.1 
				res = true;
			}
			else
			{
				
				if ((index + 1) == dst_path.size() && index == src_path.size() && dst_path[index] == '/')
				{
					//检查是否只剩目的路径的结尾 / 没有匹配
					// /home/ ----- /home 
					res = true;
				}
			}
		}
	}
	else if (parent_info.isFile())
	{
		//如果是文件就比较两个路径是否相同
		if (parent_path.compare(child_path) == 0)
		{
			res = true;
		}
	}
	return res;
}

CandidateSatusBar::CandidateSatusBar(QWidget *parent /*= NULL*/) :PainterWidget(parent)
{
	vAllLay = new QVBoxLayout(this);
	info_HLay = new QHBoxLayout;

	info_label = new QLabel(trUtf8("未添加任何文件"));

	info_label->adjustSize();
	info_label->setAlignment(Qt::AlignRight);

	info_HLay->setMargin(0);
	info_HLay->setSpacing(0);
	info_HLay->addStretch(1);
	info_HLay->addWidget(info_label);

	vAllLay->setMargin(0);
	vAllLay->setSpacing(0);
	vAllLay->setAlignment(Qt::AlignCenter);
	vAllLay->addLayout(info_HLay);

	info_label->setObjectName("statusInfoLabel");
}

CandidateSatusBar::~CandidateSatusBar()
{

}

void CandidateSatusBar::updateStatusText(QString text)
{
	info_label->setText(text);
}
