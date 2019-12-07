/*!
 * FileName: CustomTitleBar.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2018-10-18
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 
 */


#include <QObject>
#include <QDebug>
#include <QStyleOption>
#include <QPainter>
#include <QApplication>

#include "custom/CustomWidget.h"
#include "custom/CustomTitleBar.h"


Direction CustomTitleBar::getCursorPosition(QWidget *widget, const QPoint &currentPoint, int padding,bool test_shape)
{

	Direction dir = NONE;
	int width, height;
	width = widget->width();
	height = widget->height();

	int x = currentPoint.x();
	int y = currentPoint.y();
	if (x <= padding&& y <= padding)
	{
		// 左上角
		dir = LEFTTOP;
		if (!test_shape)
		{
			widget->setCursor(QCursor(Qt::SizeFDiagCursor));  // 设置鼠标形状
		}
	}
	else if (x >= width - padding && x <= width && y >= height - padding && y <= height)
	{
		// 右下角
		dir = RIGHTBOTTOM;
		if (!test_shape)
		{
			widget->setCursor(QCursor(Qt::SizeFDiagCursor));
		}
	}
	else if (x <= padding && y >= height - padding && y <= height)
	{
		//左下角
		dir = LEFTBOTTOM;
		if (!test_shape)
		{
			widget->setCursor(QCursor(Qt::SizeBDiagCursor));
		}
	}
	else if (x >= width - padding && x <= width && y <= padding)
	{
		// 右上角
		dir = RIGHTTOP;
		if (!test_shape)
		{
			widget->setCursor(QCursor(Qt::SizeBDiagCursor));
		}
	}
	else if (x < padding && y > padding && y < height - padding)
	{
		// 左边
		dir = LEFT;
		if (!test_shape)
		{
			widget->setCursor(QCursor(Qt::SizeHorCursor));
		}
	}
	else if (x > width - padding && x < width &&  y > padding && y < height - padding)
	{
		// 右边
		dir = RIGHT;
		if (!test_shape)
		{
			widget->setCursor(QCursor(Qt::SizeHorCursor));
		}
	}
	else if (y <padding && x > padding  && x < width - padding)
	{
		// 上边
		dir = UP;
		if (!test_shape)
		{
			widget->setCursor(QCursor(Qt::SizeVerCursor));
		}
	}
	else if (y > height - padding && y < height  && x > padding  && x < width - padding)
	{
		// 下边
		dir = DOWN;
		if (!test_shape)
		{
			widget->setCursor(QCursor(Qt::SizeVerCursor));
		}
	}
	else
	{
		// 默认
		dir = NONE;
	}

	return dir;
}


CustomTitleBar::CustomTitleBar(QWidget *parnet,
	QWidget *relative_widget,
	QWidget *close_widget,
	bool enable_icon,
	bool enable_titletext,
	bool enable_mini,
	bool enable_maxnormal,
	bool enable_close,
	bool enable_mouse_scale,
	bool is_layout) :
	QWidget(parnet),
	icon_enable(enable_icon),
	title_enable(enable_titletext),
	mini_enable(enable_mini),
	max_normale_enable(enable_maxnormal),
	close_enable(enable_close),
	enableMouseScale(enable_mouse_scale),
	isLayout(is_layout)
{
	setObjectName("CustomTitleBar");

	moveRelativeWidget = relative_widget;
	closeRelativeWidget = close_widget;
	leftMousePressed = false;
	changeShap = false;
	double_click_max_enbale = false;

	titleHlay = new QHBoxLayout(this);
	iconLabel = new QLabel(this);
	titleLabel = new QLabel(this);
	miniSizeButt = new QPushButton(this);
	maxNormalSizeButt = new QPushButton(this);
	closeButt = new QPushButton(this);

	maxNormalSizeButt->setCheckable(true);

	iconLabel->setObjectName("iconLabel");
	titleLabel->setObjectName("titleLabel");
	miniSizeButt->setObjectName("miniSizeButt");
	maxNormalSizeButt->setObjectName("maxNormalSizeButt");
	closeButt->setObjectName("closeButt");


	titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);  //靠左并上下居中

    titleHlay->setMargin(0);
    titleHlay->setSpacing(0);

	titleHlay->addWidget(iconLabel, 0, Qt::AlignLeft);
	titleHlay->addWidget(titleLabel, 0, Qt::AlignLeft);
    titleHlay->addStretch(1);
	titleHlay->addWidget(miniSizeButt, 0, Qt::AlignRight);
	titleHlay->addWidget(maxNormalSizeButt, 0, Qt::AlignRight);
	titleHlay->addWidget(closeButt, 0, Qt::AlignRight);



    //加载qss在创建该标题栏的父窗体中进行 可以为标题栏指定不同的样式和内容
   /*QFile qss(":/res/qss/CustomTitleBar.qss");
    qss.open(QFile::ReadOnly);
    QString qssStr=qss.readAll();
    qss.close();
    setStyleSheet(qssStr);*/



    updateDisplay();
    if(moveRelativeWidget!=NULL)
    {
		Qt::WindowFlags wFlags = moveRelativeWidget->windowFlags();  //获取窗口类型  
		wFlags = wFlags & Qt::WindowType_Mask;               //提取窗口类型
		wFlags = wFlags | Qt::CustomizeWindowHint;         //隐藏标题栏
		wFlags = wFlags | Qt::FramelessWindowHint;
        moveRelativeWidget->setWindowFlags(wFlags);  //设置关系窗体隐藏标题栏
    }
	if (closeRelativeWidget != NULL)
	{
		Qt::WindowFlags wFlags = closeRelativeWidget->windowFlags();  //获取窗口类型  
		wFlags = wFlags & Qt::WindowType_Mask;               //提取窗口类型
		wFlags = wFlags | Qt::CustomizeWindowHint;         //隐藏标题栏
		wFlags = wFlags | Qt::FramelessWindowHint;
		closeRelativeWidget->setWindowFlags(wFlags);  //设置关闭关系窗体隐藏标题栏
													  //qDebug() << wFlags << "after ----- " << relativeWidget->windowFlags();;
		updateMaxNormalState();

		if (mini_enable || max_normale_enable)
		{
			closeRelativeWidget->installEventFilter(this);
		}

		connect(miniSizeButt, SIGNAL(clicked(bool)), this, SLOT(changeMiniSize()));
		connect(closeButt, SIGNAL(clicked(bool)), closeRelativeWidget, SLOT(close()));
		connect(maxNormalSizeButt, SIGNAL(clicked(bool)), this, SLOT(changeMaxNormalSize()));
	}
}

