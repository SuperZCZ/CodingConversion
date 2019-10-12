/*!
 * FileName: CandidateToolWidget.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-9-13
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 候选队列工具栏
 */

#ifndef _CANDIDATE_TOOL_WIDGET_H_
#define _CANDIDATE_TOOL_WIDGET_H_

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>

#include "CharGlobal.h"
#include "custom/CustomWidget.h"

class MultiFileDialog :public QFileDialog
{
	Q_OBJECT
public:
	MultiFileDialog(QWidget* parent = nullptr, const QString& caption = QString(), const QString& directory = QString(), const QString& filter = QString());
	virtual ~MultiFileDialog() {}
public slots:
	void fileOrDirOpen();
	void filesEditTextChanged(const QString& text);
};

class CandidateToolWidget :public PainterWidget
{
	Q_OBJECT
public:
	CandidateToolWidget(QWidget* parent = NULL);
	virtual ~CandidateToolWidget() {}

	void initConnect();
protected:
private:
	QHBoxLayout* hAllLay;

	QLabel* toolInfoLabel;
	QPushButton* addButt;
	QPushButton* delButt;
	QPushButton* upButt;
	QPushButton* downButt;
public slots:
	void addFilesOrDirs();
	void setAddButtEnabled(bool flag)
	{
		addButt->setEnabled(flag);
	}
	void setRemoveButtEnabled(bool flag)
	{
		delButt->setEnabled(flag);
	}
	void setUpButtEnabled(bool flag)
	{
		upButt->setEnabled(flag);
	}
	void setDownButtEnabled(bool flag)
	{
		downButt->setEnabled(flag);
	}
signals:
	void SIG_addNewFilesOrDirs(QStringList fileList);
};


#endif
