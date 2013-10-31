#include "EntityItem.h"

EntityItem::EntityItem()
{
	_qPainterPath.addRect(QRect( 0, 0, 200, 60 ));
}

EntityItem::~EntityItem()
{
}

QRectF EntityItem::boundingRect() const
{
	return _qPainterPath.boundingRect();
}

QPainterPath EntityItem::sharp()
{
	return _qPainterPath;
}

void EntityItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	QFont font;
	font.setStyleHint(QFont::Times, QFont::PreferAntialias);
	font.setUnderline(true);
	font.setBold(true);
	font.setPointSize(18);

	// QRect第一二個參數是方形的左上角座標，第三個參數是長、第四個是寬
	painter->drawRect(QRect(0, 0, 200, 60));

	painter->setRenderHint(QPainter::Antialiasing);
	painter->setBrush(Qt::black);
	painter->setFont(font);
	painter->drawText(40, 40, "Hello World");


}

