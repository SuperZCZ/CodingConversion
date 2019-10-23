/*!
 * FileName: CustomTooltipsMessage.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-6-24
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 气泡提示窗体
 */
#ifndef _CUSTOM_TOOLTIPS_MESSAGE_H_
#define _CUSTOM_TOOLTIPS_MESSAGE_H_

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QPropertyAnimation>
#include <QPoint>
//弹出气泡信息 2.5s后逐渐透明并自动析构  在气泡上按下鼠标恢复初始状态 释放鼠标从新进入消失过程 双击关闭(戳破)气泡 
class CustomTooltipsMessage :public QWidget
{
	Q_OBJECT
public:
	Q_PROPERTY(double tooltipsOpacity READ getTooltipsOpacity WRITE setTooltipsOpacity)
public:
	//************************************
	// Method:    critical
	// FullName:  CustomTooltipsMessage::critical
	// Access:    public static 
	// Returns:   void
	// Qualifier: 错误信息气泡 2.5s后逐渐透明并自动析构  在气泡上按下鼠标恢复初始状态 释放鼠标从新进入消失过程 双击关闭(戳破)气泡
	// Parameter: const QString & text 信息内容 气泡大小会根据text数量自适应
	// Parameter: const QString & title 默认为空 如果不为空则在 信息内容的上方显示 标题 如果为空则气泡中只有信息内容没有标题
	// Parameter: const QPoint & pos 默认为QPoint(0, 0)-自动将气泡移动到屏幕的中心   如果指定了非QPoint(0, 0)的坐标则气泡会被移动到设定的pos
	//************************************
	static void critical(const QString &text, const QString &title = "", const QPoint &pos = QPoint(0, 0));

	//************************************
	// Method:    information
	// FullName:  CustomTooltipsMessage::information
	// Access:    public static 
	// Returns:   void
	// Qualifier: 提示信息气泡 2.5s后逐渐透明并自动析构  在气泡上按下鼠标恢复初始状态 释放鼠标从新进入消失过程 双击关闭(戳破)气泡
	// Parameter: const QString & text 信息内容 气泡大小会根据text数量自适应
	// Parameter: const QString & title 默认为空 如果不为空则在 信息内容的上方显示 标题 如果为空则气泡中只有信息内容没有标题
	// Parameter: const QPoint & pos 默认为QPoint(0, 0)-自动将气泡移动到屏幕的中心   如果指定了非QPoint(0, 0)的坐标则气泡会被移动到设定的pos
	//************************************
	static void information(const QString &text, const QString &title = "", const QPoint &pos = QPoint(0, 0));


	//************************************
	// Method:    warning
	// FullName:  CustomTooltipsMessage::warning
	// Access:    public static 
	// Returns:   void
	// Qualifier: 警告信息气泡 2.5s后逐渐透明并自动析构  在气泡上按下鼠标恢复初始状态 释放鼠标从新进入消失过程 双击关闭(戳破)气泡
	// Parameter: const QString & text 信息内容 气泡大小会根据text数量自适应
	// Parameter: const QString & title 默认为空 如果不为空则在 信息内容的上方显示 标题 如果为空则气泡中只有信息内容没有标题
	// Parameter: const QPoint & pos 默认为QPoint(0, 0)-自动将气泡移动到屏幕的中心   如果指定了非QPoint(0, 0)的坐标则气泡会被移动到设定的pos
	//************************************
	static void warning(const QString &text, const QString &title = "", const QPoint &pos = QPoint(0, 0));


	//************************************
	// Method:    success
	// FullName:  CustomTooltipsMessage::success
	// Access:    public static 
	// Returns:   void
	// Qualifier: 成功信息气泡 2.5s后逐渐透明并自动析构  在气泡上按下鼠标恢复初始状态 释放鼠标从新进入消失过程 双击关闭(戳破)气泡
	// Parameter: const QString & text 信息内容 气泡大小会根据text数量自适应
	// Parameter: const QString & title 默认为空 如果不为空则在 信息内容的上方显示 标题 如果为空则气泡中只有信息内容没有标题
	// Parameter: const QPoint & pos 默认为QPoint(0, 0)-自动将气泡移动到屏幕的中心   如果指定了非QPoint(0, 0)的坐标则气泡会被移动到设定的pos
	//************************************
	static void success(const QString &text, const QString &title = "", const QPoint &pos = QPoint(0, 0));
	
	~CustomTooltipsMessage();
protected:
	virtual void paintEvent(QPaintEvent *event);
private:
	QHBoxLayout *hAllLay;
	QVBoxLayout *iconVLay;
	QVBoxLayout *textVLay;
	QHBoxLayout *titleHLay;
	QHBoxLayout *msgTextHLay;

	QLabel *iconLabel;
	QLabel *titleLabel;
	QLabel *msgTextLabel;

	QTimer clock_timer;

	double tooltipsOpacity;
	QPropertyAnimation *propertyAnimation;
public slots:
void closeTooltips();
void moveToDesktopCenter();
public:
	double getTooltipsOpacity();
	void setTooltipsOpacity(double opactiy);
protected:
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
private:
	//************************************
	// Method:    CustomTooltipsMessage
	// FullName:  CustomTooltipsMessage::CustomTooltipsMessage
	// Access:    private 
	// Returns:   
	// Qualifier: 弹出气泡信息 2.5s后逐渐透明并自动析构  在气泡上按下鼠标恢复初始状态 释放鼠标从新进入消失过程 双击关闭(戳破)气泡 
	// Parameter: int messageType
	// Parameter: const QString & title
	// Parameter: const QString & text
	// Parameter: QWidget * parent
	//************************************
	CustomTooltipsMessage(int messageType, const QString &title, const QString &text, QWidget *parent = NULL);
private:
	CustomTooltipsMessage(const CustomTooltipsMessage &src); //禁止复制
	CustomTooltipsMessage &operator=(const CustomTooltipsMessage &src); //禁止赋值
};


#endif

