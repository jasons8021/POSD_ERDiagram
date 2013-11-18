#include "ItemConnection.h"
#include "..\src\corelib\io\qdebug.h"

ItemConnection::ItemConnection()
{
}

ItemConnection::ItemConnection( ItemComponent* sourceComponentItem, ItemComponent* destionationComponentItem, QString text)
{
	_sourceComponentItem = sourceComponentItem;
	_destionationComponentItem = destionationComponentItem;
	_text = text;

	setSourceDestinationPoint();

	setZValue(-1);
	setFlags(QGraphicsItem::ItemIsSelectable /*| QGraphicsItem::ItemIsMovable*/);
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
	updatePosition();
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
	_qPainterPath.~QPainterPath();
	_qPainterPath = QPainterPath();
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

void ItemConnection::updatePosition()
{
	// 兩點連線中心
	_centerPos.setX((getScenePosition(_sourceComponentItem).rx() + getScenePosition(_destionationComponentItem).rx()) / 2);
	_centerPos.setY((getScenePosition(_sourceComponentItem).ry() + getScenePosition(_destionationComponentItem).ry()) / 2);
	_connectedLine = QLineF(getScenePosition(_sourceComponentItem), getScenePosition(_destionationComponentItem));
	QVector<QPointF> pointSet;
	// 起點
	pointSet.push_back(getScenePosition(_sourceComponentItem));
	// 終點
	pointSet.push_back(getScenePosition(_destionationComponentItem));

	setPath(pointSet);
// 
// 	setSourceDestinationPoint();
}

// 設起終點位置
void ItemConnection::setSourceDestinationPoint()
{
	// 兩點連線中心
	_centerPos.setX((_sourceComponentItem->getItemCenter().rx() + _destionationComponentItem->getItemCenter().rx()) / 2);
	_centerPos.setY((_sourceComponentItem->getItemCenter().ry() + _destionationComponentItem->getItemCenter().ry()) / 2);
	_connectedLine = QLineF(_sourceComponentItem->getItemCenter(), _destionationComponentItem->getItemCenter());
	QVector<QPointF> pointSet;
	// 起點
	pointSet.push_back(QPointF(_connectedLine.p1()));
	// 終點
	pointSet.push_back(QPointF(_connectedLine.p2()));

	setPath(pointSet);
}

QPointF ItemConnection::getScenePosition( ItemComponent* item )
{
	qreal basedPosX = item->getItemCenter().rx();
	qreal basedPosY = item->getItemCenter().ry();

	return mapFromItem(item, basedPosX, basedPosY);
}
