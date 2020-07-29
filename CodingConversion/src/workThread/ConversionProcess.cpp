/*!
 * FileName: ConversionProcess.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2020-4-9
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 检测线程
 */

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDirIterator>
#include <QApplication>
#include <QMutexLocker>

#include "SignalController.h"
#include "workThread/ConversionProcess.h"
#include "setting/GlobalSetting.h"


ConversionThread::ConversionThread(QObject *parent /*= NULL*/) :QThread(parent)
{
	is_stop = true;
	is_pause = true;

	conversionProcessQueue = NULL;

	connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));

	initConnect();
}

ConversionThread::~ConversionThread()
{
	qDebug() << "delete ConversionThread";
	//析构所有转换线程 
	destroyProcessingThread();
	delete conversionProcessQueue; //析构待处理队列
}

void ConversionThread::initConnect()
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

void ConversionThread::setCandidateQueue(const QVector<QString> &path_list)
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

void ConversionThread::stop()
{
	is_stop = true;
	condMutex.lock();
	runWaitCond.wakeOne();
	condMutex.unlock();
}

void ConversionThread::pause()
{
	is_pause = true;
	condMutex.lock();
	runWaitCond.wakeOne();
	condMutex.unlock();
}

void ConversionThread::start()
{
	if (isRunning()) 
	{
		//线程事件轮询已经开启了则取消暂停 继续检测循环
		is_pause = false;
	}
	else
	{
		//线程事件轮询未开启则启动事件轮询并运行run函数
		is_stop = false;
		is_pause = false;
		conversionProcessQueue = new ConversionProcessQueue(4);
		QThread::start();
		createProcessingThread(4); //创建处理线程线程
	}
	condMutex.lock();
	runWaitCond.wakeOne();
	condMutex.unlock();
}

void ConversionThread::run()
{
	emit startover();
	qDebug() << "Detected thread start";
	QStringList current_candidate; //当前待检测文件列表(用list是为了方便对目录下的所有文件进行检测)
	volatile bool befor_paused = is_pause;
	while (!is_stop)
	{
		condMutex.lock();
		while (!is_stop && (is_pause || (candidateQueue.isEmpty() && current_candidate.isEmpty()))) //在未停止状态下 如果暂停或者 待探测队列和当前待探测列表都为空(能检测的都检测完了)则阻塞线程等待唤醒条件
		{
			if ((befor_paused != is_pause) && (is_pause == true))
			{
				emit paused(); //是被暂停的发出暂停信号
				qDebug() << "Detected thread pause";
				befor_paused = true;
			}
			if (candidateQueue.isEmpty() && current_candidate.isEmpty())
			{
				//待检测的队列已经全部检测结束 向待转换队列末尾插入等转换线程数量的结束标识 以让转换线程终止并退出
				conversionProcessQueue->pushOverFlag();
			}
			runWaitCond.wait(&condMutex); //等待暂停被唤醒 或者有新的待检测路径被加入
			if (befor_paused != is_pause&&is_pause == false)
			{
				//暂停被唤醒
				emit startover();
				qDebug() << "Detected thread startover";
				befor_paused = false;
			}
		}
		if (!is_pause && current_candidate.isEmpty() && !candidateQueue.isEmpty())
		{
			//未暂停且当前待检测列表为空(当前待检测的一批数据已经检测完了)且待检测队列不为空 从待检测队列上拿下一个放置当前待检测列表中
			current_candidate << candidateQueue.dequeue();
		}
		condMutex.unlock();
		if (is_stop)
		{
			break;
		}
		qDebug() << "++++++++++++++++++++++++++++++++++++++++++";

		//循环检测当前待检测列表
		while (!current_candidate.isEmpty() && !is_pause && !is_stop) //都检测完或者需要暂停或者需要停止时终止当前待检测列表遍历
		{
			QString current_path; //当前检测文件的路径  可能是一个用户指定的文件也可能是待检测目录中的一个文件
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
					//插入目录下待检测文件列表到待检测列表的头
					current_candidate = sub_file_path_list + current_candidate;
				}
				continue; //目录项已经根据递归设置被替换为子文件项 继续循环
			}

			//检测current_path 一定是非目录项
			QThread::sleep(1);
			qDebug() << "-------------------------file:" << current_path;
			ConversionProcessQueue::ProcessTask process_task;
			process_task.taskType = ConversionProcessQueue::PROCESS_FILE_PATH_TYPE;
			process_task.taskData = current_path;
			conversionProcessQueue->enqueue(process_task);
		}
	}

	//用户强制终止的处理(用户点击停止按钮后可能待检测队列中还有排队的文件，此时检测线程跳出循环后需要强制终止处理线程)
	//如果是某一个处理线程收到了结束标识(其他线程的下一个任务也一定是结束标识，队列的性质就是这样)，这里再次终止不会有坏的影响
	conversionProcessQueue->stop();

	//wait所有转换线程终止并统计所有转换线程终止
	for (int i = 0; i < processingThreads.size(); i++)
	{
		processingThreads[i]->wait();
	}

	//统计转换结果
	//do something ...

	emit stoped(); //线程停止
	qDebug() << "Detected thread stop";
}

