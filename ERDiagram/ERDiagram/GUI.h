#ifndef GUI_H_
#define GUI_H_

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

#include <QtGui/QMainWindow>
#include <QFileDialog>
#include <QMenu>
#include <QtGui>
#include <QLabel>
#include <QFileDialog>
#include <QWidget>
#include <QGraphicsItem>
#include "ERDiagramMessageBoxManager.h"
#include "ERDiagramScene.h"
#include "PresentationModel.h"
#include "ItemEntity.h"
#include "ItemAttribute.h"
#include "ItemRelationship.h"
#include "ItemConnection.h"
#include "ItemComponent.h"
#include "ItemFactory.h"

QT_BEGIN_NAMESPACE
class DiagramScene;
class QGraphicsView;
class QAction;
class QString;
QT_END_NAMESPACE

class GUI:public QMainWindow
{
	Q_OBJECT
public:
	GUI(PresentationModel*);
	virtual ~GUI();
	vector<vector<string>> splitTextData(string);
	void addAllItem(vector<string>);
	ItemComponent* addComponent(vector<string>);
	ItemComponent* addConnection(vector<string>);
	void updatePlaceItemPosition(string);
	QPointF getPlaceItemPosition(string);
	void showGUI();
	

	private slots:
		void loadFile();
private:
	void createActions();
	void createMenus();
	void createToolbars();

	QGraphicsView* _view;
	ERDiagramScene* _scene;
	//bar
	QMenu* _fileMenu;
	QToolBar* _fileToolBar;
	//QAction
	QAction* _exitAction;
	QAction* _openAction;
	// Item factory
	ItemFactory _itemFactory;
	// Item
	QVector<ItemComponent*> _guiItem;
	ItemEntity* _entityItem;
	ItemAttribute* _attributeItem;
	ItemRelationship* _relationshipItem;
	ItemConnection* _connectionItem;
	// Item Placed Position
	QPointF _itemAttributeLeftPos;
	QPointF _itemEntityLeftPos;
	QPointF _itemAttributeRightPos;
	QPointF _itemEntityRightPos;
	QPointF _itemRelationshipPos;
	bool _itemEntityLeftSide;
	bool _itemAttributeLeftSide;

	PresentationModel* _presentationModel;
};
#endif