#include "ItemNode.h"

ItemNode::ItemNode( int sx, int sy, QString Text ) : ItemComponent( sx, sy, Text )
{
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

ItemNode::~ItemNode()
{

}

QRectF ItemNode::boundingRect() const
{
	return ItemComponent::boundingRect();
}

QPainterPath ItemNode::shape() const
{
	return ItemComponent::shape();
}

// 畫圖案
void ItemNode::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->setBrush(Qt::white);
	painter->drawPath(_qPainterPath);
	paintText(painter, _isUnderLine);
	paintBorder(painter);
}

void ItemNode::setPath()
{
	// 由子類別實做
}

void ItemNode::updatePosition()
{
	// 由子類別實做
}

void ItemNode::setIsUnderLine( bool isUnderLine )
{
	_isUnderLine = isUnderLine;
}
