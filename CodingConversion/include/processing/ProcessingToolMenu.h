/*!
 * FileName: ProcessingToolMenu.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-12-10
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 处理工具栏
 */

#ifndef _PROCESSING_TOOL_MENU_H_
#define _PROCESSING_TOOL_MENU_H_

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "CharGlobal.h"
#include "charDetect/CharDetect.h"
#include "custom/CustomWidget.h"

class ProcessingToolMenu : public PainterWidget
{
    Q_OBJECT
public:
    ProcessingToolMenu(QWidget *parent = NULL);
    virtual ~ProcessingToolMenu();

	void initCodeTypeList();
	void initLineEndTypeList();

	void initConnect();
public:
	struct CodecType
	{
		QString displayName;
		chardet::TextCodecInfo codecInfo;
	};

	struct LineEndType
	{
		QString displayName;
		unsigned endType;
	};

private:

    QVBoxLayout *vAllLay;
    QHBoxLayout *top_HLay;
	QLabel *codeLabel;
	QLabel *lineLabel;

    QComboBox *codeTypeCombox;
    QComboBox *lineEndTypeCombox;

    QPushButton *startPauseButt;  //开始/暂停
    QPushButton *stopButt;        //停止按钮
    QPushButton *modeButt;        //检测/转换按钮
signals:
    void SIG_startConversion();
    void SIG_pauseConversion();
    void SIG_stopConversion();

public slots:
void handleStartPauseClicked();
void handleStopClicked();
void handleModeSwitchClicked();

void handleConversionStart();
void handleConversionPause();
void handleConversionStop();

};


Q_DECLARE_METATYPE(ProcessingToolMenu::CodecType)

#endif
