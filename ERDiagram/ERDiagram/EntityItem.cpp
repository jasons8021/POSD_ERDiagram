#include "EntityItem.h"
#include <math.h>

EntityItem::EntityItem()
{
	_qPainterPath.addRect(QRect( 0, 0, 200, 60 ));
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemClipsToShape);
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
	QFont font;
	font.setStyleHint(QFont::Times, QFont::PreferAntialias);
	font.setUnderline(true);
	font.setBold(true);
	font.setPointSize(18);

	// QRect材把计琌よオà畒夹材把计琌材琌糴
	painter->drawPath(_qPainterPath);

	painter->setRenderHint(QPainter::Antialiasing);
	painter->setBrush(Qt::black);
	painter->setFont(font);
	painter->drawText( QRect( 30, 20, 200, 60 ) , "Hello World");
}