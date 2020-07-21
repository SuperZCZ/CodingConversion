/*!
 * FileName: ProcessThread.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2020-4-9
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 处理线程
 */

#include <QDebug>
#include <QApplication>

#include "workThread/ProcessThread.h"


TestWorker::TestWorker()
{
    workThread = NULL;
    workThread = new QThread;
    this->moveToThread(workThread);
    
	connect(workThread, SIGNAL(finished()), this, SLOT(deleteLater()));
	connect(this, SIGNAL(SIG_destroy()), this, SLOT(SLOT_destroy()));

    workThread->start();
}

TestWorker::~TestWorker()
{
	if (workThread != NULL)
    {
        disconnect(workThread, SIGNAL(finished()), this, SLOT(deleteLater()));
        if (workThread->isRunning()) //如果是外部直接调用析构函数(调用线程不是workThread) 则workThread一定没有退出事件轮询 则需手动quit 并等待事件轮询真正退出后再析构 这样做可能会阻塞调用线程!
		{
			workThread->quit();
			workThread->wait(); //quit调用后线程有可能还未完全退出 如果不执行wait操作Qt会抛出异常(Destroyed while thread is still running)
            qDebug() << "666666666666666666666666";
        }
        qDebug() << "++++++++45454";
        delete workThread; //不能在线程的事件轮询中析构自己！！ 否则Qt会抛出 不能在线程中wait自己的异常！
    }
    qDebug() << "delete TestWorker";
}

void TestWorker::testFun()
{
    qDebug() << "begin sleep*****";
    QThread::sleep(10);
    qDebug() << "end sleep=======";
}

void TestWorker::destroy()
{
    workThread->quit();
    //emit SIG_destroy(); //让moveToThread在线程中完成 只能在object所属的线程中调用object的moveToThread函数
}

void TestWorker::SLOT_destroy()
{
    //如果是从外部调用destroy函数则需要让finished信号连接的deleteLater槽函数在workThread线程外执行(不能在线程自己的事件轮询中析构自己)
    //所以要在线程的事件轮询中将object移动到GUI线程后调用workThread的quit函数 这样在线程真正结束后就会在GUI线程中执行析构函数了
    //这样析构比直接在线程外部调用析构函数的好处是不用阻塞调用线程！！！
	this->moveToThread(QApplication::instance()->thread());
	workThread->quit();
}
