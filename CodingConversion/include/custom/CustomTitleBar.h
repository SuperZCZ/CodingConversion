/*!
 * FileName: CustomTitleBar.h
 *
 * Author:   ZhangChaoZe
 * Date:     2018-10-18
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 自定义标题栏
 */
#ifndef _CUSTOM_TITLEBAR_H_
#define _CUSTOM_TITLEBAR_H_
#include <QWidget>
#include <QPushButton>
#include <QFile>
#include <QLabel>
#include <QIcon>
#include <QHBoxLayout>
#include <QDebug>
#include <QMouseEvent>
#include <QCursor>
#include <QSize>
#include <QVector>

#include "CharGlobal.h"

#define MOUSE_TRACK_PADDING  6 //窗口边缘鼠标跟踪区域的宽度

enum Direction {
	NONE, UP, DOWN, LEFT, RIGHT, LEFTTOP, LEFTBOTTOM, RIGHTBOTTOM, RIGHTTOP
};

class CustomTitleBar:public QWidget
{
    Q_OBJECT

private:

    QWidget *parentWidget;
	QWidget *moveRelativeWidget;   //移动事件的关系窗体
	QWidget *closeRelativeWidget;  //关闭 最大 最小化

    QHBoxLayout *titleHlay;
    QLabel *iconLabel;
    QLabel *titleLabel;
    QPushButton *miniSizeButt;
    QPushButton *maxNormalSizeButt;
    QPushButton *closeButt;

    bool icon_enable;
    bool title_enable;
    bool mini_enable;
    bool max_normale_enable;
    bool close_enable;
	bool double_click_max_enbale;

    bool is_show_max;

	QPoint beforMouseGlobalPos;
	bool leftMousePressed;
	bool changeShap;
	bool enableMouseScale; //是否允许鼠标缩放
	bool isLayout;           //父窗体是否是layout布局
	QCursor nowCursorShap;

	QVector<QWidget *> relativeMoveWidgets;


protected:
virtual void paintEvent(QPaintEvent *event);
virtual void mousePressEvent(QMouseEvent *event);
virtual void mouseMoveEvent(QMouseEvent *event);
virtual void mouseReleaseEvent(QMouseEvent *event);
virtual void mouseDoubleClickEvent(QMouseEvent *event);
virtual	bool eventFilter(QObject *obj, QEvent *event);
public:

	//************************************
	// Method:    CustomTitleBar
	// FullName:  CustomTitleBar::CustomTitleBar
	// Access:    public 
	// Returns:   
	// Qualifier:
	// Parameter: QWidget * parnet  父窗体必须指定
	// Parameter: QWidget * relative_widget 鼠标点击拖动标题栏后移动的窗体 可以为NULL
	// Parameter: QWidget * close_widget 鼠标点击拖动标题栏最大 最小 关闭 响应的窗体 可以为NULL 
	//            注意若想让relative_widget可移动则必须指定close_widget
	// Parameter: bool enable_icon   使能图标
	// Parameter: bool enable_titletext 使能文字描述
	// Parameter: bool enable_mini     使能最小化
	// Parameter: bool enable_maxnormal  使能最大化正常化
	// Parameter: bool enable_close   使能关闭
	// Parameter: bool enable_mouse_scale   使能鼠标在标题栏缩放关系窗体
	//************************************
	CustomTitleBar(QWidget *parnet, 
		QWidget *relative_widget = NULL, 
		QWidget *close_widget = NULL,
		bool enable_icon = true, 
		bool enable_titletext = true, 
		bool enable_mini = true, 
		bool enable_maxnormal = true, 
		bool enable_close = true,
		bool enable_mouse_scale = false,
		bool is_ayout = false);

    virtual ~CustomTitleBar();

    //************************************
    // Method:    updateDisplay
    // FullName:  CustomTitleBar::updateDisplay
    // Access:    public 
    // Returns:   void
    // Qualifier: 更新窗体部件并设置宽度 与 父窗体相同
    //************************************
    void updateDisplay();

    //************************************
    // Method:    setTitleIcon
    // FullName:  CustomTitleBar::setTitleIcon
    // Access:    public 
    // Returns:   void
    // Qualifier: 为标题栏设置图标  也可以在qss中设置
    // Parameter: const QPixmap & icon
    //************************************
    void setTitleIcon(const QPixmap &icon);

	//************************************
	// Method:    setTitleText
	// FullName:  CustomTitleBar::setTitleText
	// Access:    public 
	// Returns:   void
	// Qualifier: 为标题栏设置提示性文字
	// Parameter: const QString & text
	// Parameter: bool also_window_title 是否同时设置窗名称 默认是
	//************************************
	void setTitleText(const QString &text, bool also_window_title = true);

    void setIconEnabled(bool flag=true)
    {
        icon_enable=flag;
        updateDisplay();
    }
    void setTitleTextEnabled(bool flag=true)
    {
        title_enable=flag;
        updateDisplay();
    }
    void setMiniSizeEnabled(bool flag=true)
    {
        mini_enable=flag;
        updateDisplay();
    }
    void setMaxNormalSizeEnabled(bool flag=true)
    {
        max_normale_enable=flag;
        updateDisplay();
    }
    void setCloseEnabled(bool flag=true)
    {
        close_enable=flag;
        updateDisplay();
    }

	void setEnableMouseScale(bool flag = true)
	{
		enableMouseScale = flag;
	}

	void setEnableDoubleClickMax(bool flag = true)
	{
		double_click_max_enbale = flag;
	}

	//************************************
	// Method:    getCursorPosition
	// FullName:  CustomTitleBar::getCursorPosition
	// Access:    public static 
	// Returns:   Direction
	// Qualifier: 获取currentPoint 在widget的方位
	// Parameter: QWidget * widget 需要查询的窗体
	// Parameter: const QPoint & currentPoint 当前鼠标所在窗体的位置  可以计算得出不一定必须要求鼠标当前在该窗体上点击
	// Parameter: int padding  边距大小
	// Parameter: bool test_shape 测试当前鼠标所在的位置不改变鼠标形状
	//************************************
	static Direction getCursorPosition(QWidget *widget, const QPoint &currentPoint, int padding, bool test_shape = false);
signals:
	void switchMaxNormal();
public slots:
    void changeMaxNormalSize();
	void changeMiniSize();
private:
	void updateMaxNormalState();
};
#endif
