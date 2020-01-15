/*!
 * FileName: FilterEditWidget.h
 *
 * Author:   ZhangChaoZe
 * Date:     2019-10-30
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 过滤器编辑窗体
 */

#ifndef _FILTER_EDIT_WIDGET_H_
#define _FILTER_EDIT_WIDGET_H_

#include <QWidget>
#include <QList>
#include <QVariant>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>

#include "custom/CustomDialog.h"
#include "view/FilterEditView.h"
#include "viewModel/FilterEditTableModel.h"

struct FilterItem
{
	int type;   //0-排除 1-处理
	QString filter_name; //过滤名称
};

Q_DECLARE_METATYPE(FilterItem)

class FilterEditWidget : public CustomDialog
{
	Q_OBJECT
public:
	FilterEditWidget(QWidget *parent = NULL);
	virtual ~FilterEditWidget();

	void initConnect();
public slots:
	void setFilterList(QList<QVariant> filter_list);
	void saveFilterList();
	void resetFilterList();

	void setAddButtEnabled(bool flag)
	{
		addButt->setEnabled(flag);
	}
	void setRemoveButtEnabled(bool flag)
	{
		delButt->setEnabled(flag);
	}
	void setUpButtEnabled(bool flag)
	{
		upButt->setEnabled(flag);
	}
	void setDownButtEnabled(bool flag)
	{
		downButt->setEnabled(flag);
	}

private:
	QHBoxLayout *toolBox_HLay;
	QVBoxLayout *tableView_VLay;
	QHBoxLayout *bottom_HLay;

	QPushButton *addButt;
	QPushButton *delButt;
	QPushButton *upButt;
	QPushButton *downButt;

	FilterEditView *filterTableView;
	FilterEditTableModel *filterModel;


	QPushButton *cancelButt;
	QPushButton *saveButt;
	QPushButton *resetButt;
};

#endif

