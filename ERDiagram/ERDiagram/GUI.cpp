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
	
	// 右上角的Components
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

// string轉QString
QString GUI::stringConvertQString( string beforeConvertString )
{
	return QString::fromLocal8Bit(beforeConvertString.c_str());
}

// QString轉string
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
	// 開啟檔案 & 離開
	_menu = menuBar()->addMenu(tr("File","Exit"));
	_menu->addAction(_openAction);
	_menu->addSeparator();
	_menu->addAction(_exitAction);
}

// 產生Button
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
	// 開啟檔案 & 離開
	_toolBar = addToolBar(tr("ToolBars"));
	_toolBar->addAction(_openAction);
	_toolBar->addAction(_exitAction);

	_toolBar->addSeparator();
	_toolBar->addAction(_undoAction);
	_toolBar->addAction(_redoAction);

	_toolBar->addSeparator();
	_toolBar->addAction(_deleteAction);

	createButton();
	// state的Menu
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
//							使用到GUI的Function							//
//////////////////////////////////////////////////////////////////////////

// GUI提供按了哪個按鈕，由Scene去根據按鈕決定該去的State
void GUI::buttonGroupClicked()
{
	_scene->changeState(_buttonGroup->checkedId());
}

// buttonGroup轉為PointerButton
void GUI::changeToPointerMode()
{
	_buttonGroup->button((int)ERDiagramScene::PointerMode)->click();
}

// 將新的Node加入TableView中
void GUI::addNodeIntoTable( int rowNumber, QString type, QString text )
{
	// 先更新TableViewModel
	_tableViewModel->addNodeIntoModel(rowNumber, type, text);
	_tableView->updateModel(_tableViewModel);
}

void GUI::changeDeleteActionEnable( bool isEnable )
{
	_deleteAction->setEnabled(isEnable);
}

// 設定Undo/Redo的Enable
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

// 刪除TableView的一列
void GUI::deleteTableRow( int rowIndex )
{
	_tableViewModel->removeRow(rowIndex);
	_tableView->updateModel(_tableViewModel);
}
//////////////////////////////////////////////////////////////////////////
//							跟Scene溝通的Function						//
//////////////////////////////////////////////////////////////////////////

// 讀檔
void GUI::loadFile()
{
	QString directory = QFileDialog::getOpenFileName(this, tr("Find File"), "C://", tr("ERD File (*.erd)"));
	if (directory != "")
	{
		QVector<QString> inputFileText;
		_presentationModel->loadERDiagram_TextUI(directory.toStdString());

		// 取得Component、Connection、PK的資訊，並轉成QT物件
		inputFileText.push_back(stringConvertQString(_presentationModel->getComponent_GUI()));
		inputFileText.push_back(stringConvertQString(_presentationModel->getConnection_GUI()));
		inputFileText.push_back(stringConvertQString(_presentationModel->getPrimaryKey_GUI()));
		_scene->loadAllItem(inputFileText);
	}
}

// Subject(ERModel)的TEXT有變動時，會呼叫Notify通知GUI的updateTextChanged進行GUI的修改
void GUI::updateTextChanged( int targetNodeID, string editedText )
{
	int itemID = _scene->searchItemIDByERModelID(targetNodeID);
	_scene->changeItemText(targetNodeID, stringConvertQString(editedText));

	_tableViewModel->changeTargetRowText(itemID, _scene->getTargetItemType(itemID), stringConvertQString(editedText));

	_tableView->updateModel(_tableViewModel);
}

// 向Scene發出PK更改的訊息
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
//								跟PM溝通的Function						//
//////////////////////////////////////////////////////////////////////////

// 將QString 轉為String後，用PM加入Model中
void GUI::addNode( QString type_qstring, QString text_qstring, QPointF point )
{
	string type_string = qstringConvertString(type_qstring); 
	string text_string = qstringConvertString(text_qstring); 
	_presentationModel->addNodeCmd(type_string, text_string, point.x(), point.y());
	changeUnRedoActionEnable();
}

// 將addConnection的要求送到PM中，讓PM去跟Model講
void GUI::addConnection( int sourceNodeId, int destinationNodeId, QString cardinality )
{
	_presentationModel->addConnectionCmd_GUI(sourceNodeId, destinationNodeId, qstringConvertString(cardinality));
	changeUnRedoActionEnable();
}

// TableView進行修改後，像PM發出更改通知
void GUI::changeItemText( int targetItemID, QString editedText )
{
	_presentationModel->changeTextCmd(_scene->searchERModelIDByItemID(targetItemID), qstringConvertString(editedText));
	
	changeUnRedoActionEnable();
}

// 向PM發出要求說，PK改變了
void GUI::changePrimaryKey( int targetNodeID, bool isPrimaryKey )
{
	_presentationModel->changePrimaryKeyCmd(targetNodeID, isPrimaryKey);
	changeUnRedoActionEnable();
}

// 檢查是否要設Cardinality
bool GUI::checkSetCardinality( int sourceNodeID, int destinationNodeID )
{
	return _presentationModel->checkSetCardinality(sourceNodeID, destinationNodeID);
}

// 按下delete鍵後，向PM發出刪除要求
void GUI::deleteItem()
{
	int itemIsSelected = _scene->searchItemIsSelected();
	_presentationModel->deleteCmd(itemIsSelected);
	changeUnRedoActionEnable();
	_deleteAction->setEnabled(false);
}

// 取得現在的Model中的ComponentID是多少
int GUI::getERModelComponentID()
{
	return _presentationModel->getComponentID();
}

// 復原
void GUI::undo()
{
	_presentationModel->undoCmd();
	changeUnRedoActionEnable();
}

// 重做
void GUI::redo()
{
	_presentationModel->redoCmd();
	changeUnRedoActionEnable();
}
