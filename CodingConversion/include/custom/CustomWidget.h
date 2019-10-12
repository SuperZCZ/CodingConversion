/*!
 * FileName: CustomWidget.h
 *
 * Author:   ZhangChaoZe
 * Date:     2018-10-18
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 自定义Widget
 */
#ifndef _CUSTOMWIDGET_H_
#define _CUSTOMWIDGET_H_
#include <QWidget>
#include <QPoint>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPushButton>
#include "CustomTitleBar.h"

#include "CharGlobal.h"

class CustomWidget :public QWidget
{
	Q_OBJECT
public:

	//************************************
	// Method:    CustomWidget
	// FullName:  CustomWidget::CustomWidget
	// Access:    public 
	// Returns:   
	// Qualifier:
	// Parameter: QWidget * parent
	// Parameter: bool mouse_scale 是否支持鼠标缩放 默认关闭 一般来说只有主窗体需要
	//************************************
	CustomWidget(QWidget *parent = NULL, bool mouse_scale = false);

	virtual ~CustomWidget();

	void setEnableMouseScale(bool flag = true)
	{
		enableMouseScale = flag;
		if (titleBar!=NULL)
		{
			titleBar->setEnableMouseScale(flag);
		}
	}

	//************************************
	// Method:    createTitleBar
	// FullName:  CustomWidget::createTitleBar
	// Access:    public 
	// Returns:   void
	// Qualifier: 为窗体创建标题栏  
	// 注意：若想创建标题栏 必须在未向该窗体的vAlley中添加任何内容前调用该函数 
	//       目的是为了保证标题栏置顶
	// Parameter: QWidget * releative_widgget 指定关联窗体以便移动关联窗体
	// Parameter: bool enable_icon
	// Parameter: bool enable_titletext
	// Parameter: bool enable_mini
	// Parameter: bool enable_maxnormal
	// Parameter: bool enable_close
	// Parameter: bool enable_mouse_scale 使能鼠标在标题栏点击缩放窗体 默认：关
	//************************************
	void createTitleBar(QWidget *releative_widgget,
		bool enable_icon = true,
		bool enable_titletext = true,
		bool enable_mini = true,
		bool enable_maxnormal = true,
		bool enable_close = true,
		bool enable_mouse_scale = false);

	CustomTitleBar* widgetTitleBar()
	{
		return titleBar;
	}


	//************************************
	// Method:    autoResizeMoveToCenter
	// FullName:  CustomWidget::autoResizeMoveToCenter
	// Access:    public 
	// Returns:   void
	// Qualifier: 自动大小并将窗体移动到默认屏幕中央
	// Parameter: double w_percent 宽相对屏幕的缩放比
	// Parameter: double h_percent 高相对屏幕的缩放比
	//************************************
	void autoResizeMoveToDesktopCenter(double w_percent=0.417,double h_percent=0.484);

	//************************************
	// Method:    resizeMoveToDesktopCenter
	// FullName:  CustomWidget::resizeMoveToDesktopCenter
	// Access:    public 
	// Returns:   void
	// Qualifier: 设置窗体大小并将窗体移动到屏幕中央
	// Parameter: int width_size
	// Parameter: int height_size
	//************************************
	void resizeMoveToDesktopCenter(int width_size,int height_size);

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

	Direction dir;
	QCursor nowCursor;     //记录当前鼠标形状 用于缩放完恢复鼠标形状
	QPoint beforPoint;     //记录鼠标之前的位置 和当前位置作差 获取缩放大小
	bool isLeftPressDown;  //左键是否按下
	bool enableMouseScale;

	static const int Padding = MOUSE_TRACK_PADDING; //可唤醒缩放鼠标的内边距大小 

	//************************************
	// Method:    reSetCursor
	// FullName:  CustomWidget::reSetCursor
	// Access:    private 
	// Returns:   void
	// Qualifier: 根据鼠标在窗体的位置改变鼠标形状
	// Parameter: const QPoint &
	//************************************
	void reSetCursor(const QPoint &currentPoint);
};


class PainterWidget :public QWidget
{
	Q_OBJECT
public:
	PainterWidget(QWidget *parent = NULL) :QWidget(parent) {}
	virtual ~PainterWidget() {}
protected:
	virtual void paintEvent(QPaintEvent *event);
public:
	void loadStyleSheet(const QString &res_url);
};
#endif
