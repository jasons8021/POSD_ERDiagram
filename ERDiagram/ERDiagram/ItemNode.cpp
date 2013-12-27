#include "ItemNode.h"

ItemNode::ItemNode( int erModelID, int sx, int sy, QString type, QString text ) : ItemComponent( erModelID, sx, sy, type, text )
{
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

// preview Item�ϥ�
ItemNode::ItemNode( int sx, int sy, QString type, QString text ) : ItemComponent( sx, sy, type, text )
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
