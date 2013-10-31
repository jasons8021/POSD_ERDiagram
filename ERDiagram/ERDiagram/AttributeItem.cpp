#include "AttributeItem.h"

AttributeItem::AttributeItem()
{
	_qPainterPath.addEllipse(QRect( 0, 0, 100, 80 ));
}

AttributeItem::~AttributeItem()
{
}

QRectF AttributeItem::boundingRect() const
{
	return _qPainterPath.boundingRect();
}

QPainterPath AttributeItem::sharp()
{
	return _qPainterPath;
}

void AttributeItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	// painter->drawRect(QRect( 10, 10, 100, 80 ));
	// �b�@�Ӥ�Τ��e�@�Ӿ��A0,0����Υ��W�����y�СA100���e�B80����
	painter->drawEllipse( QRect( 0, 0, 100, 80 ) );
}
