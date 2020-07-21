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

class TestWorker :public QObject
{
    Q_OBJECT
public:
    TestWorker();
    virtual ~TestWorker();

signals:
    void SIG_destroy();
public slots:
    void testFun();
    void destroy();
private slots:
    void SLOT_destroy();
private:
    QThread *workThread;
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
