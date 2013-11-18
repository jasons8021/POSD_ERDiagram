#ifndef ItemAttribute_H_
#define ItemAttribute_H_

#define PARAMETER_ATTRIBUTEITEMSHAPE "Ellipse"

#include <QRect>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>
#include <QPainterPath>
#include "ItemComponent.h"

class ItemAttribute : public ItemComponent
{
public:
	ItemAttribute(int,int,QString);
	virtual ~ItemAttribute();
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* );
	void paintText(QPainter*);
	void setPrimaryKey(bool);
	bool getPrimaryKey();
	void setPath();
	void paintBorder(QPainter*);
	void updatePosition();
private:
	bool _isPrimaryKey;
};

#endif