#ifndef GUI_H_
#define GUI_H_
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
#include "EntityItem.h"
#include "AttributeItem.h"

QT_BEGIN_NAMESPACE
class DiagramScene;
class QGraphicsView;
class QAction;
QT_END_NAMESPACE

class GUI:public QMainWindow
{
	Q_OBJECT
public:
	GUI(PresentationModel*);
	virtual ~GUI();

	private slots:
		void browse();
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

	// Item
	EntityItem* _entityItem;
	AttributeItem* _attributeItem;

	PresentationModel* _presentationModel;
};
#endif