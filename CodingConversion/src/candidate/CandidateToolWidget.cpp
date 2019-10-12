/*!
 * FileName: CandidateTopWidget.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-9-13
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 候选队列工具栏
 */

#include <QDir>
#include <QFileDialog>
#include <QListView>
#include <QTreeView>
#include <QLineEdit>
#include <QDialogButtonBox>

#include "SignalController.h"
#include "candidate/CandidateToolWidget.h"

CandidateToolWidget::CandidateToolWidget(QWidget* parent /*= NULL*/) :PainterWidget(parent)
{
	hAllLay = new QHBoxLayout(this);

	toolInfoLabel = new QLabel(trUtf8("工作列表:"));
	addButt = new QPushButton;
	delButt = new QPushButton;
	upButt = new QPushButton;
	downButt = new QPushButton;

	delButt->setDisabled(true);
	upButt->setDisabled(true);
	downButt->setDisabled(true);

	hAllLay->setSpacing(5);
	hAllLay->setContentsMargins(0, 0, 5, 0);
	hAllLay->addWidget(toolInfoLabel);
	hAllLay->addStretch(1);
	hAllLay->addWidget(addButt);
	hAllLay->addWidget(delButt);
	hAllLay->addWidget(upButt);
	hAllLay->addWidget(downButt);


	toolInfoLabel->setObjectName("toolInfoLabel");
	addButt->setObjectName("addButt");
	delButt->setObjectName("delButt");
	upButt->setObjectName("upButt");
	downButt->setObjectName("downButt");

	initConnect();
}

void CandidateToolWidget::initConnect()
{
	ConnectInfo connectInfo[] = {
		addButt,SIGNAL(clicked()),this,SLOT(addFilesOrDirs()),Qt::AutoConnection,
		delButt,SIGNAL(clicked()),signalController,SIGNAL(SIG_removeSelected()),Qt::AutoConnection,
		upButt,SIGNAL(clicked()),signalController,SIGNAL(SIG_moveUpItem()),Qt::AutoConnection,
		downButt,SIGNAL(clicked()),signalController,SIGNAL(SIG_moveDownItem()),Qt::AutoConnection,
		this,SIGNAL(SIG_addNewFilesOrDirs(QStringList)),signalController,SIGNAL(SIG_addNewFilesOrDirs(QStringList)),Qt::AutoConnection,
	};

	SignalController::setConnectInfo(connectInfo, sizeof(connectInfo) / sizeof(ConnectInfo));
}

void CandidateToolWidget::addFilesOrDirs()
{
	static QString lastPath = QDir::homePath();

	MultiFileDialog fileDialog(this, trUtf8("选择文件/文件夹"), lastPath, trUtf8("All Files(*)"));

	if (fileDialog.exec() == QFileDialog::Accepted)
	{
		QStringList openFiles = fileDialog.selectedFiles();
		if (!openFiles.isEmpty())
		{
			lastPath = openFiles.last();
			lastPath = QFileInfo(lastPath).absolutePath();

			emit SIG_addNewFilesOrDirs(openFiles);
		}
	}
	

	
}

MultiFileDialog::MultiFileDialog(QWidget* parent /*= nullptr*/, const QString& caption /*= QString()*/, const QString& directory /*= QString()*/, const QString& filter /*= QString()*/) :QFileDialog(parent, caption, directory, filter)
{
	
	setOption(QFileDialog::DontUseNativeDialog);
	setFileMode(QFileDialog::Directory);

	setNameFilter(filter);


	//修改open按钮的动作
	QDialogButtonBox* btnbox = this->findChild<QDialogButtonBox*>("buttonBox");
	if (btnbox != NULL)
	{
		disconnect(btnbox, SIGNAL(accepted()), this, SLOT(accept()));
		connect(btnbox, SIGNAL(accepted()), this, SLOT(fileOrDirOpen()));

		//因为当任何目录和文件都不选择时 默认是选中caption目录 所以要强制 disable一下 choose按钮 这样在什么都不选择时就不能accept
		if (this->acceptMode() == QFileDialog::AcceptOpen)
		{
			if (QPushButton * button = btnbox->button(QDialogButtonBox::Open))
			{
				button->setDisabled(true);
			}
		}
		else
		{
			if (QPushButton * button = btnbox->button(QDialogButtonBox::Save))
			{
				button->setDisabled(true);
			}
		}
	}

	//修改候选文件名的改变信号 以允许选中
	QLineEdit* edit = findChild<QLineEdit*>("fileNameEdit");
	if (NULL != edit)
	{
		connect(edit, SIGNAL(textChanged(const QString&)), this, SLOT(filesEditTextChanged(const QString&)), Qt::QueuedConnection);
	}


	QLabel* fileLabel = findChild<QLabel*>("fileNameLabel");
	if (fileLabel != NULL)
	{
		fileLabel->setText("File/Dir Name:");
	}

	//设置QFileDialog内部的表现方式
	QListView* lv = this->findChild<QListView*>("listView");
	if (lv != NULL)
	{
		lv->setSelectionMode(QAbstractItemView::ExtendedSelection);
	}
		
	QTreeView* tv = this->findChild<QTreeView*>();
	if (tv != NULL)
	{
		tv->setSelectionMode(QAbstractItemView::ExtendedSelection);
	}

	
}

void MultiFileDialog::fileOrDirOpen()
{
	QDialog::accept();
}

void MultiFileDialog::filesEditTextChanged(const QString& text)
{
	QDialogButtonBox* btnbox = this->findChild<QDialogButtonBox*>("buttonBox");

	if (btnbox != NULL)
	{
		QLineEdit* edit = findChild<QLineEdit*>("fileNameEdit");
		if (NULL != edit)
		{
			if (!edit->text().isEmpty())
			{
				if (this->acceptMode() == QFileDialog::AcceptOpen) 
				{
					if (QPushButton * button = btnbox->button(QDialogButtonBox::Open))
					{
						button->setEnabled(true);
					}
				}
				else 
				{
					if (QPushButton * button = btnbox->button(QDialogButtonBox::Save))
					{
						button->setEnabled(true);
					}
				}
			}
		}
	}
}
