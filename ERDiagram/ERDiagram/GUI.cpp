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

	_informationLabel = new QLabel;

	_tableViewModel = new TableViewModel();
	_tableView = new ComponentTableView(this, _tableViewModel);

	QVBoxLayout* layout_vr = new QVBoxLayout;

	//設置停靠窗口
	_dock = new QDockWidget(tr("Table View"),this);
	_dock->setFeatures(QDockWidget::DockWidgetClosable);
	_dock->setMinimumHeight(DOCK_MINHEIGHT);
	_textEditor = new QTextEdit();
	_textEditor->setReadOnly(true);
	_dock->setWidget(_textEditor);
	addDockWidget(Qt::BottomDockWidgetArea,_dock);

	layout_v->addWidget(label);
	layout_v->addWidget(_tableView);
	layout_vr->addWidget(_view);
	layout_vr->addWidget(_dock);
	layout_h->addLayout(layout_vr);
	layout_h->addLayout(layout_v);
	
	_dock->setVisible(false);

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
	_exitAction->setShortcut(tr("Ctrl+E"));
	connect(_exitAction, SIGNAL(triggered()), this, SLOT(close()));

	_addAttributeAction = new QAction(QIcon("images/circle.png"), tr("Add Attribute"), this);
	connect(_addAttributeAction, SIGNAL(triggered()), this, SLOT(addAttributeClicked()));

	_addEntityAction = new QAction(QIcon("images/rectangle.png"), tr("Add Entity"), this);
	connect(_addEntityAction, SIGNAL(triggered()), this, SLOT(addEntityClicked()));

	_addRelationshipAction = new QAction(QIcon("images/diamond.png"), tr("Add Relationship"), this);
	connect(_addRelationshipAction, SIGNAL(triggered()), this, SLOT(addRelationshipClicked()));

	_deleteAction = new QAction(QIcon("images/delete.png"), tr("Delete"), this);
	_deleteAction->setShortcut(tr("Del"));
	connect(_deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));
	_deleteAction->setEnabled(false);

	_undoAction = new QAction(QIcon("images/undo.png"), tr("Undo"), this);
	_undoAction->setShortcut(tr("Ctrl+Z"));
	connect(_undoAction, SIGNAL(triggered()), this, SLOT(undo()));
	_undoAction->setEnabled(false);

	_redoAction = new QAction(QIcon("images/redo.png"), tr("Redo"), this);
	_redoAction->setShortcut(tr("Ctrl+Y"));
	connect(_redoAction, SIGNAL(triggered()), this, SLOT(redo()));
	_redoAction->setEnabled(false);

	_cutAction = new QAction(QIcon("images/cut.png"), tr("Cut"), this);
	_cutAction->setShortcut(tr("Ctrl+X"));
 	connect(_cutAction, SIGNAL(triggered()), this, SLOT(cutComponent()));
 	_cutAction->setEnabled(false);

	_copyAction = new QAction(QIcon("images/copy.png"), tr("Copy"), this);
	_copyAction->setShortcut(tr("Ctrl+C"));
 	connect(_copyAction, SIGNAL(triggered()), this, SLOT(copyComponent()));
 	_copyAction->setEnabled(false);

	_pasteAction = new QAction(QIcon("images/paste.png"), tr("Paste"), this);
	_pasteAction->setShortcut(tr("Ctrl+V"));
 	connect(_pasteAction, SIGNAL(triggered()), this, SLOT(pasteComponent()));
 	_pasteAction->setEnabled(false);

	_aboutAction = new QAction(QIcon("images/about.png"), tr("About"), this);
	connect(_aboutAction, SIGNAL(triggered()), this, SLOT(information()));

	_saveAsComponentAction = new QAction(QIcon("images/save.png"), tr("Save"), this);
	_saveAsComponentAction->setShortcut(tr("Ctrl+S"));
	connect(_saveAsComponentAction, SIGNAL(triggered()), this, SLOT(saveAsComponent()));

	_saveAsXmlAction = new QAction(QIcon("images/saveXml.png"), tr("Save as xml"), this);
	connect(_saveAsXmlAction, SIGNAL(triggered()), this, SLOT(saveAsXml()));

	_erDiagramAction = new QAction(QIcon("images/erdiagram.png"), tr("Show ERDiagram"), this);
	connect(_erDiagramAction, SIGNAL(triggered()), this, SLOT(getHTMLERDiagramTable()));
	_erDiagramAction->setCheckable(true);
}

