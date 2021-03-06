#include "ItemComponent.h"
#include "ERDiagramScene.h"

ItemComponent::ItemComponent( int erModelID, QString type, QString text )
{
	_erModelID = erModelID;
	_type = type;
	_text = text;
}

ItemComponent::ItemComponent( int erModelID, int sx, int sy, QString type, QString text )
{
	_erModelID = erModelID;

	// 圖形左上角座標為Item的基礎座標，圖形依照左上角為準
	_sx = sx;
	_sy = sy;
	_type = type;
	_text = text;
	setTextBoundingRectangle(_sx, _sy);
}

// preview Item使用
ItemComponent::ItemComponent( int sx, int sy, QString type, QString text )
{
	_sx = sx;
	_sy = sy;
	_type = type;
	_text = text;
	setTextBoundingRectangle(_sx, _sy);
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
 	_scene->changeEditActionEnable(true);
	mousePressPoint = this->mapToParent(this->mapFromScene(event->scenePos()));
}

void ItemComponent::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
	QGraphicsItem::mouseReleaseEvent(event);
	QPointF mouseReleasePoint = event->scenePos();

	// 傳回值為ERModel ID
	QVector<int> selectedItemSet = _scene->searchItemIsSelected();

	_scene->moveGroupItemPositionByCmd(selectedItemSet, mouseReleasePoint - mousePressPoint);

	// 位置有改變，呼叫scene去更新
	_scene->updateItemPositionInScene();
}

void ItemComponent::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
	QGraphicsItem::mouseMoveEvent(event);

	// 位置有改變，呼叫scene去更新
	_scene->updateItemPositionInScene();
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

QString ItemComponent::getType()
{
	return _type;
}

QString ItemComponent::getText()
{
	return _text;
}

void ItemComponent::setERModelID( int componentID )
{
	_erModelID = componentID;
}

int ItemComponent::getERModelID()
{
	return _erModelID;
}

void ItemComponent::setPosition( QPointF newPosition )
{
	_sx = _sx + newPosition.x();
	_sy = _sy + newPosition.y();
	setTextBoundingRectangle(_sx, _sy);
	setPath();
}