QStringList ConversionThread::getDirSubFileList(const QString &dir_path, bool recursion_dir)
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

void ConversionThread::createProcessingThread(int thread_num)
{
	for (int i = 0; i < thread_num; i++)
	{
		ConversionProcessThread *processThread = new ConversionProcessThread(conversionProcessQueue);
		if (processThread != NULL)
		{
			processingThreads.push_back(processThread);
			processThread->start();
		}
	}
}

void ConversionThread::destroyProcessingThread()
{
	for (int i = 0; i < processingThreads.size(); i++)
	{
		delete processingThreads[i];
	}
	processingThreads.clear();
}

//---------------------------------------转换处理线程-------------------------------------

ConversionProcessThread::ConversionProcessThread(ConversionProcessQueue *processQueue, QObject *parent) :QThread(parent)
{
	conversionProcessQueue = processQueue;

	initConnect();
}

ConversionProcessThread::~ConversionProcessThread()
{
	qDebug() << "delete ConversionProcessThread";
}

void ConversionProcessThread::initConnect()
{
	//处理线程在待处理队列中检测到终止标识(检测线程在处理完所有待检测文件后放入待处理队列)后退出线程事件轮询
	//待处理队列尾部如果有终止标识说明检测线程已经完全做完所有检测任务，处理线程在读到终止标识时说所有转换任务都已完成可以终止整个转换流程了
	ConnectInfo connectInfo[] = {
		{ this,SIGNAL(finished()),signalController,SIGNAL(SIG_stopConversion()),Qt::AutoConnection }, 
	};

	SignalController::setConnectInfo(connectInfo, sizeof(connectInfo) / sizeof(ConnectInfo));
}


void ConversionProcessThread::run()
{
	qDebug() << "Process thread start " << currentThreadId();

	while (true)
	{
		ConversionProcessQueue::ProcessTask process_task = conversionProcessQueue->dequeue();
		if (process_task.taskType == ConversionProcessQueue::PROCESS_FILE_PATH_TYPE)
		{
			QString file_path = process_task.taskData.toString();
			qDebug() << "Processing --------- " << file_path << " ---- " << this->currentThreadId();
			QThread::sleep(2);
		}
		else if (process_task.taskType == ConversionProcessQueue::PROCESS_STOP_THREEAD_TYPE)
		{
			break;
		}
	}
	qDebug() << "Process thread stop";
	//某一个处理线程结束后就会发送停止转换信号迫使检测线程终止 以此终止所有转换线程
}

ConversionProcessQueue::ConversionProcessQueue(int process_thread_num)
{
	is_pause = false;
	processThreadNum = process_thread_num;
}

ConversionProcessQueue::ProcessTask ConversionProcessQueue::dequeue()
{
	bool is_empty;
	ProcessTask res_task;
	res_task.taskType = PROCESS_TASK_NONE_TYPE;

	queueMutex.lock();
	while (is_pause || processQueue.isEmpty()) //如果暂停或者待处理队列为空则使当前线程休眠
	{
		queueWaitCond.wait(&queueMutex);
	}
	QString file_path = processQueue.dequeue();
	if (!file_path.isEmpty())
	{
		if (file_path == "STOP_PROCESSING_THREAD")
		{
			res_task.taskType = PROCESS_STOP_THREEAD_TYPE;
		}
		else
		{
			res_task.taskType = PROCESS_FILE_PATH_TYPE;
			res_task.taskData = file_path;
		}
	}
	queueMutex.unlock();
	return res_task;
}

void ConversionProcessQueue::enqueue(const ProcessTask &task)
{
	if (task.taskType == PROCESS_TASK_NONE_TYPE)
	{
		return;
	}
	QString file_path = task.taskData.toString();
	if (file_path.isEmpty())
	{
		return;
	}
	queueMutex.lock();
	processQueue.enqueue(file_path);
	queueWaitCond.wakeOne();
	queueMutex.unlock();
}

void ConversionProcessQueue::pushOverFlag()
{
	queueMutex.lock();
	for (int i = 0; i < processThreadNum; i++)
	{
		processQueue.enqueue("STOP_PROCESSING_THREAD"); //放入等数量的终止标识符
	}
	queueWaitCond.wakeAll();
	queueMutex.unlock();
}

void ConversionProcessQueue::stop()
{
	queueMutex.lock();
	processQueue.clear(); //清空当前队列
	for (int i = 0; i < processThreadNum; i++)
	{
		processQueue.enqueue("STOP_PROCESSING_THREAD"); //放入等数量的终止标识符
	}
	queueWaitCond.wakeAll();
	queueMutex.unlock();
}

void ConversionProcessQueue::pause()
{
	queueMutex.lock();
	is_pause = true;
	queueWaitCond.wakeAll();
	queueMutex.unlock();
}

void ConversionProcessQueue::startover()
{
	queueMutex.lock();
	is_pause = false;
	queueWaitCond.wakeAll();
	queueMutex.unlock();
}
