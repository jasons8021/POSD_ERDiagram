#ifndef ERDIAGRAMSCENE_H_
#define ERDIAGRAMSCENE_H_

#define COMMA ","
#define SEMICOLON ";"
#define PARAMETER_COMPONENTDATA 0
#define PARAMETER_CONNECTIONDATA 1
#define PARAMETER_PRIMARYKEYDATA 2
#define PARAMETER_TYPE 0
#define PARAMETER_TEXT 1
#define PARAMETER_SOURCEITEMID 0
#define PARAMETER_DESTIONATIONITEMID 1
#define PARAMETER_CONNECTIONITEMTEXT 2
#define PARAMETER_ATTRIBUTE "A"
#define PARAMETER_ENTITY "E"
#define PARAMETER_RELATIONSHIP "R"
#define PARAMETER_CONNECTOR "C"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QPointF>
#include "ItemEntity.h"
#include "ItemComponent.h"
#include "ItemFactory.h"

class ERDiagramScene : public QGraphicsScene
{
	Q_OBJECT
public:
	ERDiagramScene(QObject* parent = 0);
	virtual ~ERDiagramScene();
	void addAllItem(QVector<QString>);
	ItemComponent* addComponent(QStringList);
	ItemComponent* addConnection(QStringList);
	void updatePlaceItemPosition(QString);
	QPointF getPlaceItemPosition(QString);
	QVector<QStringList> splitTextData(QString);
private:
	ItemFactory _itemFactory;
	QVector<ItemComponent*> _guiItem;
	QPointF _itemAttributePos;
	QPointF _itemEntityPos;
	QPointF _itemRelationshipPos;
};

#endif