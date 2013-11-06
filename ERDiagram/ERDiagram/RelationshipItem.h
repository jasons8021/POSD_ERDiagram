#ifndef RELATIONSHIPITEM_H_
#define RELATIONSHIPITEM_H_

#define PARAMETER_RELATIONSHIPITEMSHAPE "Diamond"

#include <QRect>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>
#include <QPainterPath>
#include "ComponentItem.h"

class RelationshipItem : public ComponentItem
{
public:
	RelationshipItem(int, int, QString);
	virtual ~RelationshipItem();
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* );
	void paintText(QPainter*);
// 	void mouseMoveEvent(QGraphicsSceneMouseEvent*);
// 	void mousePressEvent(QGraphicsSceneMouseEvent*);
// 	void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
private:
};

#endif