/*!
 * FileName: ConversionProcess.h
 *
 * Author:   ZhangChaoZe
 * Date:     2020-4-9
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 处理线程
 */

#ifndef _PROCESS_THREAD_H_
#define _PROCESS_THREAD_H_

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QString>
#include <QVector>
#include <QQueue>
#include <QVariant>

//待处理队列
class ConversionProcessQueue
{
public:
	ConversionProcessQueue(int process_thread_num);
	virtual ~ConversionProcessQueue() {}

	enum ProcessTaskType
	{
		PROCESS_TASK_NONE_TYPE = 0,
		PROCESS_FILE_PATH_TYPE,
		PROCESS_STOP_THREEAD_TYPE,
	};

	struct ProcessTask
	{
		ProcessTaskType taskType;
		QVariant taskData;
	};


	ProcessTask dequeue();
	void enqueue(const ProcessTask &task);

	void pushOverFlag(); //所有待检测文件都处理完了在待处理队列最后置入结束标识(处理线程最终会有序退出)
	void stop();
	void pause();
	void startover();
private:
	int processThreadNum;

	bool is_pause;
	QQueue<QString> processQueue;

	QMutex queueMutex;
	QWaitCondition queueWaitCond;
};

class ConversionProcessThread;

//检测和转换管理线程
//检测或转换任务的启动暂停和终止都以这个线程的状态为准
class ConversionThread:public QThread
{
	Q_OBJECT
public:
	ConversionThread(QObject *parent = NULL);
	virtual ~ConversionThread();

	void initConnect();

	void setCandidateQueue(const QVector<QString> &path_list);
protected:
	virtual void run();

private:
	volatile bool is_stop;
	volatile bool is_pause;
	QQueue<QString> candidateQueue;

	QMutex condMutex;
	QWaitCondition runWaitCond;

	ConversionProcessQueue *conversionProcessQueue;

	QStringList getDirSubFileList(const QString &dir_path, bool recursion_dir);

	QVector<ConversionProcessThread *> processingThreads;

	void createProcessingThread(int thread_num);
	void destroyProcessingThread();
public slots:
void stop(); //终止检测线程
void pause(); //暂停检测线程
void start(); //开始/继续

signals:
	void paused();  //暂停
	void startover(); //开始
	void stoped();  //停止
};







//转换处理线程
//由检测管理线程创建的多个专门负责转换文件编码和换行方式的线程
class ConversionProcessThread :public QThread
{
	Q_OBJECT
public:
	ConversionProcessThread(ConversionProcessQueue *processQueue, QObject *parent = NULL);
	virtual ~ConversionProcessThread();

	void initConnect();
protected:
    virtual void run();
private:
	ConversionProcessQueue *conversionProcessQueue;
};

#endif
