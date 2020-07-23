/*!
 * FileName: GlobalSetting.h
 *
 * Author:   ZhangChaoZe
 * Date:     2020-7-22
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 全局设置项
 */

#include <QObject>
#include <QMap>
#include <QMutex>
#include <QVariant>

class GlobalSetting;

extern GlobalSetting *globalSetting;

class GlobalSetting : public QObject
{
	Q_OBJECT
public:
	static GlobalSetting *createInstance();
	static void destroyInstance();

	bool getValue(const QString &key,QVariant &value);
	bool setValue(const QString &key, const QVariant &value);


private:
	QMap<QString, QVariant> settingMap;
	QMutex mapMutex;

private:
	GlobalSetting(QObject *parent = NULL);
	virtual ~GlobalSetting();

	static GlobalSetting *instance;                  //单实例

	GlobalSetting(const GlobalSetting &);            //禁止复制
	GlobalSetting &operator=(const GlobalSetting &); //禁止赋值
};