void GUI::createMenus()
{
	// 開啟檔案 & 離開
	_menu = menuBar()->addMenu(tr("File"));
	_menu->addAction(_openAction);
	_menu->addSeparator();
	_menu->addAction(_erDiagramAction);
	_menu->addSeparator();
	_menu->addAction(_saveAsComponentAction);
	_menu->addAction(_saveAsXmlAction);
	_menu->addSeparator();
	_menu->addAction(_exitAction);

	_menu = menuBar()->addMenu(tr("Add"));
	_menu->addAction(_addAttributeAction);
	_menu->addAction(_addEntityAction);
	_menu->addAction(_addRelationshipAction);

	_menu = menuBar()->addMenu(tr("Edit"));
	_menu->addAction(_undoAction);
	_menu->addAction(_redoAction);
	_menu->addAction(_deleteAction);
	_menu->addAction(_cutAction);
	_menu->addAction(_copyAction);
	_menu->addAction(_pasteAction);

	_menu = menuBar()->addMenu(tr("Help"));
	_menu->addAction(_aboutAction);
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
	_toolBar->addAction(_saveAsComponentAction);
	_toolBar->addAction(_saveAsXmlAction);
	_toolBar->addAction(_exitAction);

	_toolBar->addSeparator();
	_toolBar->addAction(_undoAction);
	_toolBar->addAction(_redoAction);

	_toolBar->addSeparator();
	_toolBar->addAction(_deleteAction);

	_toolBar->addSeparator();
	_toolBar->addAction(_cutAction);

	_toolBar->addSeparator();
	_toolBar->addAction(_copyAction);

	_toolBar->addSeparator();
	_toolBar->addAction(_pasteAction);

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

	// ERDiagram
	_toolBar->addSeparator();
	_toolBar->addAction(_erDiagramAction);
}

//////////////////////////////////////////////////////////////////////////
//								SLOT Function							//
//////////////////////////////////////////////////////////////////////////

// GUI提供按了哪個按鈕，由Scene去根據按鈕決定該去的State
void GUI::buttonGroupClicked()
{
	_scene->changeState(_buttonGroup->checkedId());
}

// 利用Menu新增物件
void GUI::addAttributeClicked()
{
	_scene->changeState(ERDiagramScene::AttributeMode);
}

void GUI::addEntityClicked()
{
	_scene->changeState(ERDiagramScene::EntityMode);
}

void GUI::addRelationshipClicked()
{
	_scene->changeState(ERDiagramScene::RelationshipMode);
}

// about的功能
void GUI::information()
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::information(this, tr("QMessageBox::information()"), ABOUT);
	if (reply == QMessageBox::Ok)
		_informationLabel->setText(tr("OK"));
	else
		_informationLabel->setText(tr("Escape"));
}

//////////////////////////////////////////////////////////////////////////
//							使用到GUI的Function							//
//////////////////////////////////////////////////////////////////////////

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

