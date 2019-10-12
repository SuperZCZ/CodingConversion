/*!
 * FileName: CustomWidget.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2018-10-18
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 
 */


#include <QStyleOption>
#include <QPainter>
#include <QDesktopWidget>
#include <QApplication>
#include <QRect>
#include "custom/CustomWidget.h"


//********************************************* public **********************************

CustomWidget::CustomWidget(QWidget *parent /* = NULL */,
	bool mouse_scale /* = false */) :
	QWidget(parent)
{
	dir = NONE;
	isLeftPressDown = false;
	titleBar = NULL;
	enableMouseScale = mouse_scale;

	vAllLay = new QVBoxLayout(this);   //创建窗体的垂直布局 Layout 以便后续添加 titlebar和其他内容


	vAllLay->setContentsMargins(0,0,0,0);
	vAllLay->setSpacing(0);
}


void CustomWidget::createTitleBar(QWidget *releative_widgget,
	bool enable_icon /* = true */,
	bool enable_titletext /* = true */,
	bool enable_mini /* = true */,
	bool enable_maxnormal /* = true */,
	bool enable_close /* = true */,
	bool enable_mouse_scale /* = false */)
{
	//创建时自动将关闭和移动关系窗体都设置成同一个窗体  一般这个窗体的parent 是NULL
	titleBar = new CustomTitleBar(this, releative_widgget, releative_widgget, enable_icon, enable_titletext, enable_mini, enable_maxnormal, enable_close, enable_mouse_scale, true);
	vAllLay->setMenuBar(titleBar);  //使用setMenuBar 令标题栏始终在父窗体的顶部 
	vAllLay->setAlignment(Qt::AlignBottom);   //setAlignment(Qt::AlignBottom)结合setMenuBar 令vAllLay始终充满整个父窗体
}


void CustomWidget::autoResizeMoveToDesktopCenter(double w_percent/*=0.417*/, double h_percent/*=0.484*/)
{
	int resize_width, resize_height;
	QRect rect = QApplication::desktop()->screenGeometry();   //获取默认屏幕的宽高 

	if (w_percent == 0.417 && h_percent == 0.484)
	{
		//如果使用默认参数 则进行自动适配
		if ((rect.width() >= 1024 && rect.width() <= 1440) && (rect.height() >= 768 && rect.height() <= 1024))
		{
			//小屏幕的策略
			w_percent = 0.44;
			h_percent = 0.52;
		}
		else if ((rect.width() >= 1680 && rect.width() <= 2048) && (rect.height() >= 1050 && rect.height() <= 1563))
		{
			w_percent = 0.349;
			h_percent = 0.398;
		}
	}
	//否则直接使用设定的值

	resize_width = rect.width()*w_percent;
	resize_height = rect.height()*h_percent;

	//qDebug() << " w_p " << w_percent << " h_p " << h_percent << "   " << rect << " w " << resize_width << " h " << resize_height;

	resize(resize_width, resize_height);

	move((rect.width() - resize_width) / 2, (rect.height() - resize_height) / 2);   //move 是从默认屏幕的起始点移动  扩展屏按扩展坐标算
}


void CustomWidget::resizeMoveToDesktopCenter(int width_size, int height_size)
{
	int resize_width, resize_height;
	QRect rect = QApplication::desktop()->screenGeometry();   //获取默认屏幕的宽高 


	resize_width = width_size;
	resize_height = height_size;

	if (width_size >= rect.width() || width_size <= 0)
	{
		resize_width = rect.width();
	}

	if (height_size >= rect.height() || height_size <= 0)
	{
		resize_height = rect.height();
	}
	
	resize(resize_width, resize_height);

	move((rect.width() - resize_width) / 2, (rect.height() - resize_height) / 2);   //move 是从默认屏幕的起始点移动  扩展屏按扩展坐标算
}


//*******************************************protected function******************************

/***********************************************************************************/
/* 注意：此处必须重写QWieget 的 paintEvent 事件否则qss对本widget无效               */
/* assistant中写到 ：                                                              */
/* Supports only the background, background-clip and background-origin properties. */
/* If you subclass from QWidget, you need to provide a paintEvent for your custom  */
/* QWidget as below:                                                               */
/***********************************************************************************/
void CustomWidget::paintEvent(QPaintEvent *event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(event);
}

void CustomWidget::resizeEvent(QResizeEvent *event)
{
	if (titleBar!=NULL)
	{
		titleBar->updateDisplay();  //更新标题栏显示   使宽度充满窗体
	}
	QWidget::resizeEvent(event);
}



