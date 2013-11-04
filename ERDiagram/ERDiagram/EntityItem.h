#ifndef ENTITYITEM_H_
#define ENTITYITEM_H_

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
	EntityItem();
	virtual ~EntityItem();
	QRectF boundingRect() const;
	QPainterPath shape();
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* );
};

#endif

