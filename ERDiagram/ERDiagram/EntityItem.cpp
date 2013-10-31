#include "EntityItem.h"

EntityItem::EntityItem()
{
	QVector<QPointF> rectangleCoordinate;
	{
		rectangleCoordinate.push_back(QPointF(25,  25));
		rectangleCoordinate.push_back(QPointF(25, -25));
		rectangleCoordinate.push_back(QPointF(-25, -25));
		rectangleCoordinate.push_back(QPointF(-25, 25));
		rectangleCoordinate.push_back(QPointF(25, 25));
	}
	_qPainterPath.addPolygon(QPolygonF(rectangleCoordinate));
}

EntityItem::~EntityItem()
{
}

QRectF EntityItem::boundingRect() const
{
	return _qPainterPath.boundingRect();
}

QPainterPath EntityItem::sharp()
{
	return _qPainterPath;
}

void EntityItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->drawPath(_qPainterPath);
}

