/*!
 * FileName: CandidateFileDelegate.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-9-15
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 候选文件列表委托
 */

#ifndef _CANDIDATE_FILE_DELEGATE_H_
#define _CANDIDATE_FILE_DELEGATE_H_

#include <QStyledItemDelegate>

class CandidateFileDelegate :public QStyledItemDelegate
{
	Q_OBJECT
public:
	CandidateFileDelegate(QObject* parent = NULL);
	virtual ~CandidateFileDelegate() {}
protected:
	virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
private:
};


#endif
