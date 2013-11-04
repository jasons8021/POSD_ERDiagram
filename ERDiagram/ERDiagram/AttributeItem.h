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
	AttributeItem();
	virtual ~AttributeItem();
	QRectF boundingRect() const;
	QPainterPath shape();
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* );
	void setPrimaryKey(bool);
	bool getPrimaryKey();
private:
	bool _isPrimaryKey;
};

#endif