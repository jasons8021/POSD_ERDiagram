#include "TableViewModel.h"
#include "..\src\corelib\io\qdebug.h"

TableViewModel::TableViewModel()
{
	setHorizontalHeaderItem(0, new QStandardItem(QString(" Type ")));
	setHorizontalHeaderItem(1, new QStandardItem(QString(" Text ")));
}

TableViewModel::~TableViewModel()
{
}

void TableViewModel::addNodeIntoModel( QString type, QString text )
{
	// 將簡寫轉為單字
	if (type == LETTER_ATTRIBUTE)
		type = TYPE_ATTRIBUTE;
	else if (type == LETTER_ENTITY)
		type = TYPE_ENTITY;
	else if (type == LETTER_RELATIONSHIP)
		type = TYPE_RELATIONSHIP;
	else
		type = TYPE_CONNECTOR;

	QStandardItem *typeItem = new QStandardItem(type);
	QStandardItem *textItem = new QStandardItem(text);

	// 設定type那欄不可點選、編輯
	typeItem->setFlags(typeItem->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);

	// 新增row
	QList<QStandardItem*> row;
	row << typeItem << textItem;
	appendRow(row);
}
