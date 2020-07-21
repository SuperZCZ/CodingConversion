/*!
 * FileName: SignalController.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-9-14
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 信号-槽转发器
 */

#ifndef _SIGNAL_CONTROLLER_H_
#define _SIGNAL_CONTROLLER_H_

#include <QObject>
#include <QDebug>

#include "CharGlobal.h"

struct ConnectInfo
{
	const QObject* sender; //发送者
	const char* signal_str;  //发送信号
	const QObject* reciever;  //接收者
	const char* sig_slot_str; //接收(信号/槽)
	Qt::ConnectionType connect_type;  //连接类型
};


enum ToolTipsType
{
	INFORMATION_TOOLTIPS = 0,
	CRITICAL_TOOLTIPS,
	SUCCESS_TOOLTIPS,
	WARNING_TOOLTIPS
};

class SignalController;

extern SignalController* signalController;

class SignalController :public QObject
{
	Q_OBJECT
public:

	//************************************
	// Method:    setConnectInfo
	// FullName:  SignalController::setConnectInfo
	// Access:    public static 
	// Returns:   void
	// Qualifier: 连接connectList中的信号与（槽/信号）
	// Parameter: struct ConnectInfo * connectList
	// Parameter: int item_num
	//************************************
	static void setConnectInfo(struct ConnectInfo* connectList, int item_num);

	static SignalController* createSignalController(QObject* parent = NULL)  //获取单实例句柄
	{
		if (instances == NULL)
		{
			instances = new SignalController(parent);
			instances->setObjectName("SignalController");
		}
		return instances;
	}

	static void destroySignalController()
	{
		if (instances != NULL)
		{
			delete instances;
			instances = NULL;
		}
	}
private:
	static SignalController* instances;        //单实例句柄

	SignalController(QObject* parent = NULL) :QObject(parent) { registerMetaALLType(); }
	virtual ~SignalController() {}

	void registerMetaALLType();  //注册自定义信号参数类型

	explicit SignalController(const SignalController&); //禁止复制
	SignalController& operator=(const SignalController&); //禁止赋值
//******************************************下面添加自定义信号与槽*************************************
public slots:
void popupTooltipsMessage(QString text, QString title = "", ToolTipsType msg_type = INFORMATION_TOOLTIPS);
void appStartExec();
signals:
	void initDefaultSetting();
	void SIG_removeSelected();
	void SIG_moveUpItem();
	void SIG_moveDownItem();
	void SIG_addNewFilesOrDirs(QStringList fileList);
	void SIG_popupTooltipsMessage(QString text, QString title, ToolTipsType msg_type);
	void SIG_startConversion();
	void SIG_pauseConversion();
	void SIG_stopConversion();
	void SIG_conversionStarted();
	void SIG_conversionStoped();
	void SIG_conversionPaused();
public slots:


};
#endif
