#include "ItemConnection.h"
#include "..\src\corelib\io\qdebug.h"

ItemConnection::ItemConnection()
{
	// 之後再設定要使用的sourceItem跟destinationItem，可以做點對點的功能(item rect=(0,0,0,0))
	_sourceItem = NULL;
	_destionationItem = NULL;
	setZValue(-1);
	setFlags(QGraphicsItem::ItemIsSelectable /*| QGraphicsItem::ItemIsMovable*/);
	setAcceptsHoverEvents(true);
}

ItemConnection::ItemConnection( ItemComponent* sourceItem, ItemComponent* destionationItem, QString text)
{
	_sourceItem = sourceItem;
	_destionationItem = destionationItem;
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
void ItemConnection::updatePosition()
{
	setSourceDestinationPoint();
}

// 設起終點位置
void ItemConnection::setSourceDestinationPoint()
{
	// 兩點連線中心
	_centerPos.setX((getScenePosition(_sourceItem).rx() + getScenePosition(_destionationItem).rx()) / 2);
	_centerPos.setY((getScenePosition(_sourceItem).ry() + getScenePosition(_destionationItem).ry()) / 2);
	_connectedLine = QLineF(getScenePosition(_sourceItem), getScenePosition(_destionationItem));

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

void ItemConnection::setSourceItem( ItemComponent* sourceItem )
{
	sourceItem = _sourceItem;
	if (_sourceItem != NULL && _destionationItem != NULL)
		setSourceDestinationPoint();
}

void ItemConnection::setDestionationItem( ItemComponent* destionationItem )
{
	destionationItem = _destionationItem;
	if (_sourceItem != NULL && _destionationItem != NULL)
		setSourceDestinationPoint();
}
