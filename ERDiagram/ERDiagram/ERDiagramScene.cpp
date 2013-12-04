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

	for ( int i = 0; i < resultPrimaryKey.size(); i++)
		static_cast<ItemAttribute*>(_guiItem[resultPrimaryKey.at(i).toInt()])->setPrimaryKey(true);

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

// �s�WItem
ItemComponent* ERDiagramScene::addNode( QPointF point, QString type, QString text )
{
	ItemComponent* newItem;
	ItemFactory* itemFactory = new ItemFactory();

	newItem = static_cast<ItemComponent*>(itemFactory->creatItem(point.x(), point.y(), type, text));
	delete itemFactory;

	// �NnewItem push�i_guiItem���A�ó]�w�s��
	setItemID(newItem);

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

	addNode(QPointF(itemPos.x(), itemPos.y()), type, text);
}

// �qGUI�����s�WItem
void  ERDiagramScene::addNodeFromGUI( QPointF point, QString type, QString text )
{
	// �[�JItem�iERModel
	_gui->addNode(type, text);

	// �[�JItem��GUI�W
	addNode(point, type, text);

}

// �s�W�s��
ItemComponent* ERDiagramScene::addConnection( ItemComponent* sourceItem, ItemComponent* destionationItem, QString text )
{
	ItemComponent* newItem;
	ItemFactory* itemFactory = new ItemFactory();

	newItem = static_cast<ItemComponent*>(itemFactory->creatItemConnection(sourceItem, destionationItem, text));
	delete itemFactory;

	// �NnewItem push�i_guiItem���A�ó]�w�s��
	setItemID(newItem);

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
	int sourceItemId = connecionData.at(PARAMETER_SOURCEITEMID).toInt();
	int destionationItemId = connecionData.at(PARAMETER_DESTIONATIONITEMID).toInt();
	QString text =  connecionData.at(PARAMETER_CONNECTIONITEMTEXT);

	addConnection(_guiItem[sourceItemId], _guiItem[destionationItemId], text);
}

// �qGUI�W�s�W���Item���s��
void ERDiagramScene::addConnectionFromGUI( ItemComponent* sourceItem, ItemComponent* destionationItem, QString text )
{
	// �[�Jconnection�i�JERModel�A���\�b�e�s�u
	if (_gui->addConnection(findItemId(sourceItem), findItemId(destionationItem), text))
		addConnection(sourceItem, destionationItem, text);
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

int ERDiagramScene::findItemId( ItemComponent* targetItem )
{
	for (int i = 0; i < _guiItem.size(); i++)
	{
		if (_guiItem[i] == targetItem)
			return i;
	}
	return PARAMETER_NOTFINDID;
}

void ERDiagramScene::setItemID( ItemComponent* newComponent )
{
	_guiItem.push_back(newComponent);
	newComponent->setItemID(_guiItem.size() - 1);
}

QVector<ItemComponent*> ERDiagramScene::getGUIItem()
{
	return _guiItem;
}

void ERDiagramScene::changeItemText( int textChangedItemID, QString editedText )
{
	_guiItem[textChangedItemID]->changeItemText(editedText);
	update();
}