#include "ItemRelationship.h"

ItemRelationship::ItemRelationship( int sx, int sy, QString relationshipText ) : ItemComponent( sx, sy, relationshipText )
{
	QVector<QPointF> pointSet;
	pointSet.push_back(QPointF(sx, sy + (_itemHeight / 2)));
	pointSet.push_back(QPointF(sx + (_itemWidth / 2), sy));
	pointSet.push_back(QPointF(sx + _itemWidth, sy +  (_itemHeight / 2)));
	pointSet.push_back(QPointF(sx + (_itemWidth / 2), sy + _itemHeight));
	pointSet.push_back(QPointF(sx, sy + (_itemHeight / 2)));

	setPath(pointSet);
	
	setFlags(QGraphicsItem::ItemIsSelectable /*| QGraphicsItem::ItemIsMovable*/);
	setAcceptsHoverEvents(true);
}

ItemRelationship::~ItemRelationship()
{
}

QRectF ItemRelationship::boundingRect() const
{
	return _qPainterPath.boundingRect();
}

QPainterPath ItemRelationship::shape() const
{
	return _qPainterPath;
}

void ItemRelationship::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->setBrush(Qt::white);
	painter->drawPath(_qPainterPath);
	paintText(painter);
	paintBorder(painter);
}

void ItemRelationship::paintText( QPainter* painter )
{
	ItemComponent::paintText(painter, false);
}

void ItemRelationship::setPath(QVector<QPointF> pointSet)
{
	_qPainterPath.addPolygon(pointSet);
}

void ItemRelationship::paintBorder( QPainter* painter )
{
	if (isSelected())
	{
		ItemComponent::setPaintBorderFont(painter);
		painter->drawPath(_qPainterPath);
	}
}
