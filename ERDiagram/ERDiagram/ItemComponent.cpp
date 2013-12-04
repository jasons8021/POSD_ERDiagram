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

// �p���J����r�e��
int ItemComponent::caculateTextWidth( QString text )
{
	QFont textFont;
	QFontMetrics textFM(textFont);

	return textFM.width(text);
}

// �]��Item��BoundingRextangle
void ItemComponent::setTextBoundingRectangle(int sx, int sy)
{
	// QRect�Ĥ@�G�ӰѼƬO��Ϊ����W���y�СA�ĤT�ӰѼƬO���B�ĥ|�ӬO�e
	_textBoundingRectangle = QRect(sx, sy, caculateTextWidth(_text) + PARAMETER_ADJUSTWIDTH, PARAMETER_ITEMHEIGHT);
	_itemWidth = _textBoundingRectangle.width();
	_itemHeight = _textBoundingRectangle.height();
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

// �]�witem���e��
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

// �e�~�ت��榡
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

// �e�Q�I���X�{���~��
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
	// �e�u��e����paint���@�ˡA���}��
}

void ItemComponent::setScene( ERDiagramScene* erDiagramScene )
{
	_scene = erDiagramScene;
}

void ItemComponent::updatePosition()
{
	// �Ѥl���O�갵
}

void ItemComponent::setPath()
{
	// �Ѥl���O�갵
}

void ItemComponent::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
	QGraphicsItem::mousePressEvent(event);
}

void ItemComponent::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
	QGraphicsItem::mouseReleaseEvent(event);
	// ��m�����ܡA�I�sscene�h��s
	_scene->updateItemPosition();
}

void ItemComponent::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
	QGraphicsItem::mouseMoveEvent(event);
	// ��m�����ܡA�I�sscene�h��s
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