#ifndef ITEMRELATIONSHIP_H_
#define ITEMRELATIONSHIP_H_

#define PARAMETER_RELATIONSHIPITEMSHAPE "Diamond"

#include <QRect>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>
#include <QPainterPath>
#include "ItemComponent.h"

class ItemRelationship : public ItemComponent
{
public:
	ItemRelationship(int, int, QString);
	virtual ~ItemRelationship();
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* );
	void paintText(QPainter*);
	void setPath(QVector<QPointF>);
	void paintBorder(QPainter*);
	void updatePosition();
// 	void mouseMoveEvent(QGraphicsSceneMouseEvent*);
// 	void mousePressEvent(QGraphicsSceneMouseEvent*);
// 	void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
private:
};

#endif