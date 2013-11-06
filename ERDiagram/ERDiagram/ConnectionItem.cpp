#include "ConnectionItem.h"
#include "..\src\corelib\io\qdebug.h"

ConnectionItem::ConnectionItem()
{
}

ConnectionItem::ConnectionItem( ComponentItem* sourceComponentItem, ComponentItem* destionationComponentItem )
{
	_sourceComponentItem = sourceComponentItem;
	_destionationComponentItem = destionationComponentItem;

	QVector<QPointF> pointSet;
	pointSet.push_back(_sourceComponentItem->getItemCenter());
	pointSet.push_back(_destionationComponentItem->getItemCenter());

	ComponentItem::setPath(pointSet);

	setFlags(QGraphicsItem::ItemIsSelectable);
	setAcceptsHoverEvents(true);
}

ConnectionItem::~ConnectionItem()
{
}

QRectF ConnectionItem::boundingRect() const
{
	return _qPainterPath.boundingRect();
}

QPainterPath ConnectionItem::shape() const
{
	return _qPainterPath;
}
// 
// void ConnectionItem::updatePosition()
// {
// 	_qPainterPath = QPainterPath();
// 	QVector<QPointF> pointSet;
// 	pointSet.push_back(_sourceComponentItem->getItemCenter());
// 	pointSet.push_back(_destionationComponentItem->getItemCenter());
// 
// 	setPath(pointSet);
// }

void ConnectionItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
// 	QPen qPen;
// 	qPen.setStyle(Qt::SolidLine);
// 	qPen.setWidth(1);
// 	qPen.setBrush(Qt::black);
// 	painter->setPen(qPen);

	painter->drawPath(_qPainterPath);
	paintBorder(painter, "Line");
}
