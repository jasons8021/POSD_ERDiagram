#include "RelationshipItem.h"
#include "ERDiagramMessageBoxManager.h"

RelationshipItem::RelationshipItem( QPointF sourceComponentItem, QPointF destionationComponentItem )
{
	// �_�I
	_qPainterPath.currentPosition().setX(sourceComponentItem.rx());
	_qPainterPath.currentPosition().setY(sourceComponentItem.ry());

	// ���I
	_qPainterPath.lineTo(destionationComponentItem);
}

RelationshipItem::~RelationshipItem()
{
}

QRectF RelationshipItem::boundingRect() const
{
	return _qPainterPath.boundingRect();
}

QPainterPath RelationshipItem::sharp()
{
	return _qPainterPath;
}

void RelationshipItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->drawPath(_qPainterPath);
}
