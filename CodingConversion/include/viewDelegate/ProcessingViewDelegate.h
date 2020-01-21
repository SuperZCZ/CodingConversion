/*!
 * FileName: ProcessingViewDelegate.h
 *
 * Author:   ZhangChaoZe
 * Date:     2020-1-19
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 处理视图委托
 */

#ifndef _PROCESSING_VIEW_DELEGATE_H_
#define _PROCESSING_VIEW_DELEGATE_H_

#include <QPushButton>
#include <QStyledItemDelegate>

class ProcessingViewDelegate :public QStyledItemDelegate
{
	Q_OBJECT
public:
	ProcessingViewDelegate(QObject *parent = NULL);
	virtual ~ProcessingViewDelegate();

	enum
	{
		COLUMN_FILE_PATH = 0,
		COLUMN_SRC_CODEC_INFO,
		COLUMN_STATUS_INFO,
		COLUMN_DST_CODEC_INFO
	};

private:
	QPushButton *emptyStyleButt;
	QPushButton *errorStyleButt;
	QPushButton *convertStyleButt;
	QPushButton *checkStyleButt;
	QPushButton *codecInfoStyleButt;
protected:
	virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
	void paintHoverBackground(QPainter *painter, QStyleOptionViewItem &option, const QModelIndex &index) const;
	void paintPathItem(QPainter *painter, QStyleOptionViewItem &option, const QModelIndex &index) const;
	void paintCodecInfo(QPainter *painter, QStyleOptionViewItem &option, const QModelIndex &index) const;
	void paintStatusItem(QPainter *painter, QStyleOptionViewItem &option, const QModelIndex &index) const;
};
#endif
