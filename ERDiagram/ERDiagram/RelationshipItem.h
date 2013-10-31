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
	QPainterPath sharp();
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* );
};

#endif