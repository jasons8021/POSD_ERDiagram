#include "ERDiagramScene.h"
#include "..\src\corelib\io\qdebug.h"

ERDiagramScene::ERDiagramScene(QObject* parent) : QGraphicsScene(parent)
{
	_itemAttributePos = QPointF(0,50);
	_itemEntityPos = QPointF(350,50);
	_itemRelationshipPos = QPointF(700,50);
	_state = new PointerState(this);
	_gui = static_cast<GUI*>(parent);
	_currentMode = PointerMode;
}

ERDiagramScene::~ERDiagramScene()
{
}

// 根據loadFile將全部的Item加進畫面中
void ERDiagramScene::loadAllItem( QVector<QString> inputFileText )
{
	int connectionCounter = 0;
	QVector<QStringList> resultComponentData = splitTextData(inputFileText[PARAMETER_COMPONENTDATA]);
	QVector<QStringList> resultConnectionData = splitTextData(inputFileText[PARAMETER_CONNECTIONDATA]);
	QStringList resultPrimaryKey = inputFileText[PARAMETER_PRIMARYKEYDATA].split(COMMA);
	
	for(int i = 0; i < resultComponentData.size(); i++)
	{
		if (resultComponentData[i].at(PARAMETER_TYPE) != PARAMETER_CONNECTOR)
			addNodeFromLoadFile(resultComponentData[i]);
		else
		{
			addConnectionFromLoadFile(resultConnectionData[connectionCounter]);
			connectionCounter++;
		}
	}

	// resultPrimaryKey是由ERModel傳過來的，所以要配合用erModelID
	for ( int i = 0; i < resultPrimaryKey.size(); i++)
		static_cast<ItemAttribute*>(searchItemByERModelID(resultPrimaryKey.at(i).toInt()))->setPrimaryKey(true);

}

// 加入Item到Scene上
ItemComponent* ERDiagramScene::addNode( int erModelID, QString type, QString text, QPointF point )
{
	ItemComponent* newItem;
	ItemFactory* itemFactory = new ItemFactory();

	newItem = itemFactory->creatItem(point.x(), point.y(), type, text);
	delete itemFactory;

	// 將newItem push進_guiItem中，並設定GUI上面的ItemID以及ERModel的ComponentID
	setItemIDandERModelID(newItem, erModelID);

	// 將scene設給Item
	newItem->setScene(this);
	// 將Item加入scene
	this->addItem(newItem);

	_gui->addNodeIntoTable(newItem->getItemID(), type, text);

	return newItem;
}

// 從輸入的檔案新增Item
void ERDiagramScene::addNodeFromLoadFile( QStringList componentData )
{ 
	QPointF itemPos;
	QString type =  componentData.at(PARAMETER_TYPE);
	QString text =  componentData.at(PARAMETER_TEXT);

	// 原本的erd檔沒有位置資訊，因此隨機假設位置進去
	itemPos = getPlaceItemPosition(componentData[PARAMETER_TYPE]);

	_gui->addNode(type, text, QPointF(itemPos.x(), itemPos.y()));
	update();
	_gui->update();
}

// 新增連結
ItemComponent* ERDiagramScene::addConnection( int erModelID, ItemComponent* sourceItem, ItemComponent* destionationItem, QString text )
{
	ItemComponent* newItem;
	ItemFactory* itemFactory = new ItemFactory();

	// 判斷是否為Cardinality，並設定進connectionItem中
	bool isSetCardinality = checkSetCardinality(sourceItem->getERModelID(), destionationItem->getERModelID());

	newItem = itemFactory->creatItemConnection(sourceItem, destionationItem, text, isSetCardinality);
	
	delete itemFactory;

	// 將newItem push進_guiItem中，並設定編號
	setItemIDandERModelID(newItem, erModelID);

	// 將scene設給Item
	newItem->setScene(this);
	// 將Item加入scene
	this->addItem(newItem);

	_gui->addNodeIntoTable(newItem->getItemID(), "Connector", text);

	return newItem;
}

