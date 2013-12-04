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
#define PARAMETER_NOTFINDID -1

#define PRIVIEWITEMORIGINAL_X 0
#define PRIVIEWITEMORIGINAL_Y 0
#define NULLTEXT ""


#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QPointF>
#include "ItemComponent.h"
#include "ItemFactory.h"
#include "GuiState.h"
#include "PointerState.h"
#include "ConnectionState.h"
#include "AddAttributeState.h"
#include "AddEntityState.h"
#include "AddRelationshipState.h"
#include "GUI.h"
#include "Observer.h"

class GUI;

class ERDiagramScene : public QGraphicsScene
{
	Q_OBJECT
public:
	enum stateMode{
		PointerMode,
		ConnectionMode,
		AttributeMode,
		EntityMode,
		RelationshipMode
	};

	ERDiagramScene(QObject* parent = 0);
	virtual ~ERDiagramScene();
	void loadAllItem(QVector<QString>);
	void addNodeFromGUI(QPointF, QString, QString);
	void addNodeFromLoadFile(QStringList);
	ItemComponent* addNode(QPointF, QString, QString);
	void addConnectionFromGUI(ItemComponent*, ItemComponent*, QString);
	ItemComponent* addConnection(ItemComponent*, ItemComponent*, QString);
	void addConnectionFromLoadFile(QStringList);
	void updatePlaceItemPosition(QString);
	QPointF getPlaceItemPosition(QString);
	QVector<QStringList> splitTextData(QString);
	void updateItemPosition();
	void changeState(int);
	void mousePressEvent(QGraphicsSceneMouseEvent*);
	void mouseMoveEvent(QGraphicsSceneMouseEvent*);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
	int getCurrentMode();
	void changToPointerMode();
	int findItemId(ItemComponent*);
	void setItemID(ItemComponent*);
	QVector<ItemComponent*> getGUIItem();
	void changeItemText(int,QString);
private:
	int _currentMode;
	ItemFactory _itemFactory;
	QVector<ItemComponent*> _guiItem;
	QPointF _itemAttributePos;
	QPointF _itemEntityPos;
	QPointF _itemRelationshipPos;
	GuiState* _state;
	ItemComponent* _previewItem;
	GUI* _gui;
};

#endif