void GUI::changeEditActionEnable( bool isEnable )
{
	_deleteAction->setEnabled(isEnable);
	_cutAction->setEnabled(isEnable);
	_copyAction->setEnabled(isEnable);
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
// slot function
void GUI::loadFile()
{
	QString directory = QFileDialog::getOpenFileName(this, tr("Find File"), "C://", tr("ERD File (*.erd)"));
	if (directory != "")
	{
		QVector<QString> inputFileText;
		
		// 因為改在addNode跟addConnection完成就notify一次，所以會先畫一次圖
		_presentationModel->loadERDiagram_TextUI(directory.toStdString());

		// 取得Component、Connection、PK的資訊，並轉成QT物件
		inputFileText.push_back(stringConvertQString(_presentationModel->getComponent_GUI()));
		inputFileText.push_back(stringConvertQString(_presentationModel->getConnection_GUI()));
		inputFileText.push_back(stringConvertQString(_presentationModel->getPrimaryKeySet_GUI()));

		_scene->LoadItem(inputFileText, _presentationModel->getPosFileExist());
		update();
	}
}

//////////////////////////////////////////////////////////////////////////
//						Observer Pattern (Observer)						//
//////////////////////////////////////////////////////////////////////////

// Subject(ERModel)的TEXT有變動時，會呼叫Notify通知GUI的updateTextChanged進行GUI的修改
void GUI::updateTextChanged( int targetNodeID, string editedText )
{
	int itemID = _scene->searchItemIDByERModelID(targetNodeID);
	_scene->changeItemText(targetNodeID, stringConvertQString(editedText));
	_tableViewModel->changeTargetRowText(itemID, _scene->getTargetItemType(itemID), stringConvertQString(editedText));
	_tableView->updateModel(_tableViewModel);
	updateERDiagramTable();
}

// 向Scene發出PK更改的訊息
void GUI::updatePrimaryKeyChanged( int targetNodeID, bool isPrimaryKey )
{
	_scene->changePrimaryKey(targetNodeID, isPrimaryKey);
	updateERDiagramTable();
}

void GUI::updateAddNewNode( int componentID, string type, string text, int sx, int sy )
{
	_scene->updateAddNewItem(componentID, stringConvertQString(type), stringConvertQString(text), QPointF(sx, sy));
	updateERDiagramTable();
}

void GUI::updateConnection( int componentID, int sourceNodeID, int destinationNodeID, string text, vector<pair<int, bool>> changePK )
{
	_scene->updateConnection(componentID, sourceNodeID, destinationNodeID, stringConvertQString(text));
	if (changePK.size() > 0)
		updatePrimaryKeyChanged(changePK[0].first, changePK[0].second);
	updateERDiagramTable();
}

void GUI::updateDeleteComplete( string deleteComponentIDSet )
{
	_scene->updateDeleteItem(stringConvertQString(deleteComponentIDSet));
	updateERDiagramTable();
}

void GUI::updateReBuildConnection( string relatedConnectionSet )
{
	_scene->updateReBuildConnection(stringConvertQString(relatedConnectionSet));
	updateERDiagramTable();
}

void GUI::updateItemPosition( int componentID, int moveDistance_x, int moveDistance_y )
{
	_scene->updateItemPosition(componentID, QPointF(moveDistance_x, moveDistance_y));
	updateERDiagramTable();
}

void GUI::updatePasteComponent( vector<int> pasteComponentIDSet )
{
	_scene->setSelectedItem(QVector<int>::fromStdVector(pasteComponentIDSet));
	updateERDiagramTable();
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
// slot Function
void GUI::deleteItem()
{
	QVector<int> beSelectedItemSet = _scene->searchItemIsSelected();
	int itemIsSelected;
	if (beSelectedItemSet.size() == 1)
	{
		itemIsSelected = beSelectedItemSet[0];
		_presentationModel->deleteCmd(itemIsSelected);
		changeUnRedoActionEnable();
	}
	else
		_presentationModel->deleteGroupCmd(beSelectedItemSet.toStdVector());

	_scene->resetSelectedItem();
	changeEditActionEnable(false);
}

// 按下Ctrl+X後，向PM發出剪下要求
// slot Function
void GUI::cutComponent()
{
	QVector<int> beSelectedItemSet = _scene->searchItemIsSelected();

	_presentationModel->cutComponentCmd(beSelectedItemSet.toStdVector());

	_scene->resetSelectedItem();
	changeEditActionEnable(false);
	_pasteAction->setEnabled(true);
	changeUnRedoActionEnable();
}

// 按下Ctrl+C後，向PM發出複製要求
// slot Function
void GUI::copyComponent()
{
	QVector<int> beSelectedItemSet = _scene->searchItemIsSelected();

	_presentationModel->copyComponent(beSelectedItemSet.toStdVector());
	_pasteAction->setEnabled(true);
	changeUnRedoActionEnable();
}

// 按下Ctrl+V後，向PM發出貼上要求
// slot function
void GUI::pasteComponent()
{
	_scene->resetSelectedItem();
	_presentationModel->pasteComponentCmd();
	changeUnRedoActionEnable();
}

// 取得現在的Model中的ComponentID是多少
int GUI::getERModelComponentID()
{
	return _presentationModel->getComponentID();
}

// 移動後通知Model位置改變
void GUI::movedItemPosition( QVector<int> targetIDSet, QPointF newPosition )
{
	_presentationModel->moveItemCmd(targetIDSet.toStdVector(), newPosition.x(), newPosition.y());
	changeUnRedoActionEnable();
}

// 從LoadFile中進來的Component，需要重新設定座標
void GUI::setInitialItemPosition( int targetID, QPointF initialPosition )
{
	_presentationModel->setInitialItemPosition(targetID, initialPosition.x(), initialPosition.y());
}


// 復原
// slot function
void GUI::undo()
{
	_presentationModel->undoCmd();
	changeUnRedoActionEnable();
}

// 重做
// slot function
void GUI::redo()
{
	_presentationModel->redoCmd();
	changeUnRedoActionEnable();
}

// 取得目標是否為PK
bool GUI::getTargetAttributeIsPrimaryKey( int erModelID )
{
	bool isPK = _presentationModel->getTargetAttributeIsPrimaryKey(erModelID);
	return isPK;
}

// 存成.erd檔
// slot function
void GUI::saveAsComponent()
{
	QString directory = QFileDialog::getSaveFileName(this, tr("Save File"), "C://", tr("ERD File (*.erd)"));
	if (directory != "")
		_presentationModel->saveERDiagram(qstringConvertString(directory));
}

// 存成.xml檔
// slot function
void GUI::saveAsXml()
{
	QString directory = QFileDialog::getSaveFileName(this, tr("Save File"), "C://", tr("XML File (*.xml)"));
	if (directory != "")
		_presentationModel->saveXmlComponent(qstringConvertString(directory));
}

void GUI::setInitialPrimaryKey( int targetNodeID, bool isPrimaryKey )
{
	_presentationModel->setInitialPrimaryKey(targetNodeID, isPrimaryKey);
}

QPointF GUI::getTargetNodePosition( int erModelID )
{
	vector<int> positionSet = _presentationModel->getTargetPosition(erModelID);
	return QPointF(positionSet[POSITIONX], positionSet[POSITIONY]);
}

void GUI::getHTMLERDiagramTable()
{
	// 如果被隱藏起來，點了ERD圖後會顯示ERD出來，所以在false抓資料，並將Table顯示出來
	if (!_dock->isVisible())
	{
		updateERDiagramTable();
		_dock->setVisible(true);
	}
	else
		_dock->setVisible(false);
}

void GUI::updateERDiagramTable()
{
	QString qMessage = stringConvertQString(_presentationModel->getHTMLERDiagramTable());
	_textEditor->setHtml(qMessage);
}
