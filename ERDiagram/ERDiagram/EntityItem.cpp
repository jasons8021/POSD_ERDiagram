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
	// QRect�Ĥ@�G�ӰѼƬO��Ϊ����W���y�СA�ĤT�ӰѼƬO���B�ĥ|�ӬO�e
	painter->drawPath(_qPainterPath);

	// �g�r
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