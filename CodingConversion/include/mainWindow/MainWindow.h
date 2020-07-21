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
#include <QAction>
#include <QSplitter>

#include "SignalController.h"
#include "CandidateWidget.h"
#include "ProcessingWidget.h"
#include "custom/CustomWidget.h"
#include "custom/CustomTitleBar.h"
#include "workThread/ProcessThread.h"


class MainTopTitleWidget:public PainterWidget
{
	Q_OBJECT
public:
	MainTopTitleWidget(QWidget* relative_widget, QWidget* parent = NULL);
 	virtual ~MainTopTitleWidget() {}

	void initConnect();
private:
	QHBoxLayout* hAllLay;

	CustomWidget* titleLabelWidget;
	QWidget* titleSettingWidget;
	CustomWidget* titleBarWidget;

	QHBoxLayout* setting_HLay;
	QPushButton* settingButt;
	QMenu* settingMenu;
signals:
	void menuActionClicked(QAction *action);
};


class MainWindow :public CustomWidget
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = NULL);
	virtual ~MainWindow();

	void initConnect();
protected:
	virtual void resizeEvent(QResizeEvent* event);
public slots:
void popupToolTipsMessage(QString text, QString title, ToolTipsType msg_type);
void handleAction(QAction *action);
void handleStartConversion();
void handlePauseConversion();
void handleStopConversion();

signals:
	void testSignal();
private:
	MainTopTitleWidget* topTitleWidget;

	QSplitter* mainWinodwSplitter;

	CandidateWidget* candidateWidget;
	ProcessingWidget* processingWidget;

	TestWorker *test_worker;
};



#endif