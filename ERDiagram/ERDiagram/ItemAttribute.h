#ifndef ItemAttribute_H_
#define ItemAttribute_H_

#define PARAMETER_ATTRIBUTEITEMSHAPE "Ellipse"
#define PARAMETER_ATTRIBUTE "A"

#include <QRect>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>
#include <QPainterPath>
#include "ItemNode.h"

class ItemAttribute : public ItemNode
{
public:
	ItemAttribute(int,int,QString);
	ItemAttribute(int, int,int,QString);
	virtual ~ItemAttribute();
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* );
	void setPrimaryKey(bool);
	bool getPrimaryKey();
	void setPath();
	void updatePosition();
private:
	bool _isPrimaryKey;
};

#endif