/*!
 * FileName: CandidateWidget.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-9-10
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 候选窗体
 */

#ifndef _CANDIDATE_WIDGET_H_
#define _CANDIDATE_WIDGET_H_

#include <QWidget>
#include <QVBoxLayout>
#include <QTableView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QCheckBox>

#include "CharGlobal.h"
#include "custom/CustomWidget.h"
#include "candidate/CandidateToolWidget.h"
#include "candidate/CandidateFilterWidget.h"


class CandidateTableView :public QTableView
{
	Q_OBJECT
public:
	CandidateTableView(CandidateToolWidget* toolWieget, QWidget* parent = NULL);
	virtual ~CandidateTableView() {}

	void updateToolState();
	void initConnect();
protected:
	void resizeEvent(QResizeEvent* event);
	virtual bool eventFilter(QObject* object, QEvent* event);
	virtual void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);	
signals:
	void updateStatusBarText(QString text);
public slots:
//************************************
// Method:    mergeDuplicatesItem
// FullName:  CandidateWidget::mergeDuplicatesItem
// Access:    public 
// Returns:   void
// Qualifier: 合并重复项 最终结果为所有项的最小但最全的集合
//            具体行为和设置是否递归处理子目录有关。
//            例如：同时存在目录目录中的文件或子目录如果设置了递归处理子目录则将只保留最顶层的父目录
//************************************
void mergeDuplicatesItem();

void updateCountInfo();
private:
	CandidateToolWidget* releativeToolWidget;
	QCheckBox* recursionCheckBox;

	//************************************
	// Method:    isSubpathOf
	// FullName:  CandidateTableView::isSubpathOf
	// Access:    private 
	// Returns:   bool
	// Qualifier: 判断child_path是否为parent_path的子孙目录
	// Parameter: const QString & child_path
	// Parameter: const QString & parent_path
	//************************************
	bool isSubpathOf(const QString &child_path, const QString &parent_path);
};

class CandidateTableModel :public QStandardItemModel
{
	Q_OBJECT
public:
	CandidateTableModel(QObject* parent = NULL) :QStandardItemModel(parent) {}
	virtual ~CandidateTableModel() {}

protected:
	virtual bool moveRows(const QModelIndex& sourceParent, int sourceRow, int count, const QModelIndex& destinationParent, int destinationChild);
};


class CandidateSatusBar :public PainterWidget
{
	Q_OBJECT
public:
	CandidateSatusBar(QWidget *parent = NULL);
	~CandidateSatusBar();
public slots:
	void updateStatusText(QString text);
protected:
private:
	QVBoxLayout *vAllLay;
	QHBoxLayout *info_HLay;
	QLabel *info_label;
};


class CandidateWidget :public PainterWidget
{
	Q_OBJECT
public:
	CandidateWidget(QWidget* parent = NULL);
	virtual ~CandidateWidget() {}

	void initConnect();
protected:
	
private:
	QVBoxLayout* vAllLay;

	CandidateToolWidget* toolWidget;
	CandidateTableView* candidateTableView;
	CandidateFilterWidget* filterWidget;
	CandidateSatusBar* stateBar;

	CandidateTableModel *tableModel;

private:

	

public slots:
	void moveUpItem();
	void moveDownItem();
	void removeSelected();
	void addNewFilesOrDirs(QStringList fileList);
};

#endif
