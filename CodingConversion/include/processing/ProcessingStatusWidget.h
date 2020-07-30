/*!
 * FileName: ProcessingStatusWidget.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-12-10
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 检测状态栏
 */

#ifndef _PROCESSING_STATUS_WIDGET_H_
#define _PROCESSING_STATUS_WIDGET_H_
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>

#include "CharGlobal.h"
#include "custom/CustomWidget.h"



class ProcessingStatusWidget :public PainterWidget
{
    Q_OBJECT
public:
    ProcessingStatusWidget(QWidget *parent = NULL);
    virtual ~ProcessingStatusWidget();

	void initConnect();
public slots:
void handleStartConversion();
void handleStopConversion();
void handlePauseConversion();

void handleConversionStoped();
void handleConversionPaused();
void handleConversionStarted();

void updateConversionProgress(int current_count);
void updateConversionCount(int count);

private:
	QHBoxLayout *hAllLay;

	QProgressBar *progressBar;
	QLabel *progressInfoLabel;
	QLabel *statusInfoLabel;
	QLabel *statusIconLabel;
};

#endif
