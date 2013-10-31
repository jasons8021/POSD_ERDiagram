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
	// 在一個方形中畫一個橢圓，0,0為方形左上角的座標，100為寬、80為高
	painter->drawEllipse( QRect( 0, 0, 100, 80 ) );
}
