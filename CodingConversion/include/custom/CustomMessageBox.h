/*!
 * FileName: CustomMessageBox.h
 *
 * Author:   ZhangChaoZe
 * Date:     2018-10-24
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 自定义 messagebox
 */

#ifndef _CUSTOM_MESSAGE_BOX_H_
#define _CUSTOM_MESSAGE_BOX_H_
#include <QMessageBox>

#include "CustomDialog.h"
#include "CharGlobal.h"


class CustomMessageBox :public CustomDialog
{
	Q_OBJECT;
public:
	CustomMessageBox(QWidget *parent = NULL);
	CustomMessageBox(
		int icon, 
		const QString &title, 
		const QString &text, 
		QWidget *parent = NULL,
		QMessageBox::StandardButtons buttons = QMessageBox::NoButton,
		QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
	virtual ~CustomMessageBox();

	static void about(QWidget *parent, const QString &title, const QString &text);
	static QMessageBox::StandardButton critical(QWidget *parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
	static QMessageBox::StandardButton information(QWidget *parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
	static QMessageBox::StandardButton question(QWidget *parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No), QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
	static QMessageBox::StandardButton warning(QWidget *parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
	static QMessageBox::StandardButton success(QWidget *parent, const QString &title, const QString &text, QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

	QMessageBox::StandardButton getButtonClicked()
	{
		return whichClicked;
	}
	public slots:
	void recordClickedButton();
private:
	QHBoxLayout *centHLay;
	QHBoxLayout *bottomHLay;
	QMessageBox::StandardButton whichClicked;
};

#endif

