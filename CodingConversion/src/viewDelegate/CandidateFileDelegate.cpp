/*!
 * FileName: CandidateFileDelegate.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-9-15
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 候选文件列表委托
 */
#include <QDebug>
#include <QPainter>
#include "viewDelegate/CandidateFileDelegate.h"

CandidateFileDelegate::CandidateFileDelegate(QObject* parent /*= NULL*/) :QStyledItemDelegate(parent)
{

}

void CandidateFileDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	//去掉获取焦点后讨厌的虚线
	QStyleOptionViewItem view_opt(option);
    if (view_opt.state & QStyle::State_HasFocus)
    {
        view_opt.state ^= QStyle::State_HasFocus;
    }
    view_opt.textElideMode = Qt::ElideMiddle; //QT好像有点bug 设置了但不生效
	QStyledItemDelegate::paint(painter, view_opt, index);
}