void CustomTitleBar::changeMaxNormalSize()
{
    if(closeRelativeWidget!=NULL)
    {
		closeRelativeWidget->setWindowState(closeRelativeWidget->windowState() ^ Qt::WindowMaximized); //按位异或
    }
}



void CustomTitleBar::changeMiniSize()
{
	if (closeRelativeWidget != NULL)
	{
		closeRelativeWidget->setWindowState(closeRelativeWidget->windowState() ^ Qt::WindowMinimized); 
	}
}

void CustomTitleBar::updateMaxNormalState()
{
	if (closeRelativeWidget->windowState() & Qt::WindowMaximized)
	{
		is_show_max = true;
	}
	else
	{
		is_show_max = false;
	}
	maxNormalSizeButt->setChecked(is_show_max);
}

void CustomTitleBar::setTitleIcon(const QPixmap &icon)
{
    iconLabel->setScaledContents(true);   //设置内容自适应缩放
    iconLabel->setPixmap(icon);
	if (closeRelativeWidget != NULL)
	{
		closeRelativeWidget->setWindowIcon(QIcon(icon));
	}
}

void CustomTitleBar::setTitleText(const QString &text, bool also_window_title)
{
    titleLabel->setText(text);
	if (closeRelativeWidget != NULL&&also_window_title)
	{
		closeRelativeWidget->setWindowTitle(text);
	}
}


/***********************************************************************************/
/* 注意：此处必须重写QWieget 的 paintEvent 事件否则qss对本widget无效               */
/* assistant中写到 ：                                                              */
/* Supports only the background, background-clip and background-origin properties. */
/* If you subclass from QWidget, you need to provide a paintEvent for your custom  */
/* QWidget as below:                                                               */ 
/***********************************************************************************/
void CustomTitleBar::paintEvent(QPaintEvent *event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(event);
}

void CustomTitleBar::updateDisplay()
{
    icon_enable==true? iconLabel->show():iconLabel->hide();
    title_enable==true? titleLabel->show():titleLabel->hide();
    mini_enable==true? miniSizeButt->show():miniSizeButt->hide();
    max_normale_enable==true? maxNormalSizeButt->show():maxNormalSizeButt->hide();
    close_enable==true? closeButt->show():closeButt->hide();
}



