#include "GUI.h"

GUI::GUI(PresentationModel* presentationModel)
{
	_presentationModel = presentationModel;
	createActions();
	createMenus();
	createToolbars();

	_scene = new ERDiagramScene(this);
	_scene->setSceneRect(QRectF(0, 0, SCENE_HEIGHT, SCENE_WIDTH));

	QHBoxLayout *layout = new QHBoxLayout;
	_view = new QGraphicsView(_scene);
	layout->addWidget(_view);

	QWidget *widget = new QWidget;
	widget->setLayout(layout);

	
	setCentralWidget(widget);
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

	// state的Menu
	_toolBar = addToolBar(tr("State"));

	_pointerButton = new QToolButton();
	_pointerButton->setIcon(QIcon("images/pointer.png"));
	_pointerButton->setCheckable(true);
	_pointerButton->setChecked(true);

	_connectionButton = new QToolButton();
	_connectionButton->setIcon(QIcon("images/connector.png"));
	_pointerButton->setCheckable(true);

	_toolBar->addWidget(_pointerButton);
	_toolBar->addWidget(_connectionButton);

	_toolBar->addSeparator();

	_addAttributeButton = new QToolButton();
	_addAttributeButton->setIcon(QIcon("images/circle.png"));
	_pointerButton->setCheckable(true);

	_addEntityButton = new QToolButton();
	_addEntityButton->setIcon(QIcon("images/rectangle.png"));
	_pointerButton->setCheckable(true);

	_addRelationshipButton = new QToolButton();
	_addRelationshipButton->setIcon(QIcon("images/diamond.png"));
	_pointerButton->setCheckable(true);

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
		_scene->addAllItem(inputFileText);
	}
}
