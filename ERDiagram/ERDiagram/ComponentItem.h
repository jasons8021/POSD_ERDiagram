#ifndef COMPONENTITEM_H_
#define COMPONENTITEM_H_

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>

class ComponentItem : public QGraphicsItem
{
public:
	ComponentItem();
	virtual ~ComponentItem();
	virtual QRectF boundingRect() const = 0;
	virtual QPainterPath sharp() = 0;
	virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* ) = 0;
protected:
	QPainterPath _qPainterPath;
};

#endif