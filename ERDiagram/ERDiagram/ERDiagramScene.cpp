#include "ERDiagramScene.h"

ERDiagramScene::ERDiagramScene(QObject* parent) : QGraphicsScene(parent)
{
	_previewItem = new ItemComponent();
	_itemAttributePos = QPointF(0,50);
	_itemEntityPos = QPointF(350,50);
	_itemRelationshipPos = QPointF(700,50);
	_state = new PointerState(this);
	_gui = static_cast<GUI*>(parent);
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

// QString的字串切割，QString.split()會回傳QStringList，QStringList的用法是.at(index)
QVector<QStringList> ERDiagramScene::splitTextData( QString textData )
{
	QVector<QStringList> resultTextData;
	QStringList textDataRowSet = textData.split(SEMICOLON);
	for (int i = 0; i < textDataRowSet.size(); i++)
		resultTextData.push_back(textDataRowSet.at(i).split(COMMA));

	return resultTextData;
}

// 加入Item到Scene上
ItemComponent* ERDiagramScene::addNode( QString type, QString text, QPointF point )
{
	ItemComponent* newItem;
	ItemFactory* itemFactory = new ItemFactory();

	newItem = static_cast<ItemComponent*>(itemFactory->creatItem(point.x(), point.y(), type, text));
	delete itemFactory;

	// 將newItem push進_guiItem中，並設定GUI上面的ItemID以及ERModel的ComponentID
	setItemIDandERModelID(newItem);

	// 將scene設給Item
	newItem->setScene(this);
	// 將Item加入scene
	this->addItem(newItem);

	_gui->addNodeIntoTable(type,text);

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

	addNode(type, text, QPointF(itemPos.x(), itemPos.y()));
}

// GUI介面新增Item到ERModel中(不含畫在Scene上)
void  ERDiagramScene::addNodeFromGUI( QString type, QString text, QPointF point )
{
	// 加入Item進ERModel
	_gui->addNode(type, text, point);
}

// 新增連結
ItemComponent* ERDiagramScene::addConnection( ItemComponent* sourceItem, ItemComponent* destionationItem, QString text )
{
	ItemComponent* newItem;
	ItemFactory* itemFactory = new ItemFactory();

	// 判斷是否為Cardinality，並設定進connectionItem中
	bool isSetCardinality = checkSetCardinality(sourceItem->getItemID(), destionationItem->getItemID());

	newItem = static_cast<ItemComponent*>(itemFactory->creatItemConnection(sourceItem, destionationItem, text, isSetCardinality));
	delete itemFactory;

	// 將newItem push進_guiItem中，並設定編號
	setItemIDandERModelID(newItem);

	// 將scene設給Item
	newItem->setScene(this);
	// 將Item加入scene
	this->addItem(newItem);

	_gui->addNodeIntoTable("Connector", text);

	return newItem;
}

// 從輸入的檔案新增兩個Item間的連結
void  ERDiagramScene::addConnectionFromLoadFile( QStringList connecionData )
{
	int sourceItemID = connecionData.at(PARAMETER_SOURCEITEMID).toInt();
	int destionationItemID = connecionData.at(PARAMETER_DESTIONATIONITEMID).toInt();
	QString text =  connecionData.at(PARAMETER_CONNECTIONITEMTEXT);

	// 這邊的sourceItemID以及destionationItemID都是從ERModel那邊得到的ComponentID
	addConnection(searchItemByERModelID(sourceItemID), searchItemByERModelID(destionationItemID), text);
}

// 向GUI發出Connection的要求，使GUI去告訴PM
void ERDiagramScene::addConnectionFromGUI( ItemComponent* sourceItem, ItemComponent* destionationItem, QString text )
{
	_gui->addConnection(sourceItem->getERModelID(), destionationItem->getERModelID(), text);
}

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

void ERDiagramScene::changeState( int stateMode )
{
	delete _state;
	_currentMode = stateMode;
	switch(stateMode)
	{
	case PointerMode:
		_previewItem = new ItemComponent();
		_state = new PointerState(this);
		break;
	case ConnectionMode:
		_previewItem = new ItemConnection();
		_state = new ConnectionState(this, _previewItem);
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
		_previewItem = new ItemComponent();
		_state = new SetPrimaryKeyState(_gui, this);
		break;
	}
}

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

void ERDiagramScene::changToPointerMode()
{
	_gui->changeToPointerMode();
}

void ERDiagramScene::setItemIDandERModelID( ItemComponent* newItem )
{
	_guiItem.push_back(newItem);
	newItem->setItemID(_guiItem.size() - 1);
	newItem->setERModelID(_gui->getERModelComponentID() - 1);
}

QVector<ItemComponent*> ERDiagramScene::getGUIItem()
{
	return _guiItem;
}

void ERDiagramScene::changeItemText( int targetNodeID, QString editedText )
{
	// 這邊的targetNodeID是對應到TableView的RowNumber，所以是GUI這邊的ItemID
	_guiItem[targetNodeID]->changeItemText(editedText);
	update();
}

void ERDiagramScene::changePrimaryKey( int targetNodeID, bool isPrimaryKey )
{
	// 這邊的targetNodeID是對應到TableView的RowNumber，所以是GUI這邊的ItemID
	static_cast<ItemAttribute*>(_guiItem[targetNodeID])->setPrimaryKey(isPrimaryKey);
	update();
}

bool ERDiagramScene::checkSetCardinality( int sourceNodeID, int destinationNodeID )
{
	return _gui->checkSetCardinality(sourceNodeID, destinationNodeID);
}

// 收到observer的通知，加入新的Item進入Scene中(畫出來)
void ERDiagramScene::updateAddNewItem( QString type, QString text, QPointF point )
{
	addNode(type, text, point);
}

// 收到observer的通知，加入Connection到Scene中(畫出來)
void ERDiagramScene::updateConnection( int sourceItemID, int destionationItemID, QString text )
{
	// 這邊收到的是ERModel那邊得到的ComponentID
	addConnection(searchItemByERModelID(sourceItemID), searchItemByERModelID(destionationItemID), text);
}

void ERDiagramScene::changeDeleteActionEnable()
{
	_gui->changeDeleteActionEnable(true);
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

// baseLineID是指刪除了row幾的數字，大於此數字的ItemID都要減1
void ERDiagramScene::resetItemID( int baseLineID )
{
	for(int i = 0; i < _guiItem.size(); i++)
	{
		if (_guiItem[i]->getItemID() > baseLineID)
			_guiItem[i]->setItemID(_guiItem[i]->getItemID() - 1);
	}
}

int ERDiagramScene::searchItemIsSelected()
{
	int result;

	result = PARAMETER_NOTFINDID;

	// 找尋被點中的Item
	for (int i = 0; i < _guiItem.size(); i++)
	{
		if (_guiItem[i]->isSelected())
		{
			// 這邊的deleteItemID是砍GUI Item的，所以使用ItemID
			result = i;
			break;
		}
	}
	return result;
}

void ERDiagramScene::deleteItem( int deleteItemID )
{
	// 刪除畫面上的Item
	this->removeItem(_guiItem[deleteItemID]);
	// 刪除TableView的Row
	_gui->deleteTableRow(deleteItemID);
	// 重設ItemID
	resetItemID(deleteItemID);
	// 刪除_guiItem
	for(int i = 0; i < _guiItem.size(); i++)
	{
		if (deleteItemID == _guiItem[i]->getItemID())
			_guiItem.erase(_guiItem.begin()+i);
	}
}

void ERDiagramScene::deleteSelectedItem()
{
	// 此deleteItemID為ItemID
	int deleteItemID = searchItemIsSelected();

	deleteItem(deleteItemID);

}