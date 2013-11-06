#include "RelationshipItem.h"

RelationshipItem::RelationshipItem( QPointF sourceComponentItem, QPointF destionationComponentItem )
{
	_sourceComponentItem = sourceComponentItem;
	_destionationComponentItem = destionationComponentItem;

// 	QVector<QPointF> qPointSet;
// 	qPointSet.push_back(_sourceComponentItem);
// 	qPointSet.push_back(_destionationComponentItem);
// 	_qPainterPath.addPolygon(qPointSet);

	
	_text = "1";
	QFont textFont;
	QFontMetrics fm(textFont);
	_textWidth = fm.width(_text);

	_textBoundingRectangle = QRect(100, 500, _textWidth + 100, 80);
	qDebug()<<_textBoundingRectangle;
	_qPainterPath.addRect(QRect(100, 500, _textWidth + 100, 0.5));
 	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	setAcceptsHoverEvents(true);
}

RelationshipItem::~RelationshipItem()
{
}

QRectF RelationshipItem::boundingRect() const
{
	return _qPainterPath.boundingRect();
}

QPainterPath RelationshipItem::shape() const
{
	return _qPainterPath;
}

void RelationshipItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	painter->setBrush(Qt::black);
	painter->drawPath(_qPainterPath);

	// ¼g¦r
	QFont font;
	font.setStyleHint(QFont::Times, QFont::PreferAntialias);
	font.setBold(true);
	font.setPointSize(18);

	painter->setRenderHint(QPainter::Antialiasing);
	//painter->setBrush(Qt::black);
	painter->setFont(font);
	painter->drawText( _textBoundingRectangle, Qt::TextWordWrap | Qt::AlignCenter, _text);
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