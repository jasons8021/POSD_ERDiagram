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

	// QRect�Ĥ@�G�ӰѼƬO��Ϊ����W���y�СA�ĤT�ӰѼƬO���B�ĥ|�ӬO�e
	painter->drawRect(QRect(0, 0, 200, 60));

	painter->setRenderHint(QPainter::Antialiasing);
	painter->setBrush(Qt::black);
	painter->setFont(font);
	painter->drawText(40, 40, "Hello World");


}

