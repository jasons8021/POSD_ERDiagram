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

// �ھ�loadFile�N������Item�[�i�e����
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

	// resultPrimaryKey�O��ERModel�ǹL�Ӫ��A�ҥH�n�t�X��erModelID
	for ( int i = 0; i < resultPrimaryKey.size(); i++)
		static_cast<ItemAttribute*>(searchItemByERModelID(resultPrimaryKey.at(i).toInt()))->setPrimaryKey(true);

}

// �[�JItem��Scene�W
ItemComponent* ERDiagramScene::addNode( int erModelID, QString type, QString text, QPointF point )
{
	ItemComponent* newItem;
	ItemFactory* itemFactory = new ItemFactory();

	newItem = itemFactory->creatItem(point.x(), point.y(), type, text);
	delete itemFactory;

	// �NnewItem push�i_guiItem���A�ó]�wGUI�W����ItemID�H��ERModel��ComponentID
	setItemIDandERModelID(newItem, erModelID);

	// �Nscene�]��Item
	newItem->setScene(this);
	// �NItem�[�Jscene
	this->addItem(newItem);

	_gui->addNodeIntoTable(newItem->getItemID(), type, text);

	return newItem;
}

// �q��J���ɮ׷s�WItem
void ERDiagramScene::addNodeFromLoadFile( QStringList componentData )
{ 
	QPointF itemPos;
	QString type =  componentData.at(PARAMETER_TYPE);
	QString text =  componentData.at(PARAMETER_TEXT);

	// �쥻��erd�ɨS����m��T�A�]���H�����]��m�i�h
	itemPos = getPlaceItemPosition(componentData[PARAMETER_TYPE]);

	_gui->addNode(type, text, QPointF(itemPos.x(), itemPos.y()));
	update();
	_gui->update();
}

// �s�W�s��
ItemComponent* ERDiagramScene::addConnection( int erModelID, ItemComponent* sourceItem, ItemComponent* destionationItem, QString text )
{
	ItemComponent* newItem;
	ItemFactory* itemFactory = new ItemFactory();

	// �P�_�O�_��Cardinality�A�ó]�w�iconnectionItem��
	bool isSetCardinality = checkSetCardinality(sourceItem->getERModelID(), destionationItem->getERModelID());

	newItem = itemFactory->creatItemConnection(sourceItem, destionationItem, text, isSetCardinality);
	
	delete itemFactory;

	// �NnewItem push�i_guiItem���A�ó]�w�s��
	setItemIDandERModelID(newItem, erModelID);

	// �Nscene�]��Item
	newItem->setScene(this);
	// �NItem�[�Jscene
	this->addItem(newItem);

	_gui->addNodeIntoTable(newItem->getItemID(), "Connector", text);

	return newItem;
}

// �q��J���ɮ׷s�W���Item�����s��
void  ERDiagramScene::addConnectionFromLoadFile( QStringList connecionData )
{
	int sourceItemID = connecionData.at(PARAMETER_SOURCEITEMID).toInt();
	int destionationItemID = connecionData.at(PARAMETER_DESTIONATIONITEMID).toInt();
	QString text =  connecionData.at(PARAMETER_CONNECTIONITEMTEXT);

	_gui->addConnection(sourceItemID, destionationItemID, text);
}

void ERDiagramScene::deleteItem( int deleteItemID )
{
	// �R���e���W��Item
	this->removeItem(_guiItem[deleteItemID]);
	// �R��TableView��Row
	_gui->deleteTableRow(deleteItemID);
	// �R��_guiItem
	_guiItem.remove(deleteItemID);
}

//////////////////////////////////////////////////////////////////////////
//						���m������Function							//
//////////////////////////////////////////////////////////////////////////

// ��loadFile�i�Ӫ���Ƴ]�w��m
void ERDiagramScene::updatePlaceItemPosition( QString type )
{
	// �ھ�type�A����Y�b�y�Ц�m
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

	// �]�wItem����m
	if (type == PARAMETER_ENTITY)
		itemPos = _itemEntityPos;
	else if (type == PARAMETER_ATTRIBUTE)
		itemPos = _itemAttributePos;
	else if (type == PARAMETER_RELATIONSHIP)
		itemPos = _itemRelationshipPos;

	// ��s��m��T
	updatePlaceItemPosition(type);

	return itemPos;
}

// ��sItem���ʫ᪺��m
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
//					��statePattern������Function						//
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
//							��update�t�C��Function						//
//////////////////////////////////////////////////////////////////////////

// ����observer���q���A�[�J�s��Item�i�JScene��(�e�X��)
void ERDiagramScene::updateAddNewItem( int erModelID, QString type, QString text, QPointF point )
{
	addNode(erModelID, type, text, point);
}

