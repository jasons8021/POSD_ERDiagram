#ifndef GUI_H_
#define GUI_H_

#define SCENE_HEIGHT 1600
#define SCENE_WIDTH 1200
#define VIEW_MINWIDTH 1200
#define VIEW_MAXWIDTH 1300

#define ABOUT "Entity Relation Diagraming Tool 1.0\nAuthor : 102598018@ntut"

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
#include <string>
#include <QTableView>
#include <QStandardItemModel>

#include "ERDiagramScene.h"
#include "PresentationModel.h"
#include "TableViewModel.h"
#include "ComponentTableView.h"
#include "Observer.h"

class ComponentTableView;

using namespace std;

class GUI : public QMainWindow, public Observer
{
	Q_OBJECT
public:
	GUI(PresentationModel*);
	virtual ~GUI();

	int getERModelComponentID();
	bool checkSetCardinality(int, int);
	void addNodeIntoTable(int, QString, QString);
	// 向PM發出要求
	void addNode(QString, QString, QPointF);
	void addConnection(int, int, QString);
	void changePrimaryKey(int, bool);
	void changeItemText(int, QString);
	void changeToPointerMode();
	// 改變Button的enable
	void changeDeleteActionEnable(bool);
	void changeUnRedoActionEnable();
	// 向Scene發出要求
	void updateAddNewNode(int, string, string, int, int);
	void updateConnection(int, int, int, string);
	void updateDeleteNode(int);
	void updatePrimaryKeyChanged(int, bool);
	void updateTextChanged(int, string);
	void updateDeleteComplete(string);
	void updateReBuildConnection(string);
	// 更改TableView
	void deleteTableRow(int);
	// Tool
	string qstringConvertString(QString);
	QString stringConvertQString(string);
	private slots:
		void loadFile();
		void buttonGroupClicked();
		void deleteItem();
		void undo();
		void redo();
		void addAttributeClicked();
		void addEntityClicked();
		void addRelationshipClicked();
		void information();
private:
	void createActions();
	void createMenus();
	void createToolbars();
	void createButton();

	QGraphicsView* _view;
	ERDiagramScene* _scene;

	QMenu* _menu;
	QToolBar* _toolBar;

	QAction* _openAction;
	QAction* _exitAction;
	QAction* _addAttributeAction;
	QAction* _addEntityAction;
	QAction* _addRelationshipAction;
	QAction* _deleteAction;
	QAction* _undoAction;
	QAction* _redoAction;
	QAction* _cutAction;
	QAction* _copyAction;
	QAction* _pasteAction;
	QAction* _aboutAction;

	QLabel* _informationLabel;

	QToolButton* _pointerButton;
	QToolButton* _connectionButton;
	QToolButton* _addAttributeButton;
	QToolButton* _addEntityButton;
	QToolButton* _addRelationshipButton;
	QToolButton* _setPrimaryKeyButton;
	QButtonGroup* _buttonGroup;

	TableViewModel* _tableViewModel;
	ComponentTableView* _tableView;

	PresentationModel* _presentationModel;
};
#endif