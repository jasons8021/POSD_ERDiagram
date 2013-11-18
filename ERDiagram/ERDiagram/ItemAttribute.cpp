#include "ItemAttribute.h"

ItemAttribute::ItemAttribute( int sx, int sy, QString attributeText ) : ItemComponent( sx, sy, attributeText )
{
	setPath();
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
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

// �e�XItem
void ItemAttribute::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->setBrush(Qt::white);
	painter->drawPath(_qPainterPath);
	paintText(painter);
	paintBorder(painter);
}

// �g�r�A�p�G�J��Attribute�OPK���ܡA�N�[�J���u
void ItemAttribute::paintText(QPainter* painter)
{
	ItemComponent::paintText(painter, _isPrimaryKey);
}

// get/set PK
void ItemAttribute::setPrimaryKey( bool isPrimaryKey )
{
	_isPrimaryKey = isPrimaryKey;
}

bool ItemAttribute::getPrimaryKey()
{
	return _isPrimaryKey;
}

// �]�w���|
void ItemAttribute::setPath()
{
	_qPainterPath.addEllipse(_textBoundingRectangle);
}

// �e�~��
void ItemAttribute::paintBorder( QPainter* painter )
{
	if (isSelected())
	{
		ItemComponent::setPaintBorderFont(painter);
		painter->drawEllipse(_textBoundingRectangle);
	}
}

void ItemAttribute::updatePosition()
{

}
