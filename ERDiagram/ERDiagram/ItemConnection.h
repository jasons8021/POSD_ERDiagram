#ifndef ITEMCONNECTION_H_
#define ITEMCONNECTION_H_

#define PARAMETER_CONNECTIONITEMSHAPE "Line"
#define PARAMETER_CONNECTOR "C"

#include <QRect>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>
#include <QPainterPath>
#include <QPen>
#include <QLineF>
#include <QtGui>
#include "ItemComponent.h"
#include <QDebug>

class ItemConnection : public ItemComponent
{
public:
	//ItemConnection();
	ItemConnection(int, ItemComponent*, ItemComponent*, QString, bool);
	virtual ~ItemConnection();
	QRectF boundingRect() const;
	QPainterPath shape() const;
	QPointF getScenePosition(ItemComponent*);
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* );
	void setPath(QVector<QPointF>);
	void paintBorder(QPainter*);
	void updatePosition();
	void setSourceDestinationPoint();
	void setSourceItem(ItemComponent*);
	void setDestionationItem(ItemComponent*);
	bool getSetCardinality();
	QLineF getConnectedLine();
private:
	ItemComponent* _sourceItem;
	ItemComponent* _destionationItem;
	QPointF _centerPos;
	QLineF _connectedLine;
	bool _isSetCardinality;
};

#endif