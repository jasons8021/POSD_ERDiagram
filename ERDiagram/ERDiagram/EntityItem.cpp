#include "EntityItem.h"
#include <math.h>

EntityItem::EntityItem( int sx, int sy, QString entityText ) : ComponentItem( sx, sy, entityText )
{
	ComponentItem::setPath(PARAMETER_ENTITYITEMSHAPE);
	setFlags(QGraphicsItem::ItemIsSelectable /*| QGraphicsItem::ItemIsMovable*/);
	setAcceptsHoverEvents(true);
}

EntityItem::~EntityItem()
{
}

QRectF EntityItem::boundingRect() const
{
	return _qPainterPath.boundingRect();
}

QPainterPath EntityItem::shape() const
{
	return _qPainterPath;
}

void EntityItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->setBrush(Qt::white);
	painter->drawPath(_qPainterPath);
	paintText(painter);
	paintBorder(painter, PARAMETER_ENTITYITEMSHAPE);
}

void EntityItem::paintText( QPainter* painter )
{
	ComponentItem::paintText(painter, false);
}
