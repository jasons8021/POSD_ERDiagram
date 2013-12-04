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
	void addNode(QString, QString);
	bool addConnection(int, int, QString);
	void addNodeIntoTable(QString, QString);
	void updateInfo();
	void updateTextChanged(int, string);
	void changeItemText(int, QString);
	QString stringConvertQString(string);
	string qstringConvertString(QString);
	private slots:
		void loadFile();
		void buttonGroupClicked();
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

	TableViewModel* _tableViewModel;
	ComponentTableView* _tableView;

	PresentationModel* _presentationModel;
};
#endif