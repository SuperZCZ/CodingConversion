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
#include <QFile>
#include <QFileInfo>
#include <QDirIterator>
#include <QApplication>
#include <QMutexLocker>

#include "SignalController.h"
#include "workThread/ProcessThread.h"
#include "setting/GlobalSetting.h"


DetectThread::DetectThread(QObject *parent /*= NULL*/) :QThread(parent)
{
	is_stop = true;
	is_pause = true;
	connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));

	initConnect();
}

DetectThread::~DetectThread()
{
	qDebug() << "delete DetectThread";
}

void DetectThread::initConnect()
{
	ConnectInfo connectInfo[] = {
		{ this,SIGNAL(startover()),signalController,SIGNAL(SIG_conversionStarted()),Qt::AutoConnection },
		{ this,SIGNAL(stoped()),signalController,SIGNAL(SIG_conversionStoped()),Qt::AutoConnection },
		{ this,SIGNAL(paused()),signalController,SIGNAL(SIG_conversionPaused()),Qt::AutoConnection },
		{ signalController,SIGNAL(SIG_startConversion()),this,SLOT(start()),Qt::AutoConnection },
		{ signalController,SIGNAL(SIG_stopConversion()),this,SLOT(stop()),Qt::AutoConnection },
		{ signalController,SIGNAL(SIG_pauseConversion()),this,SLOT(pause()),Qt::AutoConnection },
	};

	SignalController::setConnectInfo(connectInfo, sizeof(connectInfo) / sizeof(ConnectInfo));
}

void DetectThread::setCandidateQueue(const QVector<QString> &path_list)
{
	condMutex.lock();
	candidateQueue.clear();
	for (int i = 0; i < path_list.size(); i++)
	{
		candidateQueue.enqueue(path_list[i]);
	}
	runWaitCond.wakeOne();
	condMutex.unlock();
}

void DetectThread::stop()
{
	is_stop = true;
	condMutex.lock();
	runWaitCond.wakeOne();
	condMutex.unlock();
}

void DetectThread::pause()
{
	is_pause = true;
	condMutex.lock();
	runWaitCond.wakeOne();
	condMutex.unlock();
}

void DetectThread::start()
{
	if (isRunning()) 
	{
		//线程事件轮询已经开启了则取消暂停 继续处理循环
		is_pause = false;
	}
	else
	{
		//线程事件轮询未开启则启动事件轮询并运行run函数
		is_stop = false;
		is_pause = false;
		QThread::start();
	}
	condMutex.lock();
	runWaitCond.wakeOne();
	condMutex.unlock();
}

void DetectThread::run()
{
	emit startover();
	qDebug() << "Detected thread start";
	QStringList current_candidate; //当前待处理文件列表(用list是为了方便对目录下的所有文件进行处理)
	volatile bool befor_paused = is_pause;
	while (!is_stop)
	{
		condMutex.lock();
		bool candidate_empty = candidateQueue.isEmpty();
		if (!is_pause && current_candidate.isEmpty() && !candidate_empty)
		{
			//未暂停且当前待处理列表为空(当前待处理的一批数据已经处理完了)且待处理队列不为空 从待处理队列上拿下一个放置当前待处理列表中
			current_candidate << candidateQueue.dequeue();
		}
		while (!is_stop && (is_pause || (candidate_empty&&current_candidate.isEmpty()))) //在未停止状态下 如果暂停或者 待处理队列和当前待处理列表都为空(能处理的都处理完了)则阻塞线程等待唤醒条件
		{
			if ((befor_paused != is_pause) && (is_pause == true))
			{
				emit paused(); //是被暂停的发出暂停信号
				qDebug() << "Detected thread pause";
				befor_paused = true;
			}
			runWaitCond.wait(&condMutex); //等待暂停被唤醒 或者有新的待处理路径被加入
			if (befor_paused != is_pause&&is_pause == false)
			{
				//暂停被唤醒
				emit startover();
				qDebug() << "Detected thread startover";
				befor_paused = false;
			}
			if (!is_pause)
			{
				//被唤醒时是非暂停 从待处理队列上拿出一个
				if (!candidateQueue.empty() && current_candidate.isEmpty())
				{
					current_candidate << candidateQueue.dequeue();
				}
			}
		}
		condMutex.unlock();
		if (is_stop)
		{
			break;
		}
		qDebug() << "++++++++++++++++++++++++++++++++++++++++++";

		//循环处理当前待处理列表
		while (!current_candidate.isEmpty() && !is_pause && !is_stop) //都处理完或者需要暂停或者需要停止时终止当前待处理列表遍历
		{
			QString current_path; //当前处理文件的路径  可能是一个用户指定的文件也可能是待处理目录中的一个文件
			current_path = current_candidate.first();
			current_candidate.pop_front();

			if (current_path.isEmpty())
			{
				continue;
			}
			QFileInfo file_info(current_path);
			if (!file_info.exists())
			{
				continue;
			}

			if (file_info.isDir())
			{
				//目录的话递归获取目录下所有文件和子孙目录下的所有文件列表
				//不递归的话只回去目录下所有文件不包括子目录和孙子目录
				QVariant val;
				globalSetting->getValue("recursionDir", val);
				bool recursion_dir = val.toBool();
				QStringList sub_file_path_list = getDirSubFileList(current_path, recursion_dir);
				if (!sub_file_path_list.isEmpty())
				{
					//插入目录下待处理文件列表到待处理列表的头
					current_candidate = sub_file_path_list + current_candidate;
				}
				continue; //目录项已经根据递归设置被替换为子文件项 继续循环
			}

			//处理current_path 一定是非目录项
			QThread::sleep(2);
			qDebug() << "-------------------------file:" << current_path;
		}
	}
	emit stoped(); //线程停止
	qDebug() << "Detected thread stop";
}

QStringList DetectThread::getDirSubFileList(const QString &dir_path, bool recursion_dir)
{
	QStringList file_list;
	QDirIterator::IteratorFlags flags = QDirIterator::FollowSymlinks;
	if (recursion_dir)
	{
		flags |= QDirIterator::Subdirectories;
	}
	QDirIterator dir_iter(dir_path, flags);
	while (dir_iter.hasNext())
	{
		QString file_path = dir_iter.next();
		QFileInfo file_info(file_path);
		if (file_info.isDir())
		{
			continue;
		}
		file_list << file_path;
	}
	return file_list;
}
