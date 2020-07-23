/*!
 * FileName: CandidateTableView.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-10-29
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 就绪列表视图
 */

#ifndef _CANDIDATE_TABLE_VIEW_H_
#define _CANDIDATE_TABLE_VIEW_H_
#include <QTableView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QCheckBox>
#include <QString>
#include <QList>

#include "CharGlobal.h"
#include "candidate/CandidateToolWidget.h"

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
	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dragMoveEvent(QDragMoveEvent *event);
	virtual void dropEvent(QDropEvent *event);
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
	void moveUpItem();
	void moveDownItem();
	void removeSelected();
	void addNewFilesOrDirs(QStringList fileList);
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
#endif
