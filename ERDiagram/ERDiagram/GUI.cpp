#include "GUI.h"

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
	
	// �k�W����Components
	QVBoxLayout* layout_v = new QVBoxLayout;
	QLabel* label = new QLabel("<p align=\"center\" style=\"background-color: #BFBFC0\"><font size=\"10\">Components</font></p>");

	_tableViewModel = new TableViewModel();
	_tableView = new ComponentTableView(this, _tableViewModel);

	QTableView* tabelView = new QTableView();
	tabelView->horizontalHeader()->setStretchLastSection(true);

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

QString GUI::stringConvertQString( string beforeConvertString )
{
	return QString::fromLocal8Bit(beforeConvertString.c_str());
}

string GUI::qstringConvertString( QString beforeConvertQString )
{
	return string((const char *)beforeConvertQString.toLocal8Bit());
}

void GUI::createActions()
{
	_openAction = new QAction(QIcon("images/openFile.png"), tr("Open..."), this);
	_openAction->setShortcut(tr("Ctrl+O"));
	connect(_openAction, SIGNAL(triggered()), this, SLOT(loadFile()));

	_exitAction = new QAction(QIcon("images/exit.png"), tr("Exit"), this);
	_exitAction->setShortcut(tr("Alt+F4"));
	connect(_exitAction, SIGNAL(triggered()), this, SLOT(close()));

	_presentationModel->attachObserver(this);
}

void GUI::createMenus()
{
	// �}���ɮ� & ���}
	_menu = menuBar()->addMenu(tr("&File","&Exit"));
	_menu->addAction(_openAction);
	_menu->addSeparator();
	_menu->addAction(_exitAction);
}

// ����Button
void GUI::createButton()
{
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

	_setPrimaryKeyButton = new QToolButton();
	_setPrimaryKeyButton->setCheckable(true);
	_setPrimaryKeyButton->setIcon(QIcon("images/key.png"));

	_buttonGroup = new QButtonGroup();
	_buttonGroup->addButton(_pointerButton, ERDiagramScene::PointerMode);
	_buttonGroup->addButton(_connectionButton, ERDiagramScene::ConnectionMode);
	_buttonGroup->addButton(_addAttributeButton, ERDiagramScene::AttributeMode);
	_buttonGroup->addButton(_addEntityButton, ERDiagramScene::EntityMode);
	_buttonGroup->addButton(_addRelationshipButton, ERDiagramScene::RelationshipMode);
	_buttonGroup->addButton(_setPrimaryKeyButton, ERDiagramScene::SetPrimaryKeyMode);
	connect(_buttonGroup, SIGNAL(buttonClicked(int)),
		this, SLOT(buttonGroupClicked()));
}

void GUI::createToolbars()
{
	// �}���ɮ� & ���}
	_toolBar = addToolBar(tr("File"));
	_toolBar->addAction(_openAction);
	_toolBar->addAction(_exitAction);

	createButton();
	// state��Menu
	_toolBar = addToolBar(tr("State"));
	_toolBar->addWidget(_pointerButton);
	_toolBar->addWidget(_connectionButton);

	_toolBar->addSeparator();

	_toolBar->addWidget(_addAttributeButton);
	_toolBar->addWidget(_addEntityButton);
	_toolBar->addWidget(_addRelationshipButton);

	_toolBar->addSeparator();

	_toolBar->addWidget(_setPrimaryKeyButton);
}


void GUI::loadFile()
{
	QString directory = QFileDialog::getOpenFileName(this, tr("Find File"), "C://", tr("ERD File (*.erd)"));
	if (directory != "")
	{
		QVector<QString> inputFileText;
		_presentationModel->loadERDiagram_TextUI(directory.toStdString());

		// ���oComponent�BConnection�BPK����T�A���নQT����
		inputFileText.push_back(stringConvertQString(_presentationModel->getComponent_GUI()));
		inputFileText.push_back(stringConvertQString(_presentationModel->getConnection_GUI()));
		inputFileText.push_back(stringConvertQString(_presentationModel->getPrimaryKey_GUI()));
		_scene->loadAllItem(inputFileText);
	}
}

// GUI���ѫ��F���ӫ��s�A��Scene�h�ھګ��s�M�w�ӥh��State
void GUI::buttonGroupClicked()
{
	_scene->changeState(_buttonGroup->checkedId());
}

// buttonGroup�ରPointerButton
void GUI::changeToPointerMode()
{
	_buttonGroup->button((int)ERDiagramScene::PointerMode)->click();
}

// �NQString �ରString��A��PM�[�JModel��
void GUI::addNode( QString type_qstring, QString text_qstring )
{
	string type_string = qstringConvertString(type_qstring); 
	string text_string = qstringConvertString(text_qstring); 
	_presentationModel->addNodeCmd(type_string, text_string);
}

bool GUI::addConnection( int sourceNodeId, int destinationNodeId, QString cardinality )
{
	bool flag = _presentationModel->addConnectionCmd_GUI(sourceNodeId,destinationNodeId,"");
	return flag;
}

// �N�s��Node�[�JTableView��
void GUI::addNodeIntoTable( QString type, QString text )
{
	// ����sTableViewModel
	_tableViewModel->addNodeIntoModel(type, text);
	_tableView->updateModel(_tableViewModel);
}

void GUI::updateInfo()
{
	
}

// Subject(ERModel)��TEXT���ܰʮɡA�|�I�sNotify�q��GUI��updateTextChanged�i��GUI���ק�
void GUI::updateTextChanged( int targetNodeID, string editedText )
{
	_scene->changeItemText(targetNodeID, stringConvertQString(editedText));
}

// TableView�i��ק��A��PM�o�X���q��
void GUI::changeItemText( int targetNodeID, QString editedText )
{
	_presentationModel->changeText(targetNodeID, qstringConvertString(editedText));
}

void GUI::updatePrimaryKeyChanged( int targetNodeID, bool isPrimaryKey )
{
	_scene->changePrimaryKey(targetNodeID, isPrimaryKey);
}

void GUI::changePrimaryKey( int targetNodeID, bool isPrimaryKey )
{
	_presentationModel->changePrimaryKey(targetNodeID, isPrimaryKey);
}

bool GUI::checkSetCardinality( int sourceNodeID, int destinationNodeID )
{
	return _presentationModel->checkSetCardinality(sourceNodeID, destinationNodeID);
}
