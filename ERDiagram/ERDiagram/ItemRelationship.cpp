#include "ItemRelationship.h"

ItemRelationship::ItemRelationship( int sx, int sy, QString relationshipText ) : ItemNode( sx, sy, relationshipText )
{
	QVector<QPointF> pointSet;
	pointSet.push_back(QPointF(sx, sy + (_itemHeight / 2)));
	pointSet.push_back(QPointF(sx + (_itemWidth / 2), sy));
	pointSet.push_back(QPointF(sx + _itemWidth, sy +  (_itemHeight / 2)));
	pointSet.push_back(QPointF(sx + (_itemWidth / 2), sy + _itemHeight));
	pointSet.push_back(QPointF(sx, sy + (_itemHeight / 2)));

	setPath(pointSet);
}

ItemRelationship::~ItemRelationship()
{
}

QRectF ItemRelationship::boundingRect() const
{
	return ItemNode::boundingRect();
}

QPainterPath ItemRelationship::shape() const
{
	return ItemNode::shape();
}

void ItemRelationship::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	setIsUnderLine(false);
	ItemNode::paint(painter, option, widget);
}

// Relationship是一個菱形，所以加入的是四個點座標
void ItemRelationship::setPath(QVector<QPointF> pointSet)
{
	_qPainterPath.addPolygon(pointSet);
}

void ItemRelationship::updatePosition()
{
	// 未用到
}
