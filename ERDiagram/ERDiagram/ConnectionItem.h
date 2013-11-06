#ifndef CONNECTIONITEM_H_
#define CONNECTIONITEM_H_

#define PARAMETER_CONNECTIONITEMSHAPE "Line"

#include <QRect>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>
#include <QPainterPath>
#include <QPen>
#include <QLineF>
#include <QtGui>
#include <QGraphicsLineItem>
#include <QDebug>

#include "ComponentItem.h"

class ConnectionItem : public ComponentItem
{
public:
	ConnectionItem();
	ConnectionItem(ComponentItem*, ComponentItem*);
	virtual ~ConnectionItem();
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* );
//	void updatePosition();
private:
	ComponentItem* _sourceComponentItem;
	ComponentItem* _destionationComponentItem;
};

#endif