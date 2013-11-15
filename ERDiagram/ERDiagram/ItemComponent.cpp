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
	// QRect�Ĥ@�G�ӰѼƬO��Ϊ����W���y�СA�ĤT�ӰѼƬO���B�ĥ|�ӬO�e
	_textBoundingRectangle = QRect(sx, sy, _textWidth + PARAMETER_ADJUSTWIDTH, PARAMETER_ITEMHEIGHT);
	_itemWidth = _textBoundingRectangle.width();
	_itemHeight = _textBoundingRectangle.height();

}

ItemComponent::~ItemComponent()
{
}

// �B�zattribute�Bentity�BrelationshipItem���g�r
void ItemComponent::paintText( QPainter* painter, bool isUnderLine)
{
	qreal extra = -5;
	// �g�r
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

// ���oitem���e��
int ItemComponent::getItemWidth()
{
	return _itemWidth;
}

// �]�witem������
void ItemComponent::setItemWidth( int itemWidth )
{
	_itemWidth = itemWidth;
}

// ���oitem������
int ItemComponent::getItemHeight()
{
	return _itemHeight;
}

// �]�witem������
void ItemComponent::setItemHeight( int itemHeight )
{
	_itemHeight = itemHeight;
}

// ���oitem�������I
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
