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
	// QRect第一二個參數是方形的左上角座標，第三個參數是長、第四個是寬
	_textBoundingRectangle = QRect(sx, sy, _textWidth + PARAMETER_ADJUSTWIDTH, PARAMETER_ITEMHEIGHT);
	_itemWidth = _textBoundingRectangle.width();
	_itemHeight = _textBoundingRectangle.height();

}

ItemComponent::~ItemComponent()
{
}

// 處理attribute、entity、relationshipItem的寫字
void ItemComponent::paintText( QPainter* painter, bool isUnderLine)
{
	qreal extra = -5;
	// 寫字
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

// 取得item的寬度
int ItemComponent::getItemWidth()
{
	return _itemWidth;
}

// 設定item的高度
void ItemComponent::setItemWidth( int itemWidth )
{
	_itemWidth = itemWidth;
}

// 取得item的高度
int ItemComponent::getItemHeight()
{
	return _itemHeight;
}

// 設定item的高度
void ItemComponent::setItemHeight( int itemHeight )
{
	_itemHeight = itemHeight;
}

// 取得item的中心點
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
