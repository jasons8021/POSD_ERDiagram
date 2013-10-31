#include "AttributeItem.h"

AttributeItem::AttributeItem()
{
// 	QVector<QPointF> circleCoordinate;
// 	{
// 		circleCoordinate.push_back(QPointF(25,  25));
// 		circleCoordinate.push_back(QPointF(25, -25));
// 		circleCoordinate.push_back(QPointF(-25, -25));
// 		circleCoordinate.push_back(QPointF(-25, 25));
// 		circleCoordinate.push_back(QPointF(25, 25));
// 	}
// 	_qPainterPath.addPolygon(QPolygonF(rectangleCoordinate));
}

AttributeItem::~AttributeItem()
{
}

QRectF AttributeItem::boundingRect() const
{
	return _qPainterPath.boundingRect();
}

QPainterPath AttributeItem::sharp()
{
	return _qPainterPath;
}

void AttributeItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
// 	QPen pen = painter->pen();
// 	pen.setCosmetic(true);
// 	pen.setWidth(5);
// 	pen.setColor(QColor(Qt::red));
// 	painter->setPen(pen);
// 	QBrush brush = painter->brush();
// 	brush.setStyle(Qt::SolidPattern);
// 	painter->setBrush(brush);

	painter->drawEllipse(0,0,100,75);
}

void AttributeItem::hello()
{

}
