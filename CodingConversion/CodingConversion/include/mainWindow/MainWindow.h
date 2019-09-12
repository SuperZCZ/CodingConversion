/*!
 * FileName: MainWindow.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-9-9
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 主窗体widget
 */

#ifndef _MAIN_WINDOWS_H_
#define _MAIN_WINDOWS_H_

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QSplitter>


#include "CandidateWidget.h"
#include "ProcessingWidget.h"
#include "custom/CustomWidget.h"
#include "custom/CustomTitleBar.h"


class MainTopTitleWidget:public PainterWidget
{
	Q_OBJECT
public:
	MainTopTitleWidget(QWidget* relative_widget, QWidget* parent = NULL);
 	virtual ~MainTopTitleWidget() {}
private:
	QHBoxLayout* hAllLay;

	CustomWidget* titleLabelWidget;
	QWidget* titleSettingWidget;
	CustomWidget* titleBarWidget;

	QHBoxLayout* setting_HLay;
	QPushButton* settingButt;
	QMenu* settingMenu;

};


class MainWindow :public CustomWidget
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = NULL);
	virtual ~MainWindow();
protected:
	virtual void resizeEvent(QResizeEvent* event);
private:
	MainTopTitleWidget* topTitleWidget;

	QSplitter* mainWinodwSplitter;

	CandidateWidget* candidateWidget;
	ProcessingWidget* processingWidget;

};



#endif