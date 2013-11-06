#include "ItemEntity.h"
#include <math.h>

ItemEntity::ItemEntity( int sx, int sy, QString entityText ) : ItemComponent( sx, sy, entityText )
{
	setPath();
	setFlags(QGraphicsItem::ItemIsSelectable /*| QGraphicsItem::ItemIsMovable*/);
	setAcceptsHoverEvents(true);
}

ItemEntity::~ItemEntity()
{
}

QRectF ItemEntity::boundingRect() const
{
	return _qPainterPath.boundingRect();
}

QPainterPath ItemEntity::shape() const
{
	return _qPainterPath;
}

void ItemEntity::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->setBrush(Qt::white);
	painter->drawPath(_qPainterPath);
	paintText(painter);
	paintBorder(painter);
}

void ItemEntity::paintText( QPainter* painter )
{
	ItemComponent::paintText(painter, false);
}

void ItemEntity::setPath()
{
	_qPainterPath.addRect(_textBoundingRectangle);
}

void ItemEntity::paintBorder( QPainter* painter )
{
	if (isSelected())
	{
		ItemComponent::setPaintBorderFont(painter);
		painter->drawRect(_textBoundingRectangle);
	}
}
