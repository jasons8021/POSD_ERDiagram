#include "ComponentItem.h"

ComponentItem::ComponentItem()
{
}

ComponentItem::ComponentItem( int sx, int sy, QString text )
{
	QFont textFont;
	QFontMetrics fm(textFont);

	_sx = sx;
	_sy = sy;
	_text = text;
	_textWidth = fm.width(_text);
	// QRect材把计琌よオà畒夹材把计琌材琌糴
	_textBoundingRectangle = QRect(sx, sy, _textWidth + PARAMETER_ADJUSTWIDTH, PARAMETER_ITEMHEIGHT);
	_itemWidth = _textBoundingRectangle.width();
	_itemHeight = _textBoundingRectangle.height();

}

ComponentItem::~ComponentItem()
{
}

void ComponentItem::paintText( QPainter* painter, bool isUnderLine)
{
	qreal extra = -5;
	// 糶
	QFont font;
	font.setStyleHint(QFont::Times, QFont::PreferAntialias);
	if(isUnderLine)
		font.setUnderline(true);
	else
		font.setUnderline(false);
	font.setBold(true);
	font.setPointSize(18);

	painter->setRenderHint(QPainter::Antialiasing);
	painter->setBrush(Qt::black);
	painter->setFont(font);
	painter->drawText(_textBoundingRectangle, Qt::TextWordWrap | Qt::AlignCenter, _text);
}

int ComponentItem::getItemWidth()
{
	return _itemWidth;
}

void ComponentItem::setItemWidth( int itemWidth )
{
	_itemWidth = itemWidth;
}

int ComponentItem::getItemHeight()
{
	return _itemHeight;
}

void ComponentItem::setItemHeight( int itemHeight )
{
	_itemHeight = itemHeight;
}

void ComponentItem::setPath( QString shapeType )
{
	if (shapeType == PARAMETER_ENTITYITEMSHAPE)
		_qPainterPath.addRect(_textBoundingRectangle);
	else if (shapeType == PARAMETER_ATTRIBUTEITEMSHAPE)
		_qPainterPath.addEllipse(_textBoundingRectangle);
}

void ComponentItem::setPath( QVector<QPointF> qPointSet )
{
	_qPainterPath.addPolygon(qPointSet);
}

QPointF ComponentItem::getItemCenter()
{
	return _textBoundingRectangle.center();
}

void ComponentItem::paintBorder( QPainter* painter, QString shapeType )
{
	QPen qPen;
	painter->setBrush(Qt::NoBrush);
	if (isSelected())
	{
		qPen.setStyle(Qt::SolidLine);
		qPen.setWidth(4);
		qPen.setBrush(QColor(172, 210, 74));
		painter->setPen(qPen);

		if (shapeType == PARAMETER_ENTITYITEMSHAPE)
			painter->drawRect(_textBoundingRectangle);
		else if (shapeType == PARAMETER_ATTRIBUTEITEMSHAPE)
			painter->drawEllipse(_textBoundingRectangle);
		else if (shapeType == PARAMETER_RELATIONSHIPITEMSHAPE)
			painter->drawPath(_qPainterPath);
		else if (shapeType == PARAMETER_CONNECTIONITEMSHAPE)
			painter->drawPath(_qPainterPath);
	}
}
