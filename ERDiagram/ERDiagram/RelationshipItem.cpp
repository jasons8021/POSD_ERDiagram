#include "RelationshipItem.h"

RelationshipItem::RelationshipItem( int sx, int sy, QString relationshipText ) : ComponentItem( sx, sy, relationshipText )
{
	QVector<QPointF> pointSet;
	pointSet.push_back(QPointF(sx, sy + (_itemHeight / 2)));
	pointSet.push_back(QPointF(sx + (_itemWidth / 2), sy));
	pointSet.push_back(QPointF(sx + _itemWidth, sy +  (_itemHeight / 2)));
	pointSet.push_back(QPointF(sx + (_itemWidth / 2), sy + _itemHeight));
	pointSet.push_back(QPointF(sx, sy + (_itemHeight / 2)));

	_qPainterPath.addPolygon(pointSet);
	//ComponentItem::setPath(PARAMETER_RELATIONSHIPITEMSHAPE);
	setFlags(QGraphicsItem::ItemIsSelectable /*| QGraphicsItem::ItemIsMovable*/);
	setAcceptsHoverEvents(true);
	qDebug()<<_qPainterPath;
}

RelationshipItem::~RelationshipItem()
{
}

QRectF RelationshipItem::boundingRect() const
{
	return _qPainterPath.boundingRect();
}

QPainterPath RelationshipItem::shape() const
{
	return _qPainterPath;
}

void RelationshipItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->setBrush(Qt::white);
	painter->drawPath(_qPainterPath);
	paintText(painter);
	paintBorder(painter, PARAMETER_RELATIONSHIPITEMSHAPE);
}

void RelationshipItem::paintText( QPainter* painter )
{
	ComponentItem::paintText(painter, false);
}
// 
// void RelationshipItem::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
// {
// 	this->update();
// }
// 
// void RelationshipItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
// {
// 	this->update();
// }
// 
// void RelationshipItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
// {
// 	this->update();
// }