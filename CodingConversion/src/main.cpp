#include <QApplication>
#include <QResource>
#include <QDebug>
#include <QDir>
#include <QString>

#include "CharGlobal.h"
#include "SignalController.h"
#include "mainWindow/MainWindow.h"
#include "charDetect/CharDetect.h"

SignalController* signalController = NULL;

bool loadResources(const QString& theme)
{
	QString rcc_path = QApplication::applicationDirPath();
	rcc_path.append("/Resources/");
	rcc_path.append(theme);
	return QResource::registerResource(rcc_path);
}

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	if (!loadResources("BlueTheme.rcc")) //加载资源文件
	{
		qDebug() << "Load Resources Error!";
		return 1;
	}

	signalController = SignalController::createSignalController(); //创建信号控制器
	if (signalController == NULL)
	{
		qDebug() << "Create Signal Controller failed";
		return 1;
	}


	MainWindow mainWindow;
	mainWindow.show();
	mainWindow.autoResizeMoveToDesktopCenter();

	/*chardet::TextCodecInfo char_a;

	QString dir_path = "F:/tmp";
	QDir dir(dir_path);

	QFileInfoList file_infos = dir.entryInfoList();

	for (int i = 0; i < file_infos.size(); i++)
	{
		char_a = chardet::CharDetect::charDetect(file_infos[i].filePath());

		qDebug() << file_infos[i].filePath() << " encoding ---------------" << char_a.codec << "  BOM " << char_a.bom_info.has_bom <<  "  " << char_a.bom_info.endian;
	}*/
	return app.exec();
}