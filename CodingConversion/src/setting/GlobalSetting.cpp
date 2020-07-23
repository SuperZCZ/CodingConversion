/*!
 * FileName: GlobalSetting.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2020-7-22
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 全局设置项
 */


#include "setting/GlobalSetting.h"

GlobalSetting *globalSetting = NULL;
GlobalSetting *GlobalSetting::instance = NULL;

GlobalSetting::GlobalSetting(QObject *parent /*= NULL*/) :QObject(parent)
{

}

GlobalSetting::~GlobalSetting()
{

}

GlobalSetting * GlobalSetting::createInstance()
{
	if (instance == NULL)
	{
		instance = new GlobalSetting;
		globalSetting = instance;
	}
	return instance;
}

void GlobalSetting::destroyInstance()
{
	GlobalSetting *tmp_ptr = instance;
	globalSetting = NULL;
	instance = NULL;
	if (tmp_ptr != NULL)
	{
		delete tmp_ptr;
	}
}

bool GlobalSetting::getValue(const QString &key, QVariant &value)
{
	bool res = false;
	value.clear();
	if (!key.isEmpty())
	{
		QMutexLocker locker(&mapMutex);
		QMap<QString, QVariant>::iterator iter = settingMap.find(key);
		if (iter != settingMap.end())
		{
			value = iter.value();
			res = true;
		}
	}
	return res;
}

bool GlobalSetting::setValue(const QString &key, const QVariant &value)
{
	bool res = false;
	if (!key.isEmpty())
	{
		QMutexLocker locker(&mapMutex);
		settingMap[key] = value;
		res = true;
	}
	return res;
}

