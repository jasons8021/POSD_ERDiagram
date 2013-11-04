#include "GUI.h"

GUI::GUI(PresentationModel* presentationModel)
{
	_presentationModel = presentationModel;
	createActions();
	createMenus();
	createToolbars();
	_scene = new ERDiagramScene(this);
	_scene->setSceneRect(QRectF(0, 0, 800, 600));

	_relationshipItem = new RelationshipItem(QPointF(200,200),QPointF(500,500));
	_relationshipItem->translate(0, 0);
	_scene->addItem(_relationshipItem);

	_entityItem = new EntityItem(QString::fromLocal8Bit("hello qstring E"));
	_entityItem->translate(400, 300);
	_scene->addItem(_entityItem);

	_attributeItem = new AttributeItem(QString::fromLocal8Bit("hello qstring A"));
	_attributeItem->translate(400, 300);
	_scene->addItem(_attributeItem);

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
	_openAction = new QAction(QIcon("images/openFile.png"), tr("O&pen..."), this);
	_openAction->setShortcut(tr("Ctrl+O"));
	connect(_openAction, SIGNAL(triggered()), this, SLOT(browse()));

	_exitAction = new QAction(QIcon("images/exit.png"), tr("E&xit"), this);
	_exitAction->setShortcut(tr("Alt+F4"));
	connect(_exitAction, SIGNAL(triggered()), this, SLOT(close()));
}

void GUI::createMenus()
{
	_fileMenu = menuBar()->addMenu(tr("&File","&Exit"));
	_fileMenu->addAction(_openAction);
	_fileMenu->addSeparator();
	_fileMenu->addAction(_exitAction);
}

void GUI::createToolbars()
{
	_fileToolBar = addToolBar(tr("File"));
	_fileToolBar->addAction(_openAction);
	_fileToolBar->addAction(_exitAction);
}

void GUI::browse()
{
	QString directory = QFileDialog::getOpenFileName(this, tr("Find File"), "C://", tr("ERD File (*.erd)"));
	if (directory != "")
	{
		string inputFileText;
		inputFileText = _presentationModel->loadERDiagram_TextUI(directory.toStdString());
		ERDiagramMessageBoxManager::showMessageBox(inputFileText);
	}
}