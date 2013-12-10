#ifndef GUI_H_
#define GUI_H_

#define SCENE_HEIGHT 1600
#define SCENE_WIDTH 1200
#define VIEW_MINWIDTH 1200
#define VIEW_MAXWIDTH 1300

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

	void changeToPointerMode();
	void addNode(QString, QString, QPointF);
	void addConnection(int, int, QString);
	void addNodeIntoTable(QString, QString);
	void updateInfo();
	void updateTextChanged(int, string);
	void changeItemText(int, QString);
	void updatePrimaryKeyChanged(int, bool);
	void changePrimaryKey(int, bool);
	void updateAddNewNode(string, string, int, int);
	void updateConnection(int, int, string);
	void updateDeleteNode(int);
	QString stringConvertQString(string);
	string qstringConvertString(QString);
	bool checkSetCardinality(int, int);
	void changeDeleteActionEnable(bool);
	void changeUnRedoActionEnable();
	int getERModelComponentID();
	void deleteTableRow(int);
	private slots:
		void loadFile();
		void buttonGroupClicked();
		void deleteItem();
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
	QAction* _deleteAction;
	QAction* _undoAction;
	QAction* _redoAction;

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