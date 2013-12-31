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

#define ITEMIDINCREASE true
#define ITEMIDDECREASE false
#define ISFROMLOADFILE true
#define NOTFROMLOADFILE false

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QPointF>
#include "ItemComponent.h"
#include "ItemNode.h"
#include "ItemFactory.h"
#include "GuiState.h"
#include "PointerState.h"
#include "ConnectionState.h"
#include "AddAttributeState.h"
#include "AddEntityState.h"
#include "AddRelationshipState.h"
#include "GUI.h"
#include "Observer.h"
#include "SetPrimaryKeyState.h"

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
		RelationshipMode,
		SetPrimaryKeyMode
	};

	ERDiagramScene(QObject* parent = 0);
	virtual ~ERDiagramScene();
	int getCurrentMode();
	int adjustItemID(int);
	int searchERModelIDByItemID(int);
	int searchItemIDByERModelID(int);
	bool checkSetCardinality(int, int);
	bool getTargetAttributeIsPrimaryKey(int);
	void addNodeFromGUI(QString, QString, QPointF);
	void addNodeFromLoadFile(int, QStringList);
	void addConnectionFromGUI(ItemComponent*, ItemComponent*, QString);
	void addConnectionFromLoadFile(int, QStringList);
	void changeState(int);
	void changToPointerMode();
	void changeEditActionEnable(bool);
	void changeItemText(int, QString);
	void changePrimaryKey(int, bool);
	void deleteItem(int);
	void resetItemID();
	void LoadItem(QVector<QString>, bool);
	void mousePressEvent(QGraphicsSceneMouseEvent*);
	void mouseMoveEvent(QGraphicsSceneMouseEvent*);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
	void resetSelectedItem();
	void sceneMousePress(QGraphicsSceneMouseEvent*);
	void sceneMouseMove(QGraphicsSceneMouseEvent*);
	void sceneMouseRelease(QGraphicsSceneMouseEvent*);
	void setItemIDandERModelID(ItemComponent*, int);
	void updatePlaceItemPosition(QString);
	void updateItemPositionInScene();
	void updateAddNewItem(int, QString, QString, QPointF);
	void updateConnection(int, int, int, QString);
	void updateDeleteItem(QString);
	void updateReBuildConnection(QString);
	void updateItemPosition(int, QPointF);
	void setSelectedItem(QVector<int>);
	void moveGroupItemPositionByCmd(QVector<int>, QPointF);
	void setInitialItemPosition(int, QPointF, bool);
	QString getTargetItemType(int);
	QPointF getPlaceItemPosition(QString);
	ItemComponent* getItemInPosition(QPointF);
	ItemComponent* searchItemByERModelID(int);
	ItemComponent* addNode(int, QString, QString, QPointF);
	ItemComponent* addConnection(int, ItemComponent*, ItemComponent*, QString);
	QVector<QStringList> splitTextData(QString);
	QVector<int> searchItemIsSelected();
private:
	int _currentMode;
	GUI* _gui;
	QPointF _itemAttributePos;
	QPointF _itemEntityPos;
	QPointF _itemRelationshipPos;
	GuiState* _state;
	ItemFactory _itemFactory;
	ItemComponent* _previewItem;
	QVector<ItemComponent*> _guiItem;
};

#endif