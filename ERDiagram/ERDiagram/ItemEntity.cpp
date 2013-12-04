#include "ItemEntity.h"
#include <math.h>

ItemEntity::ItemEntity( int sx, int sy, QString entityText ) : /*ItemComponent*/ItemNode( sx, sy, entityText )
{
	setPath();
}

ItemEntity::~ItemEntity()
{
}

QRectF ItemEntity::boundingRect() const
{
	return ItemNode::boundingRect();
}

QPainterPath ItemEntity::shape() const
{
	return ItemNode::shape();
}

void ItemEntity::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	setIsUnderLine(false);
	ItemNode::paint(painter, option, widget);
}

// Entity�O�@�Ӥ�ΡA�ҥH�[�J���ORect
void ItemEntity::setPath()
{
	_qPainterPath.~QPainterPath();
	_qPainterPath = QPainterPath();
	_qPainterPath.addRect(_textBoundingRectangle);
}

void ItemEntity::updatePosition()
{
	// ���Ψ�
}
