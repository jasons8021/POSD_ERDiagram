#ifndef GUI_H_
#define GUI_H_

#define SCENE_HEIGHT 1600
#define SCENE_WIDTH 1200
#define VIEW_MINWIDTH 1300
#define VIEW_MAXWIDTH 1400
#define TABLEVIEW_MINWIDTH 200
#define TABLEVIEW_MAXWIDTH 300
#define LETTER_ATTRIBUTE "A"
#define LETTER_ENTITY "E"
#define LETTER_RELATIONSHIP "R"
#define LETTER_CONNECTOR "C"
#define TYPE_ATTRIBUTE "Attribute"
#define TYPE_ENTITY "Entity"
#define TYPE_RELATIONSHIP "Relationship"
#define TYPE_CONNECTOR "Connector"

#include <QtGui>
#include <QtGui/QMainWindow>
#include <QMenu>
#include <QFileDialog>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QWidget>
#include <QAction>
#include <QString>
#include <QToolButton>
#include <QIcon>

#include "ERDiagramScene.h"
#include "PresentationModel.h"

class GUI : public QMainWindow
{
	Q_OBJECT
public:
	GUI(PresentationModel*);
	virtual ~GUI();

	void changeToPointerMode();
	void addNode(QString, QString);
	bool addConnection(int, int, QString);
	void addNodeIntoTable(QString, QString);
	private slots:
		void loadFile();
		void buttonGroupClicked();
		void test();
private:
	void createActions();
	void createMenus();
	void createToolbars();

	QGraphicsView* _view;
	ERDiagramScene* _scene;

	QMenu* _menu;
	QToolBar* _toolBar;

	QAction* _openAction;
	QAction* _exitAction;
	QAction* _testAction;

	QToolButton* _pointerButton;
	QToolButton* _connectionButton;
	QToolButton* _addAttributeButton;
	QToolButton* _addEntityButton;
	QToolButton* _addRelationshipButton;
	QButtonGroup* _buttonGroup;

	QStandardItemModel* _itemModel;
	QTableView* _tableView;

	PresentationModel* _presentationModel;
};
#endif