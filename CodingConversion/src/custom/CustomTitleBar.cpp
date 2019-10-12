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

	parentWidget = parnet;
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

	if (parentWidget == NULL)
    {
        qDebug() << "CustomTitleBar parentWidget is NULL!!!!";
    }
    else
    {
		if (!isLayout)
		{
			//绝对布局时  需手动改变宽度和位置
			this->move(0,0);  //这里是移动到父窗体的(0,0)左上角   而不是关系窗体的(0,0)左上角！！！！！
			this->resize(parentWidget->width(), this->height());
		}
    }

}



//*********************************************protected function********************************************
void CustomTitleBar::mousePressEvent(QMouseEvent *event)
{
	if (moveRelativeWidget != NULL && closeRelativeWidget != NULL && event->button() == Qt::LeftButton)
	{
		int dir_type;
		QPoint current_point;
		if (closeRelativeWidget->parent() != NULL)
		{
			//如果关闭窗体不是最桌面上的窗体 使用鼠标相对于标题栏的位置
			current_point = event->pos();
		}
		else
		{
			//如果关闭窗体是最顶层窗体 使用鼠标相对于关闭窗体的位置
			current_point = cursor().pos() - closeRelativeWidget->pos();
		}
		if (!enableMouseScale || (dir_type = getCursorPosition(closeRelativeWidget, current_point, MOUSE_TRACK_PADDING, true)) == 0)
		{
			//处理移动窗体事件
			beforMouseGlobalPos = cursor().pos(); //记录当前鼠标全局位置 作为移动参考
			leftMousePressed = true;
			nowCursorShap = cursor();
		    this->setCursor(Qt::ClosedHandCursor);
		}
		else
		{
			nowCursorShap = cursor();
			switch(dir_type)
			{
			case 1:
				this->setCursor(QCursor(Qt::SizeVerCursor));
				break;
			case 2:
				this->setCursor(QCursor(Qt::SizeVerCursor));
				break;
			case 3:
				this->setCursor(QCursor(Qt::SizeHorCursor));
				break;
			case 4:
				this->setCursor(QCursor(Qt::SizeHorCursor));
				break;
			case 5:
				this->setCursor(QCursor(Qt::SizeFDiagCursor));  // 设置鼠标形状
				break;
			case 6:
				this->setCursor(QCursor(Qt::SizeBDiagCursor));
				break;
			case 7: 
				this->setCursor(QCursor(Qt::SizeFDiagCursor));
				break;
			case 8:
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
	if (leftMousePressed&&moveRelativeWidget != NULL)
	{
		int x_change,y_change;
		QPoint move_to;
		x_change = event->globalPos().x() - beforMouseGlobalPos.x();
		y_change = event->globalPos().y() - beforMouseGlobalPos.y();
		move_to.setX(moveRelativeWidget->x() + x_change);
		move_to.setY(moveRelativeWidget->y() + y_change);
		moveRelativeWidget->move(move_to);
		beforMouseGlobalPos = event->globalPos();
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
