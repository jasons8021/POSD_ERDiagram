#include "ItemEntity.h"
#include <math.h>

ItemEntity::ItemEntity( int erModelID, int sx, int sy, QString entityText ) : ItemNode( erModelID, sx, sy, PARAMETER_ENTITY, entityText )
{
	setPath();
}

// preview Item使用
ItemEntity::ItemEntity( int sx, int sy, QString entityText ) : ItemNode( sx, sy, PARAMETER_ENTITY, entityText )
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

// Entity是一個方形，所以加入的是Rect
void ItemEntity::setPath()
{
	_qPainterPath.~QPainterPath();
	_qPainterPath = QPainterPath();
	_qPainterPath.addRect(_textBoundingRectangle);
}

void ItemEntity::updatePosition()
{
	// 未用到
}
