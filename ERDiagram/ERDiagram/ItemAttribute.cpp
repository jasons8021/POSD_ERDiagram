#include "ItemAttribute.h"

ItemAttribute::ItemAttribute( int sx, int sy, QString attributeText ) : ItemComponent( sx, sy, attributeText )
{
	setPath();
	setFlags(QGraphicsItem::ItemIsSelectable /*| QGraphicsItem::ItemIsMovable*/);
	setAcceptsHoverEvents(true);
	_isPrimaryKey = false;
}

ItemAttribute::~ItemAttribute()
{
}

QRectF ItemAttribute::boundingRect() const
{
	return _qPainterPath.boundingRect();
}

QPainterPath ItemAttribute::shape() const
{
	return _qPainterPath;
}

void ItemAttribute::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->setBrush(Qt::white);
	painter->drawPath(_qPainterPath);
	paintText(painter);
	paintBorder(painter);
}

void ItemAttribute::paintText(QPainter* painter)
{
	ItemComponent::paintText(painter, _isPrimaryKey);
}

void ItemAttribute::setPrimaryKey( bool isPrimaryKey )
{
	_isPrimaryKey = isPrimaryKey;
}

bool ItemAttribute::getPrimaryKey()
{
	return _isPrimaryKey;
}

void ItemAttribute::setPath()
{
	_qPainterPath.addEllipse(_textBoundingRectangle);
}

void ItemAttribute::paintBorder( QPainter* painter )
{
	if (isSelected())
	{
		ItemComponent::setPaintBorderFont(painter);
		painter->drawEllipse(_textBoundingRectangle);
	}
}
