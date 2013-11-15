#include "ItemConnection.h"
#include "..\src\corelib\io\qdebug.h"

ItemConnection::ItemConnection()
{
}

ItemConnection::ItemConnection( ItemComponent* sourceComponentItem, ItemComponent* destionationComponentItem, QString text)
{
	_sourceComponentItem = sourceComponentItem;
	_destionationComponentItem = destionationComponentItem;

	_centerPos.setX((_sourceComponentItem->getItemCenter().rx() + _destionationComponentItem->getItemCenter().rx()) / 2);
	_centerPos.setY((_sourceComponentItem->getItemCenter().ry() + _destionationComponentItem->getItemCenter().ry()) / 2);

	_text = text;

	_connectedLine = QLineF(_sourceComponentItem->getItemCenter(), _destionationComponentItem->getItemCenter());

	QVector<QPointF> pointSet;
	// °_ÂI
	pointSet.push_back(QPointF(_connectedLine.p1()));
	// ²×ÂI
	pointSet.push_back(QPointF(_connectedLine.p2()));
	
	setPath(pointSet);

	setZValue(-1);
	setFlags(QGraphicsItem::ItemIsSelectable);
	setAcceptsHoverEvents(true);
}

ItemConnection::~ItemConnection()
{
}

QRectF ItemConnection::boundingRect() const
{
	return _qPainterPath.boundingRect();
}

QPainterPath ItemConnection::shape() const
{
	return _qPainterPath;
}

void ItemConnection::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->drawLine(_connectedLine);

	QFont font;
	font.setStyleHint(QFont::Times, QFont::PreferAntialias);
	font.setBold(true);
	font.setPointSize(18);

	painter->setRenderHint(QPainter::Antialiasing);
	painter->setBrush(Qt::black);
	painter->setFont(font);
	painter->drawText(_centerPos, _text);

	paintBorder(painter);
}

void ItemConnection::setPath( QVector<QPointF> pointSet )
{
	_qPainterPath.addPolygon(pointSet);
}

void ItemConnection::paintBorder( QPainter* painter )
{
	if (isSelected())
	{
		ItemComponent::setPaintBorderFont(painter);
		painter->drawPath(_qPainterPath);
	}
}