void CustomWidget::mousePressEvent(QMouseEvent *event)
{
	if (enableMouseScale)
	{
		switch (event->button()) 
		{
		case Qt::LeftButton:
			//qDebug() << "push butt left";
			isLeftPressDown = true;
			reSetCursor(event->pos());
			break;
		default:
			break;
		}
	}
	QWidget::mousePressEvent(event);
}

void CustomWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (enableMouseScale && dir != NONE)
	{
		int width_inc, height_inc;
		QPoint nowPos = event->globalPos();
		width_inc = nowPos.x() - beforPoint.x();
		height_inc = nowPos.y() - beforPoint.y();
		//qDebug() << "width inc " << width_inc;
		//qDebug() << "heigt inc" << height_inc;

		int reverse_x = 0, reverse_y = 0;
		int widget_posX = pos().x(), widget_posY = pos().y();
		int widget_width = width(), widget_height = height();
		int moveX_flag = 0, moveY_flag = 0;
		QPoint base_point(0, 0);

		switch (dir) {
		case LEFTTOP:
			reverse_x = -1;
			reverse_y = -1;
			base_point.setX(widget_posX + widget_width);
			base_point.setY(widget_posY + widget_height);
			moveX_flag = -1;
			moveY_flag = -1;
			break;
		case RIGHTTOP:
			reverse_x = 1;
			reverse_y = -1;
			base_point.setY(widget_posY + widget_height);
			base_point.setX(widget_posX);
			moveX_flag = 0;
			moveY_flag = -1;
			break;
		case RIGHTBOTTOM:
			reverse_x = 1;
			reverse_y = 1;
			base_point.setX(widget_posX);
			base_point.setY(widget_posY);
			moveX_flag = 0;
			moveY_flag = 0;
			break;
		case LEFTBOTTOM:
			reverse_x = -1;
			reverse_y = 1;
			base_point.setX(widget_posX + widget_width);
			base_point.setY(widget_posY);
			moveX_flag = -1;
			moveY_flag = 0;
			break;
		case UP:
			reverse_x = 0;
			reverse_y = -1;
			base_point.setX(widget_posX);
			base_point.setY(widget_posY + widget_height);
			moveX_flag = 0;
			moveY_flag = -1;
			break;
		case RIGHT:
			reverse_x = 1;
			reverse_y = 0;
			base_point.setY(widget_posY);
			base_point.setX(widget_posX);
			moveX_flag = 0;
			moveY_flag = 0;
			break;
		case DOWN:
			reverse_x = 0;
			reverse_y = 1;
			base_point.setY(widget_posY);
			base_point.setX(widget_posX);
			moveX_flag = 0;
			moveY_flag = 0;
			break;
		case LEFT:
			reverse_x = -1;
			reverse_y = 0;
			base_point.setX(widget_posX + widget_width);
			base_point.setY(widget_posY);
			moveX_flag = -1;
			moveY_flag = 0;
			break;
		default:
			break;
		}

		QPoint move_to;

		resize(widget_width + (reverse_x*width_inc), widget_height + (reverse_y*height_inc));

		//改变大小后再移动  直接获取  当前窗体大小
		move_to.setX(base_point.x() + (moveX_flag*width()));
		move_to.setY(base_point.y() + (moveY_flag*height()));
		move(move_to);

		//设置鼠标跟随      闪动  不太好
		//cursor().setPos(base_point.x()+(reverse_x*width()),base_point.y()+(reverse_y*height()));
		beforPoint = nowPos;
	}

	QWidget::mouseMoveEvent(event);
}

void CustomWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (enableMouseScale)
	{
		if (event->button() == Qt::LeftButton) 
		{
			isLeftPressDown = false;
			if (dir != NONE) 
			{
				this->setCursor(nowCursor.shape()); //松开鼠标恢复形状
			}
		}
	}
	QWidget::mouseReleaseEvent(event);
}


//*********************************************privaet**********************************


void CustomWidget::reSetCursor(const QPoint &currentPoint)
{
	// 获取窗体在屏幕上的位置区域，tl为topleft点，rb为rightbottom点
	nowCursor = cursor();

	dir = CustomTitleBar::getCursorPosition(this, currentPoint, Padding);  //根据鼠标在当前窗体的位置 改变鼠标形状

	if (dir != NONE)
	{
		beforPoint = nowCursor.pos();  //记录当前光标全局位置
	}
}


//***********************************************析构函数******************************
CustomWidget::~CustomWidget()
{

}


void PainterWidget::paintEvent(QPaintEvent *event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(event);
}

void PainterWidget::loadStyleSheet(const QString &res_url)
{
	QFile qss(res_url);
	qss.open(QFile::ReadOnly);
	QString qssStr = qss.readAll();
	qss.close();
	setStyleSheet(qssStr);
}
