#ifndef ITEMENTITY_H_
#define ITEMENTITY_H_

#define PARAMETER_ENTITYITEMSHAPE "Rectangle"

#include <QRect>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>
#include <QPainterPath>
#include <QString>
#include <QtGui>
#include "ItemNode.h"

class ItemEntity : public ItemNode/*ItemComponent*/
{
public:
	ItemEntity(int, int, QString);
	virtual ~ItemEntity();
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* );
	void setPath();
	void updatePosition();
};

#endif

