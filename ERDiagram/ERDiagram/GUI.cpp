#include "GUI.h"
#include <QTableView>
#include <QStandardItemModel>

GUI::GUI(PresentationModel* presentationModel)
{
	_presentationModel = presentationModel;
	createActions();
	createMenus();
	createToolbars();

	_scene = new ERDiagramScene(this);
	_scene->setSceneRect(QRectF(0, 0, SCENE_HEIGHT, SCENE_WIDTH));

	QHBoxLayout* layout_h = new QHBoxLayout;
	_view = new QGraphicsView(_scene);
	_view->setMinimumWidth(VIEW_MINWIDTH);
	_view->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	
	// 右上角的Components
	QVBoxLayout* layout_v = new QVBoxLayout;
	QLabel* label = new QLabel("<p align=\"center\" style=\"background-color: #BFBFC0\"><font size=\"10\">Components</font></p>");

	_tableViewModel = new TableViewModel();
	_tableView = new ComponentTableView(_tableViewModel);

	layout_v->addWidget(label);
	layout_v->addWidget(_tableView);
	layout_h->addWidget(_view);
	layout_h->addLayout(layout_v);
	
	QWidget *widget = new QWidget;
	widget->setLayout(layout_h);

	setCentralWidget(widget);
	_view->setMouseTracking(true);
}

GUI::~GUI()
{
}

void GUI::createActions()
{
	_openAction = new QAction(QIcon("images/openFile.png"), tr("Open..."), this);
	_openAction->setShortcut(tr("Ctrl+O"));
	connect(_openAction, SIGNAL(triggered()), this, SLOT(loadFile()));

	_exitAction = new QAction(QIcon("images/exit.png"), tr("Exit"), this);
	_exitAction->setShortcut(tr("Alt+F4"));
	connect(_exitAction, SIGNAL(triggered()), this, SLOT(close()));
}

void GUI::createMenus()
{
	// 開啟檔案 & 離開
	_menu = menuBar()->addMenu(tr("&File","&Exit"));
	_menu->addAction(_openAction);
	_menu->addSeparator();
	_menu->addAction(_exitAction);
}

void GUI::createToolbars()
{
	// 開啟檔案 & 離開
	_toolBar = addToolBar(tr("File"));
	_toolBar->addAction(_openAction);
	_toolBar->addAction(_exitAction);

	// state Button
	_pointerButton = new QToolButton();
	_pointerButton->setCheckable(true);
	_pointerButton->setChecked(true);
	_pointerButton->setIcon(QIcon("images/pointer.png"));

	_connectionButton = new QToolButton();
	_connectionButton->setCheckable(true);
	_connectionButton->setIcon(QIcon("images/connector.png"));

	_addAttributeButton = new QToolButton();
	_addAttributeButton->setCheckable(true);
	_addAttributeButton->setIcon(QIcon("images/circle.png"));

	_addEntityButton = new QToolButton();
	_addEntityButton->setCheckable(true);
	_addEntityButton->setIcon(QIcon("images/rectangle.png"));

	_addRelationshipButton = new QToolButton();
	_addRelationshipButton->setCheckable(true);
	_addRelationshipButton->setIcon(QIcon("images/diamond.png"));

	_buttonGroup = new QButtonGroup();
	_buttonGroup->addButton(_pointerButton, ERDiagramScene::PointerMode);
	_buttonGroup->addButton(_connectionButton, ERDiagramScene::ConnectionMode);
	_buttonGroup->addButton(_addAttributeButton, ERDiagramScene::AttributeMode);
	_buttonGroup->addButton(_addEntityButton, ERDiagramScene::EntityMode);
	_buttonGroup->addButton(_addRelationshipButton, ERDiagramScene::RelationshipMode);
	connect(_buttonGroup, SIGNAL(buttonClicked(int)),
		this, SLOT(buttonGroupClicked()));

	// state的Menu
	_toolBar = addToolBar(tr("State"));
	_toolBar->addWidget(_pointerButton);
	_toolBar->addWidget(_connectionButton);

	_toolBar->addSeparator();

	_toolBar->addWidget(_addAttributeButton);
	_toolBar->addWidget(_addEntityButton);
	_toolBar->addWidget(_addRelationshipButton);
}

void GUI::loadFile()
{
	QString directory = QFileDialog::getOpenFileName(this, tr("Find File"), "C://", tr("ERD File (*.erd)"));
	if (directory != "")
	{
		QVector<QString> inputFileText;
		_presentationModel->loadERDiagram_TextUI(directory.toStdString());

		// 取得Component、Connection、PK的資訊，並轉成QT物件
		inputFileText.push_back(QString::fromLocal8Bit(_presentationModel->getComponent_GUI().c_str()));
		inputFileText.push_back(QString::fromLocal8Bit(_presentationModel->getConnection_GUI().c_str()));
		inputFileText.push_back(QString::fromLocal8Bit(_presentationModel->getPrimaryKey_GUI().c_str()));
		_scene->loadAllItem(inputFileText);
	}
}

void GUI::buttonGroupClicked()
{
	_scene->changeState(_buttonGroup->checkedId());
}

void GUI::changeToPointerMode()
{
	_buttonGroup->button((int)ERDiagramScene::PointerMode)->click();
}

void GUI::addNode( QString type_qs, QString text_qs )
{
	string type_s = string((const char *)type_qs.toLocal8Bit()); 
	string text_s = string((const char *)text_qs.toLocal8Bit()); 
	_presentationModel->addNodeCmd(type_s, text_s);
}

bool GUI::addConnection( int sourceNodeId, int destinationNodeId, QString cardinality )
{
	bool flag = _presentationModel->addConnectionCmd_GUI(sourceNodeId,destinationNodeId,"");
	return flag;
}

void GUI::addNodeIntoTable( QString type, QString text )
{
	// 先更新TableViewModel
	_tableViewModel->addNodeIntoModel(type, text);
	_tableView->updateModel(_tableViewModel);
}
