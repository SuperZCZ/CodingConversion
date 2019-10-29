/*!
 * FileName: CandidateSatusBar.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-10-29
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 就绪列表状态栏
 */

#ifndef _CANDIDATE_STATUS_BAR_H_
#define _CANDIDATE_STATUS_BAR_H_

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "custom/CustomWidget.h"
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
#endif

