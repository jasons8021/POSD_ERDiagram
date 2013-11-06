#include "GUI.h"
#include "..\src\corelib\io\qdebug.h"

GUI::GUI(PresentationModel* presentationModel)
{
	_itemAttributeLeftPos = QPointF(0,50);
	_itemEntityLeftPos = QPointF(350,50);
	_itemRelationshipPos = QPointF(700,50);

	_presentationModel = presentationModel;
	createActions();
	createMenus();
	createToolbars();

	_scene = new ERDiagramScene(this);
	_scene->setSceneRect(QRectF(0, 0, 1300, 700));

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
	connect(_openAction, SIGNAL(triggered()), this, SLOT(loadFile()));

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

void GUI::loadFile()
{
	QString directory = QFileDialog::getOpenFileName(this, tr("Find File"), "C://", tr("ERD File (*.erd)"));
	if (directory != "")
	{
		vector<string> inputFileText;
		_presentationModel->loadERDiagram_TextUI(directory.toStdString());
		inputFileText.push_back(_presentationModel->getComponent_GUI());
		inputFileText.push_back(_presentationModel->getConnection_GUI());
		inputFileText.push_back(_presentationModel->getPrimaryKey_GUI());
		addAllItem(inputFileText);
		showGUI();
	}
}

void GUI::addAllItem( vector<string> inputFileText )
{
	int connectionCounter = 0;
	vector<vector<string>> resultComponentData = splitTextData(inputFileText[PARAMETER_COMPONENTDATA]);
	vector<vector<string>> resultConnectionData = splitTextData(inputFileText[PARAMETER_CONNECTIONDATA]);
	vector<string> resultPrimaryKey = Toolkit::splitFunction(inputFileText[PARAMETER_PRIMARYKEYDATA], COMMA);

	for(int i = 0; i < resultComponentData.size(); i++)
	{
		if (resultComponentData[i][PARAMETER_TYPE] != PARAMETER_CONNECTOR)
			_guiItem.push_back(addComponent(resultComponentData[i]));
		else
		{
			_guiItem.push_back(addConnection(resultConnectionData[connectionCounter]));
			connectionCounter++;
		}
	}

	for ( int i = 0; i < resultPrimaryKey.size(); i++)
		static_cast<ItemAttribute*>(_guiItem[i])->setPrimaryKey(true);
}

vector<vector<string>> GUI::splitTextData( string textData )
{
	vector<vector<string>> resultTextData;
	vector<string> textDataRowSet = Toolkit::splitFunction(textData, SEMICOLON);
	for (int i = 0; i < textDataRowSet.size(); i++)
		resultTextData.push_back(Toolkit::splitFunction(textDataRowSet[i], COMMA));

	return resultTextData;
}


QPointF GUI::getPlaceItemPosition( string type )
{
	QPointF itemPos;
	if (type == PARAMETER_ENTITY)
		itemPos = _itemEntityLeftPos;
	else if (type == PARAMETER_ATTRIBUTE)
		itemPos = _itemAttributeLeftPos;
	else if (type == PARAMETER_RELATIONSHIP)
		itemPos = _itemRelationshipPos;

	updatePlaceItemPosition(type);

	return itemPos;
}

void GUI::updatePlaceItemPosition( string type )
{
	if (type == PARAMETER_ENTITY)
		_itemEntityLeftPos.setY(_itemEntityLeftPos.y() + 125);
	else if (type == PARAMETER_ATTRIBUTE)
		_itemAttributeLeftPos.setY(_itemAttributeLeftPos.y() + 125);
	else if (type == PARAMETER_RELATIONSHIP)
		_itemRelationshipPos.setY(_itemRelationshipPos.y() + 125);
}

void GUI::showGUI()
{
	for (int i = 0; i < _guiItem.size(); i++)
		_scene->addItem(_guiItem[i]);
}

ItemComponent* GUI::addConnection( vector<string> connecionData )
{
	ItemComponent* newItem;
	ItemFactory* itemFactory = new ItemFactory();

	int sourceItemId = atoi(connecionData[PARAMETER_SOURCEITEMID].c_str());
	int destionationItemId = atoi(connecionData[PARAMETER_DESTIONATIONITEMID].c_str());
	QString text =  QString::fromLocal8Bit(connecionData[PARAMETER_CONNECTIONITEMTEXT].c_str());;

	newItem = static_cast<ItemComponent*>(itemFactory->creatItemConnection(_guiItem[sourceItemId], _guiItem[destionationItemId], text));

	delete itemFactory;

	return newItem;
}

ItemComponent* GUI::addComponent(vector<string> componentData )
{
	QPointF itemPos;
	ItemComponent* newItem;
	ItemFactory* itemFactory = new ItemFactory();
	QString type =  QString::fromLocal8Bit(componentData[PARAMETER_TYPE].c_str());;
	QString text =  QString::fromLocal8Bit(componentData[PARAMETER_TEXT].c_str());;

	qDebug()<<type;

	itemPos = getPlaceItemPosition(componentData[PARAMETER_TYPE]);
	newItem = static_cast<ItemComponent*>(itemFactory->creatItem(itemPos.x(), itemPos.y(), type, text));

	delete itemFactory;

	return newItem;
}
