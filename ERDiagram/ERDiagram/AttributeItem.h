#ifndef ATTRIBUTEITEM_H_
#define ATTRIBUTEITEM_H_

#define PARAMETER_ATTRIBUTEITEMSHAPE "Ellipse"

#include <QRect>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>
#include <QPainterPath>
#include "ComponentItem.h"

class AttributeItem : public ComponentItem
{
public:
	AttributeItem(int,int,QString);
	virtual ~AttributeItem();
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* );
	void paintText(QPainter*);
	void setPrimaryKey(bool);
	bool getPrimaryKey();
private:
	bool _isPrimaryKey;
};

#endif