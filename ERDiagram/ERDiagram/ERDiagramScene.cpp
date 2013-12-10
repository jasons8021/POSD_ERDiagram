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

// QString���r����ΡAQString.split()�|�^��QStringList�AQStringList���Ϊk�O.at(index)
QVector<QStringList> ERDiagramScene::splitTextData( QString textData )
{
	QVector<QStringList> resultTextData;
	QStringList textDataRowSet = textData.split(SEMICOLON);
	for (int i = 0; i < textDataRowSet.size(); i++)
		resultTextData.push_back(textDataRowSet.at(i).split(COMMA));

	return resultTextData;
}

// �[�JItem��Scene�W
ItemComponent* ERDiagramScene::addNode( QString type, QString text, QPointF point )
{
	ItemComponent* newItem;
	ItemFactory* itemFactory = new ItemFactory();

	newItem = static_cast<ItemComponent*>(itemFactory->creatItem(point.x(), point.y(), type, text));
	delete itemFactory;

	// �NnewItem push�i_guiItem���A�ó]�wGUI�W����ItemID�H��ERModel��ComponentID
	setItemIDandERModelID(newItem);

	// �Nscene�]��Item
	newItem->setScene(this);
	// �NItem�[�Jscene
	this->addItem(newItem);

	_gui->addNodeIntoTable(type,text);

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

	addNode(type, text, QPointF(itemPos.x(), itemPos.y()));
}

// GUI�����s�WItem��ERModel��(���t�e�bScene�W)
void  ERDiagramScene::addNodeFromGUI( QString type, QString text, QPointF point )
{
	// �[�JItem�iERModel
	_gui->addNode(type, text, point);
}

// �s�W�s��
ItemComponent* ERDiagramScene::addConnection( ItemComponent* sourceItem, ItemComponent* destionationItem, QString text )
{
	ItemComponent* newItem;
	ItemFactory* itemFactory = new ItemFactory();

	// �P�_�O�_��Cardinality�A�ó]�w�iconnectionItem��
	bool isSetCardinality = checkSetCardinality(sourceItem->getItemID(), destionationItem->getItemID());

	newItem = static_cast<ItemComponent*>(itemFactory->creatItemConnection(sourceItem, destionationItem, text, isSetCardinality));
	delete itemFactory;

	// �NnewItem push�i_guiItem���A�ó]�w�s��
	setItemIDandERModelID(newItem);

	// �Nscene�]��Item
	newItem->setScene(this);
	// �NItem�[�Jscene
	this->addItem(newItem);

	_gui->addNodeIntoTable("Connector", text);

	return newItem;
}

// �q��J���ɮ׷s�W���Item�����s��
void  ERDiagramScene::addConnectionFromLoadFile( QStringList connecionData )
{
	int sourceItemID = connecionData.at(PARAMETER_SOURCEITEMID).toInt();
	int destionationItemID = connecionData.at(PARAMETER_DESTIONATIONITEMID).toInt();
	QString text =  connecionData.at(PARAMETER_CONNECTIONITEMTEXT);

	// �o�䪺sourceItemID�H��destionationItemID���O�qERModel����o�쪺ComponentID
	addConnection(searchItemByERModelID(sourceItemID), searchItemByERModelID(destionationItemID), text);
}

// �VGUI�o�XConnection���n�D�A��GUI�h�i�DPM
void ERDiagramScene::addConnectionFromGUI( ItemComponent* sourceItem, ItemComponent* destionationItem, QString text )
{
	_gui->addConnection(sourceItem->getERModelID(), destionationItem->getERModelID(), text);
}

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
	// �o�䪺targetNodeID�O������TableView��RowNumber�A�ҥH�OGUI�o�䪺ItemID
	_guiItem[targetNodeID]->changeItemText(editedText);
	update();
}

void ERDiagramScene::changePrimaryKey( int targetNodeID, bool isPrimaryKey )
{
	// �o�䪺targetNodeID�O������TableView��RowNumber�A�ҥH�OGUI�o�䪺ItemID
	static_cast<ItemAttribute*>(_guiItem[targetNodeID])->setPrimaryKey(isPrimaryKey);
	update();
}

bool ERDiagramScene::checkSetCardinality( int sourceNodeID, int destinationNodeID )
{
	return _gui->checkSetCardinality(sourceNodeID, destinationNodeID);
}

// ����observer���q���A�[�J�s��Item�i�JScene��(�e�X��)
void ERDiagramScene::updateAddNewItem( QString type, QString text, QPointF point )
{
	addNode(type, text, point);
}

// ����observer���q���A�[�JConnection��Scene��(�e�X��)
void ERDiagramScene::updateConnection( int sourceItemID, int destionationItemID, QString text )
{
	// �o�䦬�쪺�OERModel����o�쪺ComponentID
	addConnection(searchItemByERModelID(sourceItemID), searchItemByERModelID(destionationItemID), text);
}

void ERDiagramScene::changeDeleteActionEnable()
{
	_gui->changeDeleteActionEnable(true);
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

// baseLineID�O���R���Frow�X���Ʀr�A�j�󦹼Ʀr��ItemID���n��1
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

	// ��M�Q�I����Item
	for (int i = 0; i < _guiItem.size(); i++)
	{
		if (_guiItem[i]->isSelected())
		{
			// �o�䪺deleteItemID�O��GUI Item���A�ҥH�ϥ�ItemID
			result = i;
			break;
		}
	}
	return result;
}

void ERDiagramScene::deleteItem( int deleteItemID )
{
	// �R���e���W��Item
	this->removeItem(_guiItem[deleteItemID]);
	// �R��TableView��Row
	_gui->deleteTableRow(deleteItemID);
	// ���]ItemID
	resetItemID(deleteItemID);
	// �R��_guiItem
	for(int i = 0; i < _guiItem.size(); i++)
	{
		if (deleteItemID == _guiItem[i]->getItemID())
			_guiItem.erase(_guiItem.begin()+i);
	}
}

void ERDiagramScene::deleteSelectedItem()
{
	// ��deleteItemID��ItemID
	int deleteItemID = searchItemIsSelected();

	deleteItem(deleteItemID);

}