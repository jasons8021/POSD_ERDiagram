#ifndef ATTRIBUTEITEM_H_
#define ATTRIBUTEITEM_H_

#include <QRect>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>
#include <QPainterPath>
#include "ComponentItem.h"

class AttributeItem : public ComponentItem
{
public:
	AttributeItem(QString);
	virtual ~AttributeItem();
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* );
	void setPrimaryKey(bool);
	bool getPrimaryKey();
private:
	bool _isPrimaryKey;
	QString _attributeText;
};

#endif