// ����observer���q���A�[�JConnection��Scene��(�e�X��)
void ERDiagramScene::updateConnection( int erModelID, int sourceItemID, int destionationItemID, QString text )
{
	// �o�䦬�쪺�OERModel����o�쪺ComponentID
	addConnection(erModelID, searchItemByERModelID(sourceItemID), searchItemByERModelID(destionationItemID), text);
}

// ��ERModel�Ƕi�Ӫ��A�ҥH�OComponentID Set
void ERDiagramScene::updateDeleteItem( QString deleteComponentIDSet )
{
	QStringList deleteIDSet = deleteComponentIDSet.split(COMMA);
	for(int i = 0; i < deleteIDSet.size(); i++)
	{
		// �ѩ�Ƕi�Ӫ��ȬOComponentID�A�ҥH�n�R���n�নItemID�Ӭ�
		int deleteItemID = searchItemByERModelID(deleteIDSet.at(i).toInt())->getItemID();
		deleteItem(deleteItemID);
		resetItemID();
	}
}

//////////////////////////////////////////////////////////////////////////
//								��GUI���q��Function						//
//////////////////////////////////////////////////////////////////////////

// GUI�����s�WItem��ERModel��(���t�e�bScene�W)
void  ERDiagramScene::addNodeFromGUI( QString type, QString text, QPointF point )
{
	// �[�JItem�iERModel
	_gui->addNode(type, text, point);
}

// �VGUI�o�XConnection���n�D�A��GUI�h�i�DPM
void ERDiagramScene::addConnectionFromGUI( ItemComponent* sourceItem, ItemComponent* destionationItem, QString text )
{
	_gui->addConnection(sourceItem->getERModelID(), destionationItem->getERModelID(), text);
}

void ERDiagramScene::changToPointerMode()
{
	_gui->changeToPointerMode();
}

// �o�䪺targetNodeID�O��Model�ǹL�Ӫ��A�ҥHtargetNodeID�OERModelID
void ERDiagramScene::changeItemText( int targetNodeID, QString editedText )
{
	searchItemByERModelID(targetNodeID)->changeItemText(editedText);
	update();
}

void ERDiagramScene::changePrimaryKey( int targetNodeID, bool isPrimaryKey )
{
	// �o�䪺targetNodeID�O������TableView��RowNumber�A�ҥH�OGUI�o�䪺ItemID
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
//							Scene�n�Ϊ�Tool Function					//
//////////////////////////////////////////////////////////////////////////

// QString���r����ΡAQString.split()�|�^��QStringList�AQStringList���Ϊk�O.at(index)
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

// ���s�վ�ItemID�A�æ^�Ǵ��J�i�h��ItemID
int ERDiagramScene::adjustItemID( int erModelID )
{
	// ���p_guiItem�̭��O�Ū�
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

// isAdd��True�ɡAreset��baseLineItemID�j��ItemID+1 ; false�h�O-1
void ERDiagramScene::resetItemID()
{
	for(int i = 0; i < _guiItem.size(); i++)
		_guiItem[i]->setItemID(i);
}

// ��ERModel���䪺ComponentID�ӷj�MItem
ItemComponent* ERDiagramScene::searchItemByERModelID( int targetERModelID )
{
	for (int i = 0; i < _guiItem.size(); i++)
	{
		if (targetERModelID == _guiItem[i]->getERModelID())
			return _guiItem[i];
	}
	return NULL;
}

// �^�ǳQ�襤Item��ERModelID
QVector<int> ERDiagramScene::searchItemIsSelected()
{
	QVector<int> resultSet;

	// ��M�Q�I����Item
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

	// �̫�@�լO�ťաA�ҥH���Υ[
	for(int i = 0; i < ReBuildConnectionSet.size()-1; i++)
	{
		// ReBuildConnectionContent�榡��ReBuildConnectionContent[0]�OConnectorID�BReBuildConnectionContent[1]�OsourceNodeID(erModelID)�B
		// ReBuildConnectionContent[2]�OdestinationNodeID(erModelID)�BReBuildConnectionContent[3]�OConnectorText
		QStringList ReBuildConnectionContent = ReBuildConnectionSet.at(i).split(COMMA);

		addConnection(ReBuildConnectionContent.at(0).toInt(), searchItemByERModelID(ReBuildConnectionContent.at(1).toInt()), 
					  searchItemByERModelID(ReBuildConnectionContent.at(2).toInt()), ReBuildConnectionContent.at(3));
	}
}

int ERDiagramScene::searchERModelIDByItemID( int itemID )
{
	return _guiItem[itemID]->getERModelID();
}

// ��ERModel���䪺ComponentID�ӧ�ItemID
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

// ��J�OERModel��ComponentID
void ERDiagramScene::setSelectedItem( QVector<int> pasteComponentIDSet )
{
	int pasteItemID;
	for(int i = 0; i < pasteComponentIDSet.size(); i++)
	{
		pasteItemID = searchItemIDByERModelID(pasteComponentIDSet[i]);
		_guiItem[pasteItemID]->setSelected(true);
	}
}
