#include "EntityItem.h"
#include <math.h>

EntityItem::EntityItem( QString entityText )
{
	_entityText = entityText;
	_qPainterPath.addRect(QRect( 0, 0, 200, 60 ));
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
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
	// QRect第一二個參數是方形的左上角座標，第三個參數是長、第四個是寬
	painter->drawPath(_qPainterPath);

	// 寫字
	QFont font;
	font.setStyleHint(QFont::Times, QFont::PreferAntialias);
	font.setUnderline(true);
	font.setBold(true);
	font.setPointSize(18);

	painter->setRenderHint(QPainter::Antialiasing);
	painter->setBrush(Qt::black);
	painter->setFont(font);
	painter->drawText( QRect( 30, 20, 200, 60 ) , _entityText);
}