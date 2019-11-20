/*!
 * FileName: AboutDialog.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-10-29
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 关于dialog
 */


#ifndef _ABOUT_DIALOG_H_
#define _ABOUT_DIALOG_H_

#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>

#include "custom/CustomDialog.h"

class AboutDialog :public CustomDialog
{
	Q_OBJECT
public:
	AboutDialog(QWidget *parent = NULL);
	virtual ~AboutDialog();
protected:
private:
	QHBoxLayout *logo_HLay;
	QHBoxLayout *name_HLay;
	QHBoxLayout *version_HLay;
	QHBoxLayout *dec_text_HLay;
	QHBoxLayout *qtabout_HLay;

	QLabel *logo_label;
	QLabel *name_label;
	QLabel *version_label;
	QTextEdit *dec_text_edit;
	QPushButton *qtabout_butt;
};

#endif