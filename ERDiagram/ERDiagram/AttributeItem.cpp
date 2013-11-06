#include "AttributeItem.h"
#include <QDebug>

AttributeItem::AttributeItem( int sx, int sy, QString attributeText ) : ComponentItem( sx, sy, attributeText )
{
	ComponentItem::setPath(PARAMETER_ATTRIBUTEITEMSHAPE);
	setFlags(QGraphicsItem::ItemIsSelectable /*| QGraphicsItem::ItemIsMovable*/);
	setAcceptsHoverEvents(true);
	_isPrimaryKey = true;
}

AttributeItem::~AttributeItem()
{
}

QRectF AttributeItem::boundingRect() const
{
	return _qPainterPath.boundingRect();
}

QPainterPath AttributeItem::shape() const
{
	return _qPainterPath;
}

void AttributeItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->setBrush(Qt::white);
	painter->drawPath(_qPainterPath);
	paintText(painter);
	paintBorder(painter, PARAMETER_ATTRIBUTEITEMSHAPE);
}

void AttributeItem::paintText(QPainter* painter)
{
	ComponentItem::paintText(painter, _isPrimaryKey);
}

void AttributeItem::setPrimaryKey( bool isPrimaryKey )
{
	_isPrimaryKey = isPrimaryKey;
}

bool AttributeItem::getPrimaryKey()
{
	return _isPrimaryKey;
}