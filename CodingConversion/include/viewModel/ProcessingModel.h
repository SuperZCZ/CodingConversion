/*!
 * FileName: ProcessingModel.h
 *
 * Author:   ZhangChaoZe
 * Date:     2020-1-19
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 处理model
 */
#ifndef _PROCESSING_MODEL_H_
#define _PROCESSING_MODEL_H_

#include <QVariant>
#include <QStandardItemModel>

class ProcessingModel :public QStandardItemModel
{
	Q_OBJECT
public:
	ProcessingModel(QObject *parent = NULL);
	virtual ~ProcessingModel();

public:
	virtual QVariant data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const;
};

#endif