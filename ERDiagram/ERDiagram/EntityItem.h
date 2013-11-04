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
	EntityItem(QString);
	virtual ~EntityItem();
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* );
private:
	QString _entityText;
};

#endif

