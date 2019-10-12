/*!
 * FileName: SignalController.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-9-14
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 信号-槽转发器
 */

#include "SignalController.h"

SignalController* SignalController::instances = NULL;

void SignalController::setConnectInfo(struct ConnectInfo* connectList, int item_num)
{
	int i = 0;
	for (i = 0; i < item_num; i++)
	{
		connect(connectList[i].sender, connectList[i].signal_str, connectList[i].reciever, connectList[i].sig_slot_str, connectList[i].connect_type);
	}
}

void SignalController::registerMetaALLType()
{

}
