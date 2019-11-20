/*!
 * FileName: AboutDialog.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2019-10-29
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 关于dialog
 */
#include <QApplication>
#include <QDebug>

#include "CharGlobal.h"
#include "dialog/AboutDialog.h"


AboutDialog::AboutDialog(QWidget *parent) :CustomDialog(parent)
{
	setAttribute(Qt::WA_DeleteOnClose);
	createTitleBar(this, true, true, true, false, true, false);
	titleBar->setTitleText(trUtf8("关于 CodingConversion "));
	setWindowIcon(QIcon(":/images/logo.png"));

	logo_HLay = new QHBoxLayout;
	name_HLay = new QHBoxLayout;
	version_HLay = new QHBoxLayout;
	dec_text_HLay = new QHBoxLayout;
	qtabout_HLay = new QHBoxLayout;

	logo_label = new QLabel();
	name_label = new QLabel(trUtf8("CodingConversion"));
	version_label = new QLabel(trUtf8("Version: %1").arg(PRODUCT_VERSION));
	dec_text_edit = new QTextEdit;
	qtabout_butt = new QPushButton(trUtf8("Based on Qt"));

	logo_HLay->setAlignment(Qt::AlignCenter);
	logo_HLay->addWidget(logo_label);

	name_HLay->setAlignment(Qt::AlignCenter);
	name_HLay->addWidget(name_label);

	version_HLay->setAlignment(Qt::AlignCenter);
	version_HLay->addWidget(version_label);

	dec_text_HLay->setAlignment(Qt::AlignCenter);
	dec_text_HLay->addWidget(dec_text_edit);

	qtabout_HLay->setAlignment(Qt::AlignCenter);
	qtabout_HLay->addWidget(qtabout_butt);


	dec_text_edit->setAlignment(Qt::AlignHCenter);
	dec_text_edit->document()->setDefaultTextOption(QTextOption(Qt::AlignCenter));
	dec_text_edit->setText(trUtf8("- - - 简介- - -\n\n本工具可用于文件编码和换行方式的批量转换，支持大多常规编码。"));
	dec_text_edit->setReadOnly(true);

	vAllLay->setSpacing(10);
	vAllLay->addLayout(logo_HLay);
	vAllLay->addLayout(name_HLay);
	vAllLay->addLayout(version_HLay);
	vAllLay->addLayout(dec_text_HLay);
	vAllLay->addLayout(qtabout_HLay);
	vAllLay->addStretch(1);

	this->setContentsMargins(1, 1, 1, 1);

	setObjectName("AboutDialog");

	logo_label->setObjectName("logoLabel");
	name_label->setObjectName("nameLabel");
	dec_text_edit->setObjectName("decTextEdit");
	qtabout_butt->setObjectName("qtAboutButt");

	QFile qss(":/qss/AboutDialog.qss");
	qss.open(QFile::ReadOnly);
	QString qssStr = qss.readAll();
	qss.close();

	setStyleSheet(qssStr);
	setStyle(QApplication::style());  //这句来更新样式  使当前的

	resizeMoveToDesktopCenter(400, 350);

	connect(qtabout_butt, SIGNAL(clicked()), QApplication::instance(), SLOT(aboutQt()));
}

AboutDialog::~AboutDialog()
{
	//qDebug() << "deltet about dialog";
}
