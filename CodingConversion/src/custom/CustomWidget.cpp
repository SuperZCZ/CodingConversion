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
		if ((rect.width() > 1024 && rect.width() <= 1440) && (rect.height() > 768 && rect.height() <= 1024))
		{
			//小屏幕的策略
			w_percent = 0.44;
			h_percent = 0.52;
		}
		else if ((rect.width() > 1440 && rect.width() <= 2000) && (rect.height() > 1024 && rect.height() <= 1100))
		{
			w_percent = 0.45;
			h_percent = 0.50;
		}
		else if ((rect.width() > 2000 && rect.width() <= 3400) && (rect.height() > 1100 && rect.height() <= 2000))
		{
			w_percent = 0.39;
			h_percent = 0.43;
		}
	}
	//否则直接使用设定的值

	resize_width = rect.width()*w_percent;
	resize_height = rect.height()*h_percent;

	//qDebug() << " w_p " << w_percent << " h_p " << h_percent << "   " << rect << " w " << resize_width << " h " << resize_height;

	resize(resize_width, resize_height);

	move(rect.center()-this->rect().center());   //move 是从默认屏幕的起始点移动  扩展屏按扩展坐标算
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

	move(rect.center() - this->rect().center());   //move 是从默认屏幕的起始点移动  扩展屏按扩展坐标算
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
			if ((this->windowState() & Qt::WindowMaximized) == 0)
			{
				//窗体不是最大化状态时才可缩放
				isLeftPressDown = true;
				beforScaleCursor = cursor();  //记住激活缩放前的光标形状
				dir = CustomTitleBar::getCursorPosition(this, event->pos(), Padding);  //计算鼠标位于可缩放窗体的位置 是否位于可激活缩放的边角
				if (dir != NONE)
				{
					//位于可激活缩放的边角 开始缩放模式
					scalePressGlobalPos = event->globalPos(); //记住激活缩放时鼠标的全局位置
					scaleWidgetGeometry = this->geometry();  //记住激活缩放前的位置和大小

				}
			}
			else
			{
				isLeftPressDown = false;
				dir = NONE;
			}
			break;
		default:
			break;
		}
	}
	QWidget::mousePressEvent(event);
}

void CustomWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (enableMouseScale && dir != NONE && isLeftPressDown)
	{
		int width_inc, height_inc;
		QPoint nowPos= event->globalPos();

		width_inc = nowPos.x() - scalePressGlobalPos.x();
		height_inc = nowPos.y() - scalePressGlobalPos.y();
		//qDebug() << "width inc " << width_inc;
		//qDebug() << "heigt inc" << height_inc;

		int reverse_x = 0, reverse_y = 0;  //缩放的系数
		int moveX_flag = 0, moveY_flag = 0; //移动的系数

		switch (dir) {
		case LEFTTOP:
			reverse_x = -1;
			reverse_y = -1;
			moveX_flag = 1;
			moveY_flag = 1;
			break;
		case RIGHTTOP:
			reverse_x = 1;
			reverse_y = -1;
			moveX_flag = 0;
			moveY_flag = 1;
			break;
		case RIGHTBOTTOM:
			reverse_x = 1;
			reverse_y = 1;
			moveX_flag = 0;
			moveY_flag = 0;
			break;
		case LEFTBOTTOM:
			reverse_x = -1;
			reverse_y = 1;
			moveX_flag = 1;
			moveY_flag = 0;
			break;
		case UP:
			reverse_x = 0;
			reverse_y = -1;
			moveX_flag = 0;
			moveY_flag = 1;
			break;
		case RIGHT:
			reverse_x = 1;
			reverse_y = 0;
			moveX_flag = 0;
			moveY_flag = 0;
			break;
		case DOWN:
			reverse_x = 0;
			reverse_y = 1;
			moveX_flag = 0;
			moveY_flag = 0;
			break;
		case LEFT:
			reverse_x = -1;
			reverse_y = 0;
			moveX_flag = 1;
			moveY_flag = 0;
			break;
		default:
			break;
		}

		if (dir >= UP && dir <= RIGHTTOP)
		{
			/*QPoint move_to_globa_pos(scaleWidgetGeometry.x() + (moveX_flag * width_inc), scaleWidgetGeometry.y() + (moveY_flag * height_inc));

			if (this->parentWidget() == NULL)
			{
				//没有父窗体直接移动到全局的指定位置
				move(move_to_globa_pos);
			}
			else
			{
				//有父窗体则移动到相对父窗体的位置
				move(this->parentWidget()->mapFromGlobal(move_to_globa_pos));
			}

			resize(scaleWidgetGeometry.width() + (reverse_x * width_inc),
				scaleWidgetGeometry.height() + (reverse_y * height_inc));*/
			setGeometry(
				scaleWidgetGeometry.x() + (moveX_flag * width_inc),
				scaleWidgetGeometry.y() + (moveY_flag * height_inc),
				scaleWidgetGeometry.width() + (reverse_x * width_inc),
				scaleWidgetGeometry.height() + (reverse_y * height_inc));

		}
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
				this->setCursor(beforScaleCursor.shape()); //松开鼠标恢复形状
			}
		}
	}
	QWidget::mouseReleaseEvent(event);
}


//*********************************************privaet*********************************


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
