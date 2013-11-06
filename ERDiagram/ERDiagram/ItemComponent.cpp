#include "ItemComponent.h"

ItemComponent::ItemComponent()
{
}

ItemComponent::ItemComponent( int sx, int sy, QString text )
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

ItemComponent::~ItemComponent()
{
}

void ItemComponent::paintText( QPainter* painter, bool isUnderLine)
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

int ItemComponent::getItemWidth()
{
	return _itemWidth;
}

void ItemComponent::setItemWidth( int itemWidth )
{
	_itemWidth = itemWidth;
}

int ItemComponent::getItemHeight()
{
	return _itemHeight;
}

void ItemComponent::setItemHeight( int itemHeight )
{
	_itemHeight = itemHeight;
}

QPointF ItemComponent::getItemCenter()
{
	return _textBoundingRectangle.center();
}

void ItemComponent::setPaintBorderFont( QPainter* painter )
{
	QPen qPen;
	painter->setBrush(Qt::NoBrush);
	if (isSelected())
	{
		qPen.setStyle(Qt::SolidLine);
		qPen.setWidth(4);
		qPen.setBrush(QColor(172, 210, 74));
		painter->setPen(qPen);
	}
}

QRectF ItemComponent::boundingRect() const
{
	return _qPainterPath.boundingRect();
}

QPainterPath ItemComponent::shape() const
{
	return _qPainterPath;
}

void ItemComponent::paint( QPainter*, const QStyleOptionGraphicsItem*, QWidget* )
{

}
