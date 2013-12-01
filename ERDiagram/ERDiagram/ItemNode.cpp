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

// �e�Ϯ�
void ItemNode::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->setBrush(Qt::white);
	painter->drawPath(_qPainterPath);
	paintText(painter, _isUnderLine);
	paintBorder(painter);
}

void ItemNode::setPath()
{
	// �Ѥl���O�갵
}

void ItemNode::updatePosition()
{
	// �Ѥl���O�갵
}

void ItemNode::setIsUnderLine( bool isUnderLine )
{
	_isUnderLine = isUnderLine;
}
