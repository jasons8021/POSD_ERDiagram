#ifndef ITEMNODE_H_
#define ITEMNODE_H_

#include <QRect>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>
#include <QPainterPath>
#include "ItemComponent.h"

class ItemNode : public ItemComponent
{
public:
	ItemNode(int, int, QString, QString);
	ItemNode(int, int, int, QString, QString);
	virtual ~ItemNode();
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
	void setPath();
	void setIsUnderLine(bool);
	void updatePosition();
private:
	bool _isUnderLine;
};


#endif
