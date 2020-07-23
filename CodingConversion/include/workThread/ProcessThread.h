/*!
 * FileName: ProcessThread.h
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

class DetectThread:public QThread
{
	Q_OBJECT
public:
	DetectThread(QObject *parent = NULL);
	virtual ~DetectThread();

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

	QStringList getDirSubFileList(const QString &dir_path, bool recursion_dir);

public slots:
void stop(); //终止检测线程
void pause(); //暂停检测线程
void start(); //开始/继续

signals:
	void paused();  //暂停
	void startover(); //开始
	void stoped();  //停止
};


/*class ProcessThread :public QThread
{
public:
    ProcessThread() {}
protected:
    virtual void run();
private:
    volatile bool is_running;
};*/

#endif
