#ifndef ITEMRELATIONSHIP_H_
#define ITEMRELATIONSHIP_H_

#define PARAMETER_RELATIONSHIPITEMSHAPE "Diamond"
#define PARAMETER_RELATIONSHIP "R"

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
	ItemRelationship(int, int, int, QString);
	virtual ~ItemRelationship();
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* );
	void setPath(QVector<QPointF>);
	void setPath();
	void updatePosition();
	QVector<QPointF> setPointSet(int, int);
};

#endif