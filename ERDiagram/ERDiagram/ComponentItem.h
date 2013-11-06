#ifndef COMPONENTITEM_H_
#define COMPONENTITEM_H_

#define PARAMETER_ITEMHEIGHT 80
#define PARAMETER_ADJUSTWIDTH 100
#define PARAMETER_HALF 1/2
#define PARAMETER_ENTITYITEMSHAPE "Rectangle"
#define PARAMETER_ATTRIBUTEITEMSHAPE "Ellipse"
#define PARAMETER_RELATIONSHIPITEMSHAPE "Diamond"
#define PARAMETER_CONNECTIONITEMSHAPE "Line"

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>
#include <QLabel>
#include <QDebug>
#include <QRect>
#include <QPainterPath>

class ComponentItem : public QGraphicsItem
{
public:
	ComponentItem();
	ComponentItem(int, int, QString);
	virtual ~ComponentItem();

	int getItemWidth();
	void setItemWidth(int);
	int getItemHeight();
	void setItemHeight(int);
	void setPath(QString);
	void setPath(QVector<QPointF>);
 	QPointF getItemCenter();
	void paintBorder(QPainter*, QString);
	virtual void paintText(QPainter*, bool);
	virtual QRectF boundingRect() const = 0;
	virtual QPainterPath shape() const = 0;
	virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* ) = 0;
protected:
	int _itemWidth;
	int _itemHeight;
	int _textWidth;
	QRect _textBoundingRectangle;
	QPainterPath _qPainterPath;
//private:
	int _sx;
	int _sy;
	QString _text;
};

#endif