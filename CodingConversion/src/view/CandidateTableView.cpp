/*!
 * FileName: CandidateTableView.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-10-29
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 就绪列表视图
 */
#include <QMimeData>
#include <QEvent>

#include "SignalController.h"
#include "mainWindow/CandidateWidget.h"
#include "view/CandidateTableView.h"
#include "setting/GlobalSetting.h"

CandidateTableView::CandidateTableView(CandidateToolWidget* toolWieget, QWidget* parent /*= NULL*/) :QTableView(parent)
{
	setDragEnabled(false); //禁止tableview默认的拖拽
	setAcceptDrops(true);  //允许接收外部拖拽
	releativeToolWidget = toolWieget;
	recursionCheckBox = new QCheckBox(trUtf8("递归处理子目录"), this);
	installEventFilter(this);

	recursionCheckBox->setObjectName("CheckBox");
	recursionCheckBox->setChecked(true);
	globalSetting->setValue("recursionDir", true);
	initConnect();
}

void CandidateTableView::initConnect()
{
	ConnectInfo connectInfo[] = {
		{ recursionCheckBox,SIGNAL(stateChanged(int)),this,SLOT(mergeDuplicatesItem()),Qt::AutoConnection },
	};

	SignalController::setConnectInfo(connectInfo, sizeof(connectInfo) / sizeof(ConnectInfo));
}

void CandidateTableView::removeSelected()
{
	//反向删除
	int row_count = model()->rowCount();
	int row_now = row_count - 1;
	int last_remove_row = -1;
	for (; row_now >= 0; row_now--)
	{
		if (selectionModel()->isSelected(model()->index(row_now, 0)))
		{
			model()->removeRow(row_now);
			last_remove_row = row_now;
		}
	}

	if ((last_remove_row - 1) >= 0)
	{
		selectionModel()->select(model()->index(last_remove_row - 1, 0), QItemSelectionModel::Select);
	}
	else if (last_remove_row == 0)
	{
		selectionModel()->select(model()->index(0, 0), QItemSelectionModel::Select);
	}
	updateCountInfo();
}

void CandidateTableView::addNewFilesOrDirs(QStringList fileList)
{
	for (int i = 0; i < fileList.size(); i++)
	{
		if (!fileList[i].isEmpty())
		{
			QStandardItem* newItem = new QStandardItem(fileList[i]);
			newItem->setEditable(false);
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
			CandidateTableModel *tableModel = static_cast<CandidateTableModel *>(model());
			tableModel->appendRow(newItem);
		}
		mergeDuplicatesItem(); //合并可能重复的项
		updateCountInfo();
	}
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

void CandidateTableView::dragEnterEvent(QDragEnterEvent *event)
{
	event->accept();
}

void CandidateTableView::dragMoveEvent(QDragMoveEvent *event)
{
	event->accept();
}

void CandidateTableView::dropEvent(QDropEvent *event)
{
	event->accept();
	const QMimeData *mime_data = event->mimeData();
	if (mime_data != NULL && mime_data->hasUrls())
	{
		QList<QUrl> file_urls = mime_data->urls();
		QStringList file_list;
		for (int i = 0; i < file_urls.size(); i++)
		{
			if (file_urls[i].isLocalFile())
			{
				file_list << file_urls[i].toLocalFile();
			}
		}
		addNewFilesOrDirs(file_list);
	}
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

void CandidateTableView::moveUpItem()
{
	QModelIndex item_index = selectionModel()->selectedIndexes().at(0);
	CandidateTableModel *tableModel = static_cast<CandidateTableModel *>(model());
	tableModel->moveRow(item_index.parent(), item_index.row(), item_index.parent(), item_index.row() - 1);  //向上移动其实是移动到上一个的前面
	updateToolState();
}

void CandidateTableView::moveDownItem()
{
	QModelIndex item_index = selectionModel()->selectedIndexes().at(0);
	CandidateTableModel *tableModel = static_cast<CandidateTableModel *>(model());
	tableModel->moveRow(item_index.parent(), item_index.row(), item_index.parent(), item_index.row() + 1); //向下移动其实是移动到下一个的后面
	updateToolState();
}

void CandidateTableView::mergeDuplicatesItem()
{
	//递归不递归都要去重 不递归去除直接子项(除目录) 递归去除子项和孙子项
	bool recursion_dir = recursionCheckBox->isChecked();
	if (recursion_dir)
	{
		//进行遍历去重操作
		globalSetting->setValue("recursionDir", true);
	}
	else
	{
		globalSetting->setValue("recursionDir", false);
	}

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
						if (!recursion_dir)
						{
							//不递归则只需去除直接子项(目录除外)
							//如果是子项是目录或者非直接子项则不用清除
							QFileInfo child_info(child_path);
							QFileInfo parent_info(parent_path);
							if (child_info.absolutePath() != parent_info.absolutePath())
							{
								//两者路径不一样不是同一文件或同一目录
								if (child_info.isDir())
								{
									should_delete = false;
								}
								else
								{
									if (parent_info.isDir() && (child_info.absoluteDir().absolutePath() != parent_info.absoluteFilePath()))
									{
										//子项文件不为父目录的直接子项 不用清除子项
										should_delete = false;
									}
								}
							}
						}
						if (should_delete)
						{
							break;
						}
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