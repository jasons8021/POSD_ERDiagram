#include "ItemComponent.h"
#include "ERDiagramScene.h"

ItemComponent::ItemComponent()
{
	_textBoundingRectangle = QRect(0, 0, 0, 0);
}

ItemComponent::ItemComponent( int sx, int sy, QString text )
{
	_sx = sx;
	_sy = sy;
	_text = text;

	setTextBoundingRectangle(sx, sy);
}

ItemComponent::~ItemComponent()
{
}

// 計算輸入的文字寬度
int ItemComponent::caculateTextWidth( QString text )
{
	QFont textFont;
	QFontMetrics textFM(textFont);

	return textFM.width(text);
}

// 設此Item的BoundingRextangle
void ItemComponent::setTextBoundingRectangle(int sx, int sy)
{
	// QRect第一二個參數是方形的左上角座標，第三個參數是長、第四個是寬
	_textBoundingRectangle = QRect(sx, sy, caculateTextWidth(_text) + PARAMETER_ADJUSTWIDTH, PARAMETER_ITEMHEIGHT);
	_itemWidth = _textBoundingRectangle.width();
	_itemHeight = _textBoundingRectangle.height();
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

// 設定item的寬度
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

// 畫外框的格式
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

// 畫被點中出現的外框
void ItemComponent::paintBorder( QPainter* painter )
{
	if (isSelected() && _scene->getCurrentMode() == PARAMETER_POINTERMODE)
	{
		ItemComponent::setPaintBorderFont(painter);
		painter->drawPath(_qPainterPath);
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
	// 畫線跟畫物件的paint不一樣，分開做
}

void ItemComponent::setScene( ERDiagramScene* erDiagramScene )
{
	_scene = erDiagramScene;
}

void ItemComponent::updatePosition()
{
	// 由子類別實做
}

void ItemComponent::setPath()
{
	// 由子類別實做
}

void ItemComponent::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
	QGraphicsItem::mousePressEvent(event);
}

void ItemComponent::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
	QGraphicsItem::mouseReleaseEvent(event);
	// 位置有改變，呼叫scene去更新
	_scene->updateItemPosition();
}

void ItemComponent::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
	QGraphicsItem::mouseMoveEvent(event);
	// 位置有改變，呼叫scene去更新
	_scene->updateItemPosition();
}

void ItemComponent::setItemID( int ID )
{
	_itemID = ID;
}

int ItemComponent::getItemID()
{
	return _itemID;
}

void ItemComponent::changeItemText( QString text )
{
	_text = text;

	if (caculateTextWidth(text) + PARAMETER_ADJUSTWIDTH > _itemWidth)
	{
		setTextBoundingRectangle(_sx, _sy);
		setPath();
	}
}