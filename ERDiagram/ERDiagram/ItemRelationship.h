#ifndef ITEMRELATIONSHIP_H_
#define ITEMRELATIONSHIP_H_

#define PARAMETER_RELATIONSHIPITEMSHAPE "Diamond"

#include <QRect>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>
#include <QPainterPath>
#include "ItemNode.h"

class ItemRelationship : public ItemNode
{
public:
	ItemRelationship(int, int, QString);
	virtual ~ItemRelationship();
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* );
	void setPath(QVector<QPointF>);
	void updatePosition();
};

#endif