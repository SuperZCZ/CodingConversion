/*!
 * FileName: CustomDialog.h
 *
 * Author:   ZhangChaoZe
 * Date:     2018-10-23
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 自定义Dialog
 */


#ifndef _CUSTOM_DIALOG_H_
#define _CUSTOM_DIALOG_H_

#include <QDialog>
#include "CustomTitleBar.h"
#include "CharGlobal.h"

class CustomDialog :public QDialog
{
	Q_OBJECT
public:

	//************************************
	// Method:    CustomDialog
	// FullName:  CustomDialog::CustomDialog
	// Access:    public 
	// Returns:   
	// Qualifier:
	// Parameter: QWidget * parent
	// Parameter: bool mouse_scale 是否允许鼠标缩放
	// Parameter: Qt::WindowFlags f
	//************************************
	CustomDialog(QWidget *parent = NULL,bool mouse_scale = false);
	virtual ~CustomDialog();

	void setEnableMouseScale(bool flag = true)
	{
		enableMouseScale = flag;
		if (titleBar != NULL)
		{
			titleBar->setEnableMouseScale(flag);
		}
	}

	//************************************
	// Method:    createTitleBar
	// FullName:  CustomDialog::createTitleBar
	// Access:    public 
	// Returns:   void
    // Qualifier: 为窗体创建标题栏
	// 注意：若想创建标题栏 必须在未向该窗体的vAlley中添加任何内容前调用该函数 
	//       目的是为了保证标题栏置顶
	// Parameter: QWidget * releative_widgget
	// Parameter: bool enable_icon
	// Parameter: bool enable_titletext
	// Parameter: bool enable_mini
	// Parameter: bool enable_maxnormal
	// Parameter: bool enable_close
	// Parameter: bool enable_mouse_scale 使能鼠标在标题栏点击缩放窗体 默认：关
	//************************************ 
	void createTitleBar(QWidget *releative_widget,
		bool enable_icon = true,
		bool enable_titletext = true,
		bool enable_mini = true,
		bool enable_maxnormal = true,
		bool enable_close = true,
		bool enable_mouse_scale = false);


	//************************************
	// Method:    autoResizeMoveToCenter
	// FullName:  CustomWidget::autoResizeMoveToCenter
	// Access:    public 
	// Returns:   void
	// Qualifier: 自动大小并将窗体移动到默认屏幕中央
	// Parameter: double w_percent 宽相对屏幕的缩放比
	// Parameter: double h_percent 高相对屏幕的缩放比
	//************************************
	void autoResizeMoveToDesktopCenter(double w_percent = 0.417, double h_percent = 0.484);

	//************************************
	// Method:    resizeMoveToDesktopCenter
	// FullName:  CustomWidget::resizeMoveToDesktopCenter
	// Access:    public 
	// Returns:   void
	// Qualifier: 设置窗体大小并将窗体移动到屏幕中央
	// Parameter: int width_size
	// Parameter: int height_size
	//************************************
	void resizeMoveToDesktopCenter(int width_size, int height_size);

protected:
	virtual  void paintEvent(QPaintEvent *event);
	virtual	void resizeEvent(QResizeEvent *event);
	virtual	void mouseMoveEvent(QMouseEvent *);
	virtual	void mousePressEvent(QMouseEvent *);
	virtual	void mouseReleaseEvent(QMouseEvent *);

protected:
	CustomTitleBar *titleBar;
	QVBoxLayout *vAllLay;  //窗体的垂直布局 Layout  请在子类 中对其中添加widget 或其他 layout
private:

	Direction dir;      //记录激活缩放时鼠标位于窗体的哪个边缘区域 UP, DOWN, LEFT, RIGHT, LEFTTOP, LEFTBOTTOM, RIGHTBOTTOM, RIGHTTOP
	QCursor beforScaleCursor;   //记录激活缩放前鼠标形状 用于缩放完恢复鼠标形状
	QPoint scalePressGlobalPos;     //记录激活缩放时鼠标的全局位置 用于计算应改变的大小
	QRect scaleWidgetGeometry;         //记录激活缩放时窗体的大小和相对位置 用于计算缩放后应处于的位置(相对全局或父widget)和大小
	bool isLeftPressDown;       //左键是否按下
	bool enableMouseScale;

	static const int Padding = MOUSE_TRACK_PADDING; //可唤醒缩放鼠标的内边距大小
};

#endif