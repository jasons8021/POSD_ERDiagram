#ifndef ENTITYITEM_H_
#define ENTITYITEM_H_

#define PARAMETER_ENTITYITEMSHAPE "Rectangle"

#include <QRect>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>
#include <QPainterPath>
#include <QString>
#include <QtGui>
#include "ComponentItem.h"


class EntityItem : public ComponentItem
{
public:
	EntityItem(int, int, QString);
	virtual ~EntityItem();
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* );
	void paintText(QPainter*);
private:
	
};

#endif

