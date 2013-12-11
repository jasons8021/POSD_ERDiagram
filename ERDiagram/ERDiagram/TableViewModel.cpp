#include "TableViewModel.h"

TableViewModel::TableViewModel()
{
	setHorizontalHeaderItem(0, new QStandardItem(QString(" Type ")));
	setHorizontalHeaderItem(1, new QStandardItem(QString(" Text ")));
}

TableViewModel::~TableViewModel()
{
}

void TableViewModel::addNodeIntoModel( int rowNumber, QString type, QString text )
{
	// �N²�g�ର��r
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

	// �]�wtype���椣�i�I��B�s��
	typeItem->setFlags(typeItem->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);

	// �s�Wrow
	QList<QStandardItem*> rowContent;
	rowContent << typeItem << textItem;
	//appendRow(row);
	insertRow(rowNumber, rowContent);
}

void TableViewModel::changeTargetRowText( int rowNumber, QString type, QString text )
{
	this->removeRow(rowNumber);
	addNodeIntoModel(rowNumber, type, text);
}

