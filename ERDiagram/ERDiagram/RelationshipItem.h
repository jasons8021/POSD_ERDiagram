#ifndef RELATIONSHIPITEM_H_
#define RELATIONSHIPITEM_H_

#include <QRect>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>
#include <QPainterPath>
#include "ComponentItem.h"

class RelationshipItem : public ComponentItem
{
public:
	RelationshipItem(QPointF, QPointF);
	virtual ~RelationshipItem();
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* );
// 	void mouseMoveEvent(QGraphicsSceneMouseEvent*);
// 	void mousePressEvent(QGraphicsSceneMouseEvent*);
// 	void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
private:
	QPointF _sourceComponentItem;
	QPointF _destionationComponentItem;
};

#endif