#include "ItemConnection.h"

ItemConnection::ItemConnection( int erModelID, ItemComponent* sourceItem, ItemComponent* destionationItem, QString text, bool isSetCardinality) : ItemComponent( erModelID, PARAMETER_CONNECTOR, text )
{
	_sourceItem = sourceItem;
	_destionationItem = destionationItem;
	_type = PARAMETER_CONNECTOR;
	_text = text;
	_isSetCardinality = isSetCardinality;

	setSourceDestinationPoint();

	setTextBoundingRectangle(_centerPos.rx(), _centerPos.ry());

	setZValue(-1);
	setFlags(QGraphicsItem::ItemIsSelectable);
	setAcceptsHoverEvents(true);
}

ItemConnection::~ItemConnection()
{
}

QRectF ItemConnection::boundingRect() const
{
	return ItemComponent::boundingRect();
}

QPainterPath ItemConnection::shape() const
{
	return ItemComponent::shape();
}

void ItemConnection::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	QPen qPen;
	qPen.setStyle(Qt::SolidLine);
	qPen.setWidth(2);
	qPen.setBrush(Qt::black);
	painter->setPen(qPen);

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

void ItemConnection::paintBorder( QPainter* painter )
{
	if (this->isSelected())
	{
		ItemComponent::setPaintBorderFont(painter);
		painter->drawPath(_qPainterPath);
	}
}

void ItemConnection::setPath( QVector<QPointF> pointSet )
{
	_qPainterPath.~QPainterPath();
	_qPainterPath = QPainterPath(pointSet[0]);
	_qPainterPath.lineTo(pointSet[1]);
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

bool ItemConnection::getSetCardinality()
{
	return _isSetCardinality;
}

QLineF ItemConnection::getConnectedLine()
{
	return _connectedLine;
}
