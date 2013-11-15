#include "ERDiagramScene.h"

ERDiagramScene::ERDiagramScene(QObject* parent) : QGraphicsScene(parent)
{
	_itemAttributePos = QPointF(0,50);
	_itemEntityPos = QPointF(350,50);
	_itemRelationshipPos = QPointF(700,50);
}

ERDiagramScene::~ERDiagramScene()
{
}

// 根據loadFile加入全部的Item
void ERDiagramScene::addAllItem( QVector<QString> inputFileText )
{
	int connectionCounter = 0;
	QVector<QStringList> resultComponentData = splitTextData(inputFileText[PARAMETER_COMPONENTDATA]);
	QVector<QStringList> resultConnectionData = splitTextData(inputFileText[PARAMETER_CONNECTIONDATA]);
	QStringList resultPrimaryKey = inputFileText[PARAMETER_PRIMARYKEYDATA].split(COMMA);

	for(int i = 0; i < resultComponentData.size(); i++)
	{
		if (resultComponentData[i].at(PARAMETER_TYPE) != PARAMETER_CONNECTOR)
			_guiItem.push_back(addComponent(resultComponentData[i]));
		else
		{
			_guiItem.push_back(addConnection(resultConnectionData[connectionCounter]));
			connectionCounter++;
		}
	}

	for ( int i = 0; i < resultPrimaryKey.size(); i++)
		static_cast<ItemAttribute*>(_guiItem[i])->setPrimaryKey(true);

	for (int i = 0; i < _guiItem.size(); i++)
		this->addItem(_guiItem[i]);
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

// 新增Item
ItemComponent* ERDiagramScene::addComponent( QStringList componentData )
{
	QPointF itemPos;
	ItemComponent* newItem;
	ItemFactory* itemFactory = new ItemFactory();
	QString type =  componentData.at(PARAMETER_TYPE);
	QString text =  componentData.at(PARAMETER_TEXT);

	itemPos = getPlaceItemPosition(componentData[PARAMETER_TYPE]);
	newItem = static_cast<ItemComponent*>(itemFactory->creatItem(itemPos.x(), itemPos.y(), type, text));

	delete itemFactory;

	return newItem;
}

// 兩個Item間的連結
ItemComponent* ERDiagramScene::addConnection( QStringList connecionData )
{
	ItemComponent* newItem;
	ItemFactory* itemFactory = new ItemFactory();

	int sourceItemId = connecionData.at(PARAMETER_SOURCEITEMID).toInt();
	int destionationItemId = connecionData.at(PARAMETER_DESTIONATIONITEMID).toInt();
	QString text =  connecionData.at(PARAMETER_CONNECTIONITEMTEXT);

	newItem = static_cast<ItemComponent*>(itemFactory->creatItemConnection(_guiItem[sourceItemId], _guiItem[destionationItemId], text));

	delete itemFactory;

	return newItem;
}

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
