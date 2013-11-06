#include "ItemConnection.h"
#include "..\src\corelib\io\qdebug.h"

ItemConnection::ItemConnection()
{
}

ItemConnection::ItemConnection( ItemComponent* sourceComponentItem, ItemComponent* destionationComponentItem, QString text)
{
	_sourceComponentItem = sourceComponentItem;
	_destionationComponentItem = destionationComponentItem;

	_centerPos.setX((_sourceComponentItem->x() + _destionationComponentItem->x()) / 2);
	_centerPos.setY((_sourceComponentItem->y() + _destionationComponentItem->y()) / 2);

	_text = text;
	QVector<QPointF> pointSet;
	pointSet.push_back(_sourceComponentItem->getItemCenter());
	pointSet.push_back(_destionationComponentItem->getItemCenter());

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
	painter->drawPath(_qPainterPath);

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
