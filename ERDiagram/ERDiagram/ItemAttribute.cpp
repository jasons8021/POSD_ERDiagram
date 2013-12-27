#include "ItemAttribute.h"
#include "ERDiagramScene.h"

ItemAttribute::ItemAttribute( int erModelID, int sx, int sy, QString attributeText ) : ItemNode( erModelID, sx, sy, PARAMETER_ATTRIBUTE, attributeText )
{
	setPath();
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	setAcceptsHoverEvents(true);
	_isPrimaryKey = false;
}

// preview Item�ϥ�
ItemAttribute::ItemAttribute( int sx, int sy, QString attributeText ) : ItemNode( sx, sy, PARAMETER_ATTRIBUTE, attributeText )
{
	setPath();
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	setAcceptsHoverEvents(true);
	_isPrimaryKey = false;
}

ItemAttribute::~ItemAttribute()
{
}

QRectF ItemAttribute::boundingRect() const
{
	return ItemNode::boundingRect();
}

QPainterPath ItemAttribute::shape() const
{
	return ItemNode::shape();
}

// �e�XItem
void ItemAttribute::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	setIsUnderLine(_isPrimaryKey);
	ItemNode::paint(painter, option, widget);
}

// get/set PK
void ItemAttribute::setPrimaryKey( bool isPrimaryKey )
{
	_isPrimaryKey = isPrimaryKey;
}

bool ItemAttribute::getPrimaryKey()
{
	return _isPrimaryKey;
}

// Attbitue�O�@�Ӿ��ΡA�ҥH�[�JEllipse
void ItemAttribute::setPath()
{
	_qPainterPath.~QPainterPath();
	_qPainterPath = QPainterPath();
	_qPainterPath.addEllipse(_textBoundingRectangle);
}

void ItemAttribute::updatePosition()
{

}