// 從輸入的檔案新增兩個Item間的連結
void  ERDiagramScene::addConnectionFromLoadFile( QStringList connecionData )
{
	int sourceItemID = connecionData.at(PARAMETER_SOURCEITEMID).toInt();
	int destionationItemID = connecionData.at(PARAMETER_DESTIONATIONITEMID).toInt();
	QString text =  connecionData.at(PARAMETER_CONNECTIONITEMTEXT);

	_gui->addConnection(sourceItemID, destionationItemID, text);
}

void ERDiagramScene::deleteItem( int deleteItemID )
{
	// 刪除畫面上的Item
	this->removeItem(_guiItem[deleteItemID]);
	// 刪除TableView的Row
	_gui->deleteTableRow(deleteItemID);
	// 刪除_guiItem
	_guiItem.remove(deleteItemID);
}

//////////////////////////////////////////////////////////////////////////
//						跟位置有關的Function							//
//////////////////////////////////////////////////////////////////////////

// 供loadFile進來的資料設定位置
void ERDiagramScene::updatePlaceItemPosition( QString type )
{
	// 根據type，改變Y軸座標位置
	if (type == PARAMETER_ENTITY)
		_itemEntityPos.setY(_itemEntityPos.y() + 125);
	else if (type == PARAMETER_ATTRIBUTE)
		_itemAttributePos.setY(_itemAttributePos.y() + 125);
	else if (type == PARAMETER_RELATIONSHIP)
		_itemRelationshipPos.setY(_itemRelationshipPos.y() + 125);
}

QPointF ERDiagramScene::getPlaceItemPosition( QString type )
{
	QPointF itemPos;

	// 設定Item的位置
	if (type == PARAMETER_ENTITY)
		itemPos = _itemEntityPos;
	else if (type == PARAMETER_ATTRIBUTE)
		itemPos = _itemAttributePos;
	else if (type == PARAMETER_RELATIONSHIP)
		itemPos = _itemRelationshipPos;

	// 更新位置資訊
	updatePlaceItemPosition(type);

	return itemPos;
}

// 更新Item移動後的位置
void ERDiagramScene::updateItemPosition()
{
	for (QVector<ItemComponent *>::iterator index = _guiItem.begin(); index < _guiItem.end(); index++)
	{
		ItemComponent* itemComponent = ((ItemComponent *)*index);
		itemComponent->updatePosition();
	}
	update(0, 0, width(), height());
}

//////////////////////////////////////////////////////////////////////////
//					跟statePattern有關的Function						//
//////////////////////////////////////////////////////////////////////////

void ERDiagramScene::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
	_state->mousePressEvent(event);
}

void ERDiagramScene::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
	_state->mouseMoveEvent(event);
}

void ERDiagramScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
	_state->mouseReleaseEvent(event);
}

int ERDiagramScene::getCurrentMode()
{
	return _currentMode;
}

