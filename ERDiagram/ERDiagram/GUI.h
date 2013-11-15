#ifndef GUI_H_
#define GUI_H_

#include <QtGui>
#include <QtGui/QMainWindow>
#include <QMenu>
#include <QFileDialog>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QWidget>
#include <QAction>
#include <QString>

#include "ERDiagramScene.h"
#include "PresentationModel.h"

class GUI:public QMainWindow
{
	Q_OBJECT
public:
	GUI(PresentationModel*);
	virtual ~GUI();

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

	PresentationModel* _presentationModel;
};
#endif