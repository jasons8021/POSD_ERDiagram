#ifndef ITEMCOMPONENT_H_
#define ITEMCOMPONENT_H_

#define PARAMETER_ITEMHEIGHT 80
#define PARAMETER_ADJUSTWIDTH 100
#define PARAMETER_HALF 1/2
#define PARAMETER_ENTITYITEMSHAPE "Rectangle"
#define PARAMETER_ATTRIBUTEITEMSHAPE "Ellipse"
#define PARAMETER_RELATIONSHIPITEMSHAPE "Diamond"
#define PARAMETER_CONNECTIONITEMSHAPE "Line"
#define PARAMETER_POINTERMODE 0

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <QWidget>
#include <QLabel>
#include <QRect>
#include <QPainterPath>
#include <QGraphicsLineItem>

class ERDiagramScene;

class ItemComponent : public QGraphicsItem
{
public:
	ItemComponent(int ,QString, QString);
	ItemComponent(int, int, QString, QString);
	ItemComponent(int, int, int, QString, QString);
	virtual ~ItemComponent();
	int getItemWidth();
	void setItemWidth(int);
	int getItemHeight();
	void setItemHeight(int);
 	QPointF getItemCenter();
	void setPaintBorderFont(QPainter*);
	void paintBorder(QPainter*);
	void setScene(ERDiagramScene*);
	void mousePressEvent(QGraphicsSceneMouseEvent *);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *);
	void setItemID(int);
	int getItemID();
	void changeItemText(QString);
	int caculateTextWidth(QString);
	void setTextBoundingRectangle(int, int);
	void setERModelID(int);
	int getERModelID();
	QString getType();
	QString getText();
	void setPosition(QPointF);
	QPointF adjustPosition(QPointF);
	virtual void updatePosition();
	virtual void paintText(QPainter*, bool);
	virtual QRectF boundingRect() const;
	virtual QPainterPath shape() const;
	virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* );
	virtual void setPath();
protected:
	int _itemWidth;
	int _itemHeight;
	int _textWidth;
	QRect _textBoundingRectangle;
	QPainterPath _qPainterPath;
	int _sx;
	int _sy;
	QString _type;
	QString _text;
	ERDiagramScene* _scene;
	int _itemID;
	int _erModelID;
	QPointF mousePressPoint;
};

#endif