void ERDiagramScene::changeState( int stateMode )
{
	delete _state;
	_currentMode = stateMode;
	switch(stateMode)
	{
	case PointerMode:
		_state = new PointerState(this);
		break;
	case ConnectionMode:
		_state = new ConnectionState(this);
		break;
	case AttributeMode:
		_previewItem = new ItemAttribute(PRIVIEWITEMORIGINAL_X, PRIVIEWITEMORIGINAL_Y, NULLTEXT);
		_state = new AddAttributeState(this, _previewItem);
		break;
	case EntityMode:
		_previewItem = new ItemEntity(PRIVIEWITEMORIGINAL_X, PRIVIEWITEMORIGINAL_Y, NULLTEXT);
		_state = new AddEntityState(this, _previewItem);
		break;
	case RelationshipMode:
		_previewItem = new ItemRelationship(PRIVIEWITEMORIGINAL_X, PRIVIEWITEMORIGINAL_Y, NULLTEXT);
		_state = new AddRelationshipState(this, _previewItem);
		break;
	case SetPrimaryKeyMode:
		_state = new SetPrimaryKeyState(_gui, this);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
//							跟update系列的Function						//
//////////////////////////////////////////////////////////////////////////

// 收到observer的通知，加入新的Item進入Scene中(畫出來)
void ERDiagramScene::updateAddNewItem( int erModelID, QString type, QString text, QPointF point )
{
	addNode(erModelID, type, text, point);
}

// 收到observer的通知，加入Connection到Scene中(畫出來)
void ERDiagramScene::updateConnection( int erModelID, int sourceItemID, int destionationItemID, QString text )
{
	// 這邊收到的是ERModel那邊得到的ComponentID
	addConnection(erModelID, searchItemByERModelID(sourceItemID), searchItemByERModelID(destionationItemID), text);
}

// 由ERModel傳進來的，所以是ComponentID Set
void ERDiagramScene::updateDeleteItem( QString deleteComponentIDSet )
{
	QStringList deleteIDSet = deleteComponentIDSet.split(COMMA);
	for(int i = 0; i < deleteIDSet.size(); i++)
	{
		// 由於傳進來的值是ComponentID，所以要刪除要轉成ItemID來砍
		int deleteItemID = searchItemByERModelID(deleteIDSet.at(i).toInt())->getItemID();
		deleteItem(deleteItemID);
		resetItemID();
	}
}

//////////////////////////////////////////////////////////////////////////
//								跟GUI溝通的Function						//
//////////////////////////////////////////////////////////////////////////

// GUI介面新增Item到ERModel中(不含畫在Scene上)
void  ERDiagramScene::addNodeFromGUI( QString type, QString text, QPointF point )
{
	// 加入Item進ERModel
	_gui->addNode(type, text, point);
}

// 向GUI發出Connection的要求，使GUI去告訴PM
void ERDiagramScene::addConnectionFromGUI( ItemComponent* sourceItem, ItemComponent* destionationItem, QString text )
{
	_gui->addConnection(sourceItem->getERModelID(), destionationItem->getERModelID(), text);
}

void ERDiagramScene::changToPointerMode()
{
	_gui->changeToPointerMode();
}

// 這邊的targetNodeID是由Model傳過來的，所以targetNodeID是ERModelID
void ERDiagramScene::changeItemText( int targetNodeID, QString editedText )
{
	searchItemByERModelID(targetNodeID)->changeItemText(editedText);
	update();
}

void ERDiagramScene::changePrimaryKey( int targetNodeID, bool isPrimaryKey )
{
	// 這邊的targetNodeID是對應到TableView的RowNumber，所以是GUI這邊的ItemID
	static_cast<ItemAttribute*>(_guiItem[targetNodeID])->setPrimaryKey(isPrimaryKey);
	update();
}

void ERDiagramScene::changeEditActionEnable( bool isEnable )
{
	_gui->changeEditActionEnable(isEnable);
}

bool ERDiagramScene::checkSetCardinality( int sourceNodeID, int destinationNodeID )
{
	return _gui->checkSetCardinality(sourceNodeID, destinationNodeID);
}

//////////////////////////////////////////////////////////////////////////
//							Scene要用的Tool Function					//
//////////////////////////////////////////////////////////////////////////

// QString的字串切割，QString.split()會回傳QStringList，QStringList的用法是.at(index)
QVector<QStringList> ERDiagramScene::splitTextData( QString textData )
{
	QVector<QStringList> resultTextData;
	QStringList textDataRowSet = textData.split(SEMICOLON);
	for (int i = 0; i < textDataRowSet.size(); i++)
		resultTextData.push_back(textDataRowSet.at(i).split(COMMA));

	return resultTextData;
}

void ERDiagramScene::setItemIDandERModelID( ItemComponent* newItem, int erModelID )
{
	newItem->setERModelID(erModelID);
	_guiItem.insert(adjustItemID(erModelID), newItem);
	resetItemID();
}

// 重新調整ItemID，並回傳插入進去的ItemID
int ERDiagramScene::adjustItemID( int erModelID )
{
	// 假如_guiItem裡面是空的
	int baseLineItemID = 0;

	for(int i = 0; i < _guiItem.size(); i++)
	{
		if (_guiItem[i]->getERModelID() > erModelID)
			baseLineItemID = i;
		else
			baseLineItemID = _guiItem.size();
	}

	return baseLineItemID;
}

// isAdd為True時，reset比baseLineItemID大的ItemID+1 ; false則是-1
void ERDiagramScene::resetItemID()
{
	for(int i = 0; i < _guiItem.size(); i++)
		_guiItem[i]->setItemID(i);
}

// 用ERModel那邊的ComponentID來搜尋Item
ItemComponent* ERDiagramScene::searchItemByERModelID( int targetERModelID )
{
	for (int i = 0; i < _guiItem.size(); i++)
	{
		if (targetERModelID == _guiItem[i]->getERModelID())
			return _guiItem[i];
	}
	return NULL;
}

// 回傳被選中Item的ERModelID
QVector<int> ERDiagramScene::searchItemIsSelected()
{
	QVector<int> resultSet;

	// 找尋被點中的Item
	for (int i = 0; i < _guiItem.size(); i++)
	{
		if (_guiItem[i]->isSelected())
			resultSet.push_back(_guiItem[i]->getERModelID());
	}

	return resultSet;
}

void ERDiagramScene::updateReBuildConnection( QString relatedConnectionSet )
{
	QStringList ReBuildConnectionSet = relatedConnectionSet.split(SEMICOLON);

	// 最後一組是空白，所以不用加
	for(int i = 0; i < ReBuildConnectionSet.size()-1; i++)
	{
		// ReBuildConnectionContent格式為ReBuildConnectionContent[0]是ConnectorID、ReBuildConnectionContent[1]是sourceNodeID(erModelID)、
		// ReBuildConnectionContent[2]是destinationNodeID(erModelID)、ReBuildConnectionContent[3]是ConnectorText
		QStringList ReBuildConnectionContent = ReBuildConnectionSet.at(i).split(COMMA);

		addConnection(ReBuildConnectionContent.at(0).toInt(), searchItemByERModelID(ReBuildConnectionContent.at(1).toInt()), 
					  searchItemByERModelID(ReBuildConnectionContent.at(2).toInt()), ReBuildConnectionContent.at(3));
	}
}

int ERDiagramScene::searchERModelIDByItemID( int itemID )
{
	return _guiItem[itemID]->getERModelID();
}

// 用ERModel那邊的ComponentID來找ItemID
int ERDiagramScene::searchItemIDByERModelID( int targetERModelID )
{
	for (int i = 0; i < _guiItem.size(); i++)
	{
		if (targetERModelID == _guiItem[i]->getERModelID())
			return _guiItem[i]->getItemID();
	}
	return NULL;
}

QString ERDiagramScene::getTargetItemType( int itemID )
{
	return _guiItem[itemID]->getType();
}

ItemComponent* ERDiagramScene::getItemInPosition( QPointF pos )
{
	return static_cast<ItemComponent*>(itemAt(pos));
}

void ERDiagramScene::sceneMousePress( QGraphicsSceneMouseEvent* event )
{
	QGraphicsScene::mousePressEvent(event);
}

void ERDiagramScene::sceneMouseMove( QGraphicsSceneMouseEvent* event )
{
	QGraphicsScene::mouseMoveEvent(event);
}

void ERDiagramScene::sceneMouseRelease( QGraphicsSceneMouseEvent* event )
{
	QGraphicsScene::mouseReleaseEvent(event);
}

void ERDiagramScene::resetSelectedItem()
{
	for (int i = 0; i < _guiItem.size(); i++)
		_guiItem[i]->setSelected(false);
}

// 輸入是ERModel的ComponentID
void ERDiagramScene::setSelectedItem( QVector<int> pasteComponentIDSet )
{
	int pasteItemID;
	for(int i = 0; i < pasteComponentIDSet.size(); i++)
	{
		pasteItemID = searchItemIDByERModelID(pasteComponentIDSet[i]);
		_guiItem[pasteItemID]->setSelected(true);
	}
}
