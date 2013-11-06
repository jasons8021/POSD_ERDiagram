#ifndef ITEMCONNECTION_H_
#define ITEMCONNECTION_H_

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
#include "ItemComponent.h"

class ItemConnection : public ItemComponent
{
public:
	ItemConnection();
	ItemConnection(ItemComponent*, ItemComponent*, QString);
	virtual ~ItemConnection();
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* );
	void setPath(QVector<QPointF>);
	void paintBorder(QPainter*);
private:
	ItemComponent* _sourceComponentItem;
	ItemComponent* _destionationComponentItem;
	QPointF _centerPos;
};

#endif