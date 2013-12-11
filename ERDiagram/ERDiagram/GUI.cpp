#include "GUI.h"

GUI::GUI(PresentationModel* presentationModel)
{
	_presentationModel = presentationModel;
	_presentationModel->attachObserver(this);
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

// string��QString
QString GUI::stringConvertQString( string beforeConvertString )
{
	return QString::fromLocal8Bit(beforeConvertString.c_str());
}

// QString��string
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

	_deleteAction = new QAction(QIcon("images/delete.png"), tr("Delete"), this);
	_deleteAction->setShortcut(tr("Del"));
	connect(_deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));
	_deleteAction->setEnabled(false);

	_undoAction = new QAction(QIcon("images/undo.png"), tr("Undo"), this);
	_undoAction->setShortcut(tr("Ctrl+z"));
	connect(_undoAction, SIGNAL(triggered()), this, SLOT(undo()));
	_undoAction->setEnabled(false);

	_redoAction = new QAction(QIcon("images/redo.png"), tr("Redo"), this);
	_redoAction->setShortcut(tr("Ctrl+Y"));
	connect(_redoAction, SIGNAL(triggered()), this, SLOT(redo()));
	_redoAction->setEnabled(false);
}

void GUI::createMenus()
{
	// �}���ɮ� & ���}
	_menu = menuBar()->addMenu(tr("File","Exit"));
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
	_toolBar = addToolBar(tr("ToolBars"));
	_toolBar->addAction(_openAction);
	_toolBar->addAction(_exitAction);

	_toolBar->addSeparator();
	_toolBar->addAction(_undoAction);
	_toolBar->addAction(_redoAction);

	_toolBar->addSeparator();
	_toolBar->addAction(_deleteAction);

	createButton();
	// state��Menu
	_toolBar->addSeparator();
	_toolBar->addWidget(_pointerButton);
	_toolBar->addWidget(_connectionButton);

	_toolBar->addSeparator();
	_toolBar->addWidget(_addAttributeButton);
	_toolBar->addWidget(_addEntityButton);
	_toolBar->addWidget(_addRelationshipButton);

	_toolBar->addSeparator();
	_toolBar->addWidget(_setPrimaryKeyButton);
}

//////////////////////////////////////////////////////////////////////////
//							�ϥΨ�GUI��Function							//
//////////////////////////////////////////////////////////////////////////

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

// �N�s��Node�[�JTableView��
void GUI::addNodeIntoTable( int rowNumber, QString type, QString text )
{
	// ����sTableViewModel
	_tableViewModel->addNodeIntoModel(rowNumber, type, text);
	_tableView->updateModel(_tableViewModel);
}

void GUI::changeDeleteActionEnable( bool isEnable )
{
	_deleteAction->setEnabled(isEnable);
}

// �]�wUndo/Redo��Enable
void GUI::changeUnRedoActionEnable()
{
	if (_presentationModel->getUndoCmdsSize() > 0)
		_undoAction->setEnabled(true);
	else
		_undoAction->setEnabled(false);

	if (_presentationModel->getRedoCmdsSize() > 0)
		_redoAction->setEnabled(true);
	else
		_redoAction->setEnabled(false);
}

// �R��TableView���@�C
void GUI::deleteTableRow( int rowIndex )
{
	_tableViewModel->removeRow(rowIndex);
	_tableView->updateModel(_tableViewModel);
}
//////////////////////////////////////////////////////////////////////////
//							��Scene���q��Function						//
//////////////////////////////////////////////////////////////////////////

// Ū��
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

// Subject(ERModel)��TEXT���ܰʮɡA�|�I�sNotify�q��GUI��updateTextChanged�i��GUI���ק�
void GUI::updateTextChanged( int targetNodeID, string editedText )
{
	int itemID = _scene->searchItemIDByERModelID(targetNodeID);
	_scene->changeItemText(targetNodeID, stringConvertQString(editedText));

	_tableViewModel->changeTargetRowText(itemID, _scene->getTargetItemType(itemID), stringConvertQString(editedText));

	_tableView->updateModel(_tableViewModel);
}

// �VScene�o�XPK��諸�T��
void GUI::updatePrimaryKeyChanged( int targetNodeID, bool isPrimaryKey )
{
	_scene->changePrimaryKey(targetNodeID, isPrimaryKey);
}

void GUI::updateAddNewNode( int componentID, string type, string text, int sx, int sy )
{
	_scene->updateAddNewItem(componentID, stringConvertQString(type), stringConvertQString(text), QPointF(sx, sy));
}

void GUI::updateConnection( int componentID, int sourceNodeID, int destinationNodeID, string text )
{
	_scene->updateConnection(componentID, sourceNodeID, destinationNodeID, stringConvertQString(text));
}

void GUI::updateDeleteComplete( string deleteComponentIDSet )
{
	_scene->updateDeleteItem(stringConvertQString(deleteComponentIDSet));
}

void GUI::updateReBuildConnection( string relatedConnectionSet )
{
	_scene->updateReBuildConnection(stringConvertQString(relatedConnectionSet));
}

//////////////////////////////////////////////////////////////////////////
//								��PM���q��Function						//
//////////////////////////////////////////////////////////////////////////

// �NQString �ରString��A��PM�[�JModel��
void GUI::addNode( QString type_qstring, QString text_qstring, QPointF point )
{
	string type_string = qstringConvertString(type_qstring); 
	string text_string = qstringConvertString(text_qstring); 
	_presentationModel->addNodeCmd(type_string, text_string, point.x(), point.y());
	changeUnRedoActionEnable();
}

// �NaddConnection���n�D�e��PM���A��PM�h��Model��
void GUI::addConnection( int sourceNodeId, int destinationNodeId, QString cardinality )
{
	_presentationModel->addConnectionCmd_GUI(sourceNodeId, destinationNodeId, qstringConvertString(cardinality));
	changeUnRedoActionEnable();
}

// TableView�i��ק��A��PM�o�X���q��
void GUI::changeItemText( int targetItemID, QString editedText )
{
	_presentationModel->changeTextCmd(_scene->searchERModelIDByItemID(targetItemID), qstringConvertString(editedText));
	
	changeUnRedoActionEnable();
}

// �VPM�o�X�n�D���APK���ܤF
void GUI::changePrimaryKey( int targetNodeID, bool isPrimaryKey )
{
	_presentationModel->changePrimaryKeyCmd(targetNodeID, isPrimaryKey);
	changeUnRedoActionEnable();
}

// �ˬd�O�_�n�]Cardinality
bool GUI::checkSetCardinality( int sourceNodeID, int destinationNodeID )
{
	return _presentationModel->checkSetCardinality(sourceNodeID, destinationNodeID);
}

// ���Udelete���A�VPM�o�X�R���n�D
void GUI::deleteItem()
{
	int itemIsSelected = _scene->searchItemIsSelected();
	_presentationModel->deleteCmd(itemIsSelected);
	changeUnRedoActionEnable();
	_deleteAction->setEnabled(false);
}

// ���o�{�b��Model����ComponentID�O�h��
int GUI::getERModelComponentID()
{
	return _presentationModel->getComponentID();
}

// �_��
void GUI::undo()
{
	_presentationModel->undoCmd();
	changeUnRedoActionEnable();
}

// ����
void GUI::redo()
{
	_presentationModel->redoCmd();
	changeUnRedoActionEnable();
}
