#include "RelationshipItem.h"
#include "ERDiagramMessageBoxManager.h"
#include <QPen>
#include <QLineF>
#include <QtGui>
#include <QGraphicsLineItem>
#include "..\src\corelib\io\qdebug.h"

RelationshipItem::RelationshipItem( QPointF sourceComponentItem, QPointF destionationComponentItem )
{
	_sourceComponentItem = sourceComponentItem;
	_destionationComponentItem = destionationComponentItem;

	QVector<QPointF> qPointSet;
	qPointSet.push_back(_sourceComponentItem);
	qPointSet.push_back(_destionationComponentItem);
	_qPainterPath.addPolygon(qPointSet);

	qDebug()<<_qPainterPath;
	qDebug()<<_sourceComponentItem;
	qDebug()<<_destionationComponentItem;

	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	setAcceptsHoverEvents(true);
}

RelationshipItem::~RelationshipItem()
{
}

QRectF RelationshipItem::boundingRect() const
{
	//qDebug()<<"in boundingRect"<<_qPainterPath;
	return _qPainterPath.boundingRect();
}

QPainterPath RelationshipItem::shape()
{
	qDebug()<<"in Sharp"<<_qPainterPath;

	QVector<QPointF> qPointSet;
	qPointSet.push_back(_sourceComponentItem);
	qPointSet.push_back(_destionationComponentItem);
	_qPainterPath.addPolygon(qPointSet);
	return _qPainterPath;
}

void RelationshipItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->drawPath(_qPainterPath);
	//painter->drawRect(boundingRect());
}
// 
// void RelationshipItem::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
// {
// 	this->update();
// }
// 
// void RelationshipItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
// {
// 	this->update();
// }
// 
// void RelationshipItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
// {
// 	this->update();
// }