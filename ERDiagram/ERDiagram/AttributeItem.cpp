#include "AttributeItem.h"

AttributeItem::AttributeItem( QString attributeText )
{
	_attributeText = attributeText;
	_qPainterPath.addEllipse(QRect( 0, 0, 100, 80 ));
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	setAcceptsHoverEvents(true);
	_isPrimaryKey = false;
}

AttributeItem::~AttributeItem()
{
}

QRectF AttributeItem::boundingRect() const
{
	return _qPainterPath.boundingRect();
}

QPainterPath AttributeItem::shape() const
{
	return _qPainterPath;
}

void AttributeItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	// painter->drawRect(QRect( 10, 10, 100, 80 ));
	// �b�@�Ӥ�Τ��e�@�Ӿ��A0,0����Υ��W�����y�СA100���e�B80����
	painter->drawEllipse( QRect( 0, 0, 100, 80 ) );

	// �g�r
	QFont font;
	font.setStyleHint(QFont::Times, QFont::PreferAntialias);
	if(_isPrimaryKey)
		font.setUnderline(true);
	font.setBold(true);
	font.setPointSize(18);

	painter->setRenderHint(QPainter::Antialiasing);
	painter->setBrush(Qt::black);
	painter->setFont(font);
	painter->drawText( QRect( 30, 20, 200, 60 ), Qt::TextWordWrap, _attributeText);
}

void AttributeItem::setPrimaryKey( bool isPrimaryKey )
{
	_isPrimaryKey = isPrimaryKey;
}

bool AttributeItem::getPrimaryKey()
{
	return _isPrimaryKey;
}