//*********************************************protected function********************************************
void CustomTitleBar::mousePressEvent(QMouseEvent *event)
{
	if ((moveRelativeWidget != NULL || closeRelativeWidget != NULL) && event->button() == Qt::LeftButton)
	{
		int dir_type = 0;

		//缩放优先级高于移动优先级 先检测是否位于激活缩放的区域
		if (enableMouseScale && closeRelativeWidget != NULL)
		{
			//允许缩放且关闭窗体不为NULL且不是最大化状态 检测按下点是否位于激活缩放的区域
			if ((closeRelativeWidget->windowState() & Qt::WindowMaximized) == 0)
			{
				QPoint close_press_pos = closeRelativeWidget->mapFromGlobal(event->globalPos());
				int close_dir_type = getCursorPosition(closeRelativeWidget, close_press_pos, MOUSE_TRACK_PADDING, true);
				if (close_dir_type != 0)
				{
					//位于可以激活缩放的区域
					dir_type = close_dir_type;
				}
			}
		}
		if (dir_type == 0)
		{
			//不位于激活缩放的区域 检测移动窗体是否存在
			if (moveRelativeWidget != NULL)
			{
				if ((moveRelativeWidget->windowState() & Qt::WindowMaximized) == 0)
				{
					//如果可移动窗体不是处于最大化才可移动
					movePressPos = moveRelativeWidget->mapFromGlobal(event->globalPos()); //获取相对移动窗体的点击位置 后续移动保证鼠标相对移动窗体的位置始终等于这个值
					leftMousePressed = true;  //开启移动
					nowCursorShap = cursor(); //保存之前光标样式
					this->setCursor(Qt::ClosedHandCursor); //抓住窗体
				}
				else
				{
					leftMousePressed = false;
				}
			}
			if (closeRelativeWidget != NULL && closeRelativeWidget != moveRelativeWidget)
			{
				if ((closeRelativeWidget->windowState() & Qt::WindowMaximized) == 0)
				{
					//关闭窗体不是最大化状态时才可移动
					closePressPos = closeRelativeWidget->mapFromGlobal(event->globalPos());
					if (moveRelativeWidget == NULL)
					{
						leftMousePressed = true; //开启移动
						nowCursorShap = cursor(); //保存之前光标样式
						this->setCursor(Qt::ClosedHandCursor); //抓住窗体
					}
				}
				else
				{
					leftMousePressed = false;
				}
			}
		}
		else
		{
			//位于激活缩放的区域 切换光标显示
			nowCursorShap = cursor();
			switch (dir_type)
			{
			case UP:
				this->setCursor(QCursor(Qt::SizeVerCursor));
				break;
			case DOWN:
				this->setCursor(QCursor(Qt::SizeVerCursor));
				break;
			case LEFT:
				this->setCursor(QCursor(Qt::SizeHorCursor));
				break;
			case RIGHT:
				this->setCursor(QCursor(Qt::SizeHorCursor));
				break;
			case LEFTTOP:
				this->setCursor(QCursor(Qt::SizeFDiagCursor));  // 设置鼠标形状
				break;
			case LEFTBOTTOM:
				this->setCursor(QCursor(Qt::SizeBDiagCursor));
				break;
			case RIGHTBOTTOM:
				this->setCursor(QCursor(Qt::SizeFDiagCursor));
				break;
			case RIGHTTOP:
				this->setCursor(QCursor(Qt::SizeBDiagCursor));
				break;
			default:
				break;
			}
			changeShap = true;
		}
	}
	QWidget::mousePressEvent(event);
}

void CustomTitleBar::mouseMoveEvent(QMouseEvent *event)
{
	if (leftMousePressed && moveRelativeWidget != NULL)
	{
		QPoint global_move_to; //应移动到位于全局的位置
		global_move_to = event->globalPos() - movePressPos; //全局目的位置=鼠标当前全局位置-开始移动时鼠标相对移动窗体位置  即  鼠标全局位置=全局目的位置+开始移动时鼠标相对移动窗体的位置
		//qDebug() << global_move_to;
		moveRelativeWidget->move(global_move_to);

		if (closeRelativeWidget != NULL && closeRelativeWidget != moveRelativeWidget)
		{
			//关闭窗体也需要被移动
			QPoint close_global_move_to = event->globalPos() - closePressPos;
			closeRelativeWidget->move(close_global_move_to);
		}

		//这里后续就可以根据鼠标全局位置 判断被移动窗体是否需要自动充满屏幕、自动充满坐半屏、自动充满右半屏、自动充满上下左右1/4屏幕 以后再实现
	}
	QWidget::mouseMoveEvent(event);
}


void CustomTitleBar::mouseReleaseEvent(QMouseEvent *event)
{
	if (moveRelativeWidget != NULL && event->button() == Qt::LeftButton && leftMousePressed)
	{
		leftMousePressed = false;
		this->setCursor(nowCursorShap.shape());
	}
	else
	{
		if (changeShap)
		{
			this->setCursor(nowCursorShap.shape());
			changeShap = false;
		}
	}
	QWidget::mouseReleaseEvent(event);
}


void CustomTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (closeRelativeWidget != NULL && event->button() == Qt::LeftButton && (max_normale_enable || double_click_max_enbale))
	{
		changeMaxNormalSize();
	}
	//QWidget::mouseDoubleClickEvent(event);
}


bool CustomTitleBar::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::WindowStateChange)
	{
		QWindowStateChangeEvent *stateEvent = static_cast<QWindowStateChangeEvent *>(event);
		if (stateEvent->oldState() & Qt::WindowMinimized)
		{
			if (mini_enable && (max_normale_enable || double_click_max_enbale) && closeRelativeWidget != NULL)
			{
				if (is_show_max)
				{
					closeRelativeWidget->setWindowState(closeRelativeWidget->windowState() | Qt::WindowMaximized);
				}
				else
				{
					closeRelativeWidget->setWindowState(closeRelativeWidget->windowState() & ~Qt::WindowMaximized);
				}
				return true;
			}
		}
		updateMaxNormalState(); //更新最大化最小化按钮状态
	}
	return QObject::eventFilter(obj, event);
}

//*********************************************析构函数***********************************************
CustomTitleBar::~CustomTitleBar()
{
    //qDebug() << "delete CustomTitleBar";
}
