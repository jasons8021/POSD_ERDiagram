#include "ERModel.h"
#include <algorithm>
#include "SaveComponentVisitor.h"
#include "SaveXmlComponentVisitor.h"

ERModel::ERModel()
{
	_componentID = 0;
	_isModify = false;
	_fileName = TEXT_NULL;
}

ERModel::~ERModel()
{
	resetERModel();
	resetClipboard();
}

//////////////////////////////////////////////////////////////////////////
//							AddNode										//
//////////////////////////////////////////////////////////////////////////

//	componentID遞增的情況下使用
int ERModel::addNode( string type, string text, int sx, int sy )
{
	return addNode(_componentID++, type, text, sx, sy);
}

//	指定componentID的情況使用，eg:undo、redo，並回傳componentID
int ERModel::addNode( int componentID, string type, string text, int sx, int sy )
{
	ComponentFactory* componentFactory = new ComponentFactory();
	
	Component* newComponent;
	newComponent = static_cast<Component*>(componentFactory->creatComponent(componentID, type, text, sx, sy));
	if (newComponent == nullptr)
		return PARAMETER_ISERROR;
	_isModify = true;

	_components.push_back(newComponent);
	delete componentFactory;

	return newComponent->getID();
}

//////////////////////////////////////////////////////////////////////////
//							AddConnection								//
//////////////////////////////////////////////////////////////////////////

int ERModel::addConnection( int sourceNodeID, int destinationNodeID, string text )
{
	return addConnection(_componentID++, sourceNodeID, destinationNodeID, text);
}

int ERModel::addConnection( int componentID, int sourceNodeID, int destinationNodeID, string text )
{
	Component* sourceNode = searchComponent(sourceNodeID);
	Component* destinationNode = searchComponent(destinationNodeID);
	Component* newConnection;
	ComponentFactory* componentFactory = new ComponentFactory();

	if (sourceNode == NULL || destinationNode == NULL )
		return PARAMETER_ISERROR;

	_isModify = true;

	if (checkConnectionState(sourceNode, destinationNode) == TEXT_CONNECTION_CANCONNECT)
	{
		newConnection = static_cast<Component*>(componentFactory->creatComponent(componentID, PARAMETER_CONNECTOR, text, PARAMETER_TEXTUI_COORDINATES, PARAMETER_TEXTUI_COORDINATES));
		_components.push_back(newConnection);

		static_cast<Connector*>(newConnection)->setConnectedNode(sourceNode, destinationNode);
		_connections.push_back(newConnection);

		connectionSetting(sourceNode, destinationNode, text);
	}
	else
		return PARAMETER_ISERROR;
	delete componentFactory;

	return newConnection->getID();
}

// 兩個Component的Connection的設定
void ERModel::connectionSetting( Component* sourceNode, Component* destinationNode, string text )
{
	sourceNode->connectTo(destinationNode);

	if (checkSetCardinality(sourceNode->getID(), destinationNode->getID()))
		setCardinality(sourceNode, destinationNode, text);
}

//	Check Connection hasn't any error. If it has, return error message.
string ERModel::checkConnectionState( Component* sourceNode, Component* destinationNode )
{
	return sourceNode->canConnectTo(destinationNode);
}

//	The method provides to textUI to get connection error message.
string ERModel::getCheckConnectionStateMessage( int sourceNodeID, int destinationNodeID )
{
	Component* sourceNode = searchComponent(sourceNodeID);
	Component* desinationNode = searchComponent(destinationNodeID);
	return checkConnectionState(sourceNode, desinationNode);
}

//	The method provide to textUI to check whether set cardinality or not.
bool ERModel::checkSetCardinality( int sourceNodeID, int destinationNodeID )
{
	Component* sourceNode = searchComponent(sourceNodeID);
	Component* desinationNode = searchComponent(destinationNodeID);

	if((sourceNode->getType() == PARAMETER_ENTITY && desinationNode->getType() == PARAMETER_RELATIONSHIP) || 
		(sourceNode->getType() == PARAMETER_RELATIONSHIP && desinationNode->getType() == PARAMETER_ENTITY))
		return true;
	else
		return false;
}

void ERModel::setCardinality( Component* sourceNode, Component* destinationNode, string cardinality )
{
	int entityID;

	if (sourceNode->getType() == PARAMETER_ENTITY)			// sourceNode is entity, and destinationNode is relationship
	{
		entityID = sourceNode->getID();
		static_cast<NodeRelationship*>(destinationNode)->setEntityCardinality(make_pair(entityID,cardinality));
	}
	else													// destinationNode is entity, and sourceNode is relationship
	{
		entityID = destinationNode->getID();
		static_cast<NodeRelationship*>(sourceNode)->setEntityCardinality(make_pair(entityID,cardinality));
	}
}


//////////////////////////////////////////////////////////////////////////
//							GetTable									//
//////////////////////////////////////////////////////////////////////////

//	搜尋特定的type(或all type)，並回傳搜尋結果的dataList
string ERModel::getComponentsTable(string type)
{
	string componentTableString;

	if (_components.size() != PARAMETER_ZEROCOMPONENT)
	{
		componentTableString = getComponentDataList(type, _components);

		return componentTableString;
	}
	return PARAMETER_NULL;
}

string ERModel::getConnectionTable()
{
	string connectionsTableString;

	if (_connections.size() != 0)
	{
		for (int i = 0; i < _connections.size(); i++) {
			connectionsTableString += TEXT_FIVESPACE + Toolkit::integerToString(_connections[i]->getID()) +  TEXT_TWOSPACE + TEXT_SPACELINE
				+ TEXT_TWOSPACE + Toolkit::integerToString(static_cast<Connector*>(_connections[i])->getSourceNodeID()) + TEXT_SPACELINE
				+ TEXT_TWOSPACE + Toolkit::integerToString(static_cast<Connector*>(_connections[i])->getDestinationNodeID()) + TEXT_SPACELINE + TEXT_ENDLINE;
		}
		return connectionsTableString;
	}

	return PARAMETER_NULL;
}

//////////////////////////////////////////////////////////////////////////
//							SetPrimaryKey								//
//////////////////////////////////////////////////////////////////////////

void ERModel::setPrimaryKey( int entityID, vector<int> primaryKeys )
{
	NodeEntity* entityNode = static_cast<NodeEntity*>(searchComponent(entityID));
	Component* attributeNode;

	_isModify = true;

	for( int i = 0; i < primaryKeys.size(); i++)
	{
		attributeNode = searchComponent(primaryKeys[i]);
		static_cast<NodeAttribute*>(attributeNode)->setIsPrimaryKey(true);
		entityNode->setPrimaryKey(attributeNode->getID());
	}
}

void ERModel::reBuildPrimaryKeyFromAttribute( int attributeID, int entityID )
{
	NodeAttribute* attributeNode = static_cast<NodeAttribute*>(searchComponent(attributeID));
	NodeEntity* entityNode = static_cast<NodeEntity*>(searchComponent(entityID));

	attributeNode->setIsPrimaryKey(true);
	entityNode->setPrimaryKey(attributeID);
}

string ERModel::searchAttributeOfEntity( int entityID )
{
	string attributeOfEntityDataList;
	Component* specificEntityNode = searchComponent(entityID);

	return getComponentDataList(PARAMETER_ATTRIBUTE, specificEntityNode->getConnections());
}

bool ERModel::searchEntityConnection( int entityID, int targetNodeID, string targetType)
{
	Component* entityNode = searchComponent(entityID);
	
	return searchComponentConnection(targetNodeID, targetType, entityNode->getConnections());
}

//////////////////////////////////////////////////////////////////////////
//							Show ERDiagram								//
//////////////////////////////////////////////////////////////////////////

string ERModel::getERDiagramTable()
{
	string erDiagramRowString;
	string erDiagramTableString;
	string primaryKeyString;
	string notPrimaryKeyString;
	vector<Component*> entitySet = searchSpecificTypeComponentSet(PARAMETER_ENTITY, _components);
	vector<Component*> attributeInEntitySet;

	setForeignKey();

	for(int i = 0; i < entitySet.size(); i++)
	{
		erDiagramRowString = TEXT_ONESPACE + entitySet[i]->getText();

		// 用空白填滿
		while (erDiagramRowString.size() < PARAMETER_FILLUPSIZE)
			erDiagramRowString += TEXT_ONESPACE;

		erDiagramRowString += TEXT_LINESPACE;
		attributeInEntitySet = searchSpecificTypeComponentSet(PARAMETER_ATTRIBUTE, entitySet[i]->getConnections());
		erDiagramRowString += getAttributeContents(attributeInEntitySet);
		
		for(int j = 0; j < static_cast<NodeEntity*>(entitySet[i])->getForeignKey().size(); j++)
		{
			if (static_cast<NodeEntity*>(entitySet[i])->getIsShowForeignKeyinERTable()[j] == PARAMETER_TRUE)
				erDiagramRowString += searchForeignKey(static_cast<NodeEntity*>(entitySet[i])->getForeignKey()[j]);
		}
		erDiagramRowString += TEXT_ENDLINE;
		erDiagramTableString += erDiagramRowString;
	}
	return erDiagramTableString;
}

string ERModel::getAttributeContents( vector<Component*> attributeSet)
{
	string isPKString;
	string isNotPKString;

	for(int i = 0; i < attributeSet.size(); i++)
	{
		if (static_cast<NodeAttribute*>(attributeSet[i])->getIsPrimaryKey())
			isPKString += attributeSet[i]->getText() + TEXT_COMMASPACE;
		else
			isNotPKString += attributeSet[i]->getText() + TEXT_COMMASPACE;
	}

	// 刪除最後一個"," 並將格式放入string中
	if (isPKString != PARAMETER_NULL)
	{
		isPKString = isPKString.substr(PARAMETER_STRINGBEGIN, isPKString.size() - PARAMETER_ADJUSTSTRINGSIZE);
		isPKString = TEXT_GETERDIAGRAM_PK + isPKString + TEXT_GETERDIAGRAM_ENDKEY;			
	}
	if (isNotPKString != PARAMETER_NULL)
	{
		isNotPKString = isNotPKString.substr(PARAMETER_STRINGBEGIN, isNotPKString.size() - PARAMETER_ADJUSTSTRINGSIZE);
		if (isPKString != PARAMETER_NULL)
			isNotPKString = TEXT_COMMASPACE + isNotPKString;
		else
			isNotPKString = isNotPKString;
	}

	return isPKString + isNotPKString;
}

void ERModel::setForeignKey()
{
	vector<Component*> relationshipSet = searchSpecificTypeComponentSet(PARAMETER_RELATIONSHIP, _components);
	vector<int> oneToOneEntityID;	// It store two entity ID which two entity is one to one relationship.
	NodeEntity* setFKEntityNodeFirst;
	NodeEntity* setFKEntityNodeSecond;

	for(int i = 0; i < relationshipSet.size(); i++)
	{
		oneToOneEntityID = oneToOne(static_cast<NodeRelationship*>(relationshipSet[i]));
		// Two entity set the other one as Foreign Key.
		if (!oneToOneEntityID.empty())
		{
			setFKEntityNodeFirst = static_cast<NodeEntity*>(searchComponent(oneToOneEntityID[0]));
 			setFKEntityNodeSecond = static_cast<NodeEntity*>(searchComponent(oneToOneEntityID[1]));

			setFKEntityNodeFirst->setForeignKey(oneToOneEntityID[1]);
 			setFKEntityNodeSecond->setForeignKey(oneToOneEntityID[0]);
			
			setFKEntityNodeFirst->setIsShowForeignKeyinERTable(true);
			setFKEntityNodeSecond->setIsShowForeignKeyinERTable(false);
		}
	}
}

// Return a vector that storing two entityID which is existing one to one relationship.
vector<int> ERModel::oneToOne( NodeRelationship* relationshipNode )
{
	// entityCardinalitySet是透過relationship相連的兩個entity的集合，格式為(entity, cardinality).
	vector<pair<int,string>> entityCardinalitySet = relationshipNode->getEntityCardinality();
	vector<int> oneToOneEntities;

	if (entityCardinalitySet.size() < PARAMETER_RELATIONSHIPLOWERBOUND)									// Relationship只有connect一個entity.
		return oneToOneEntities;
	else
	{
		if (entityCardinalitySet[PARAMETER_FIRSTENTITY].second == "1" && entityCardinalitySet[PARAMETER_SECONDENTITY].second == "1")				// One to one
		{
			oneToOneEntities.push_back(entityCardinalitySet[PARAMETER_FIRSTENTITY].first);
			oneToOneEntities.push_back(entityCardinalitySet[PARAMETER_SECONDENTITY].first);
			return oneToOneEntities;
		}
		else																							// One to N 或 N to one 或 N to N
			return oneToOneEntities;
	}
}

bool ERModel::checkOneToOne()
{
	vector<Component*> relationshipNode = searchSpecificTypeComponentSet(PARAMETER_RELATIONSHIP, _components);

	// entityCardinalitySet是透過relationship相連的兩個entity的集合，格式為(entity, cardinality).
	vector<pair<int,string>> entityCardinalitySet;

	for(int i = 0; i < relationshipNode.size(); i++){
		entityCardinalitySet = static_cast<NodeRelationship*>(relationshipNode[i])->getEntityCardinality();
		if (entityCardinalitySet.size() >= PARAMETER_RELATIONSHIPLOWERBOUND)								// 檢查relationship至少連到兩個entity.
			if (entityCardinalitySet[PARAMETER_FIRSTENTITY].second == "1" && entityCardinalitySet[PARAMETER_SECONDENTITY].second == "1")				// One to one
				return true;
	}
	return false;
}

string ERModel::searchForeignKey( int foreignKeyEntityID )
{
	Component* foreignKeyEntity = searchComponent(foreignKeyEntityID);
	vector<Component*> attributeSet = searchSpecificTypeComponentSet(PARAMETER_ATTRIBUTE, foreignKeyEntity->getConnections());
	string isFKString;

	for(int i = 0; i < attributeSet.size(); i++)
	{
		if (static_cast<NodeAttribute*>(attributeSet[i])->getIsPrimaryKey())
			isFKString += attributeSet[i]->getText() + TEXT_COMMASPACE;
	}

	// 刪除最後一個", "並將格式放入string中
	if (isFKString != PARAMETER_NULL)
	{
		isFKString = isFKString.substr(PARAMETER_STRINGBEGIN, isFKString.size() - PARAMETER_ADJUSTSTRINGSIZE);
		isFKString = TEXT_GETERDIAGRAM_FK + isFKString + TEXT_GETERDIAGRAM_ENDKEY;
	}
	return isFKString;
}

//////////////////////////////////////////////////////////////////////////
//							Load/Save									//
//////////////////////////////////////////////////////////////////////////

string ERModel::loadERDiagram( string fileName )
{
	_fileName= fileName;
	ifstream inputERDiagramFile;
	inputERDiagramFile.open(fileName);

	if (!inputERDiagramFile.is_open())
		return TEXT_LOADSAVE_OPENFAILED;

	resetERModel();

	//	取得輸入檔案的所有字元
	string erDiagramText((istreambuf_iterator<char>(inputERDiagramFile)),istreambuf_iterator<char>());
	inputERDiagramFile.close();

	recoveryFile(classifyInputFile(erDiagramText));
	
	return TEXT_LOADSAVE_OPENSUCCESS;
}


vector<pair<int,int>> ERModel::loadPosition()
{
	vector<pair<int,int>> positionSet;
	vector<string> posFileSplitedSet;
	vector<string> componentPositionSet;

	string posFileName = removeExtension(_fileName) + TEXT_EXTENSION_POS;
	ifstream inputPositionFile;
	inputPositionFile.open(posFileName);

	if (!inputPositionFile.is_open())
		return positionSet;

	//	取得輸入檔案的所有字元
	string posText((istreambuf_iterator<char>(inputPositionFile)),istreambuf_iterator<char>());
	inputPositionFile.close();

	// 將每一筆的Sx,Sy切出來變成vector<string>的一項是110 100
	posFileSplitedSet = Toolkit::splitFunction(posText, TEXT_ENDLINE);

	// 最後一組是空的
	for(int i = 0; i < posFileSplitedSet.size() - 1; i++)
	{
		componentPositionSet = Toolkit::splitFunction(posFileSplitedSet[i], SPACE);
		positionSet.push_back(make_pair(atoi(componentPositionSet[PARAMETER_POSITIONFILEX].c_str()), atoi(componentPositionSet[PARAMETER_POSITIONFILEY].c_str())));
	}

	return positionSet;
}


//	將輸入的.erd檔中的內容分類，分成ComponentsSet、ConnectionsSet、PrimaryKeysSet
vector<vector<string>> ERModel::classifyInputFile( string fileText )
{
	vector<string> splitFileText;
	vector<string> splitComponentsSet, splitConnectionsSet, splitPrimaryKeysSet;
	vector<vector<string>> splitResultSet;

	bool firstSpace = false, secondSpace = false;

	splitFileText = Toolkit::splitFunction(fileText,TEXT_ENDLINE);
	for(int i = 0; splitFileText.size() > i; i++)
	{
		if (splitFileText[i] == PARAMETER_NULL && !firstSpace)
			firstSpace = true;
		else if (splitFileText[i] == PARAMETER_NULL && firstSpace)
			secondSpace = true;

		if (splitFileText[i] != PARAMETER_NULL)
		{
			if (!firstSpace)
				splitComponentsSet.push_back(splitFileText[i]);
			else if(firstSpace && !secondSpace)
				splitConnectionsSet.push_back(splitFileText[i]);
			else if (secondSpace)
				splitPrimaryKeysSet.push_back(splitFileText[i]);
		}
	}
	splitResultSet.push_back(splitComponentsSet);
	splitResultSet.push_back(splitConnectionsSet);
	splitResultSet.push_back(splitPrimaryKeysSet);

	return splitResultSet;
}

//	重建輸入(.erd)的ERDiagram. FileSet's vector[PARAMETER_COMPONENTSET]是components的集合, vector[PARAMETER_CONNECTIONSET]是connections的集合, vector[PARAMETER_PRIMARYKEYSET]是primary keys的集合
void ERModel::recoveryFile( vector<vector<string>> fileSet )
{
 	recoveryAllComponent(fileSet[PARAMETER_COMPONENTSET], fileSet[PARAMETER_CONNECTIONSET]);
 	recoveryPrimaryKey(fileSet[PARAMETER_PRIMARYKEYSET]);
}
//	將分割後的檔案內容，針對Component跟Connection做加入
void ERModel::recoveryAllComponent( vector<string> componentsSet, vector<string> connectionsSet )
{
	vector<string> componentData;
	vector<pair<int, int>> positionSet = loadPosition();

	int connectionSetCount = 0;
	int positionSetCount = 0;
	for(int i = 0; i < componentsSet.size(); i++)
	{
		componentData = Toolkit::splitFunction(componentsSet[i], TEXT_COMMASPACE);

		if ( componentData[PARAMETER_TYPE] == PARAMETER_CONNECTOR && componentData.size() > PARAMETER_HAVETYPEANDTEXT )			//	Connector有Cardinality
			connectionSetCount = recoveryConnection(connectionsSet, connectionSetCount, componentData[1]);
		else if ( componentData[PARAMETER_TYPE] == PARAMETER_CONNECTOR )														//	Connector沒有Cardinality
			connectionSetCount = recoveryConnection(connectionsSet, connectionSetCount, PARAMETER_NULL);
		else																													//	Component是Attribute, Entity或Relationship其中一種
		{
			if (positionSet.size() > 0)
			{
 				addNode(componentData[PARAMETER_TYPE],componentData[PARAMETER_TEXT], positionSet[positionSetCount].first, positionSet[positionSetCount].second);
				positionSetCount++;
			}
			else
				addNode(componentData[PARAMETER_TYPE],componentData[PARAMETER_TEXT], PARAMETER_TEXTUI_COORDINATES, PARAMETER_TEXTUI_COORDINATES);
		}
	}
}

//	重建Connection
int ERModel::recoveryConnection( vector<string> connectionsSet, int connectionCount, string connectionText)
{
	vector<string> connectionData;

	//	splitter.first是componentID, splitter.second是存有兩個互相Connect的ID
	connectionData = splitter(connectionsSet[connectionCount]).second;
	addConnection(atoi(connectionData[PARAMETER_SOURCENODE].c_str()),atoi(connectionData[PARAMETER_DESTINATIONNODE].c_str()),connectionText);
	connectionCount += 1;

	return connectionCount;
}

//	重建PK
void ERModel::recoveryPrimaryKey( vector<string> primaryKeysSet )
{
	//	primaryKeysSet is composed by entityNodeID, primarykey set.
	pair<string,vector<string>> splitPrimaryKey;
	vector<int> primaryKeyData;

	for (int i = 0; i < primaryKeysSet.size(); i++)
	{
		primaryKeyData.clear();

		//	splitter.first是EntityID, splitter.second是primary key的集合
		splitPrimaryKey = splitter(primaryKeysSet[i]);
		for (int j = 0; j < splitPrimaryKey.second.size(); j++)
			primaryKeyData.push_back(atoi(splitPrimaryKey.second[j].c_str()));

		setPrimaryKey(atoi(splitPrimaryKey.first.c_str()), primaryKeyData);
	}
}
//	回傳此ID以及與此ID有關的ID,例如： PK： 5 1,10,31 ,ID=5、有關的ID=1,10,31
pair<string,vector<string>> ERModel::splitter( string splitRowData )
{
	vector<string> splitBySpace;
	vector<string> splitResult;

	splitBySpace = Toolkit::splitFunction(splitRowData, TEXT_ONESPACE);
	splitResult = Toolkit::splitFunction(splitBySpace[PARAMETER_FILETEXT_RELATEDCOMPONENTID], COMMA);

	return make_pair(splitBySpace[PARAMETER_FILETEXT_COMPONENTID],splitResult);
}

void ERModel::saveERDiagram( string fileName )
{
	ofstream outputERDiagramFile;

	outputERDiagramFile.open(fileName);

	if (!outputERDiagramFile.is_open())
	{
		creatFilePath(fileName);
		outputERDiagramFile.open(fileName);
	}

	_isModify = false;

	SaveComponentVisitor* saveComponentVisitor = new SaveComponentVisitor();

	for(int i = 0; i < _components.size(); i++)
		_components[i]->accept(saveComponentVisitor);

	outputERDiagramFile << saveComponentVisitor->getERDiagramComponent();

	outputERDiagramFile.close();

	savePosition(fileName, saveComponentVisitor->getPositionInfo());
}

void ERModel::savePosition( string fileName, string positionInfo )
{
	ofstream outputPositionFile;
	string positionFileName = removeExtension(fileName) + TEXT_EXTENSION_POS;

	outputPositionFile.open(positionFileName);

	if (!outputPositionFile.is_open())
	{
		creatFilePath(fileName);
		outputPositionFile.open(fileName);
	}

	outputPositionFile << positionInfo;

	outputPositionFile.close();
}



void ERModel::saveXmlComponent( string fileName )
{
	ofstream outputXmlFile;

	outputXmlFile.open(fileName);

	if (!outputXmlFile.is_open())
	{
		creatFilePath(fileName);
		outputXmlFile.open(fileName);
	}

	_isModify = false;

	SaveXmlComponentVisitor* saveXmlComponentVisitor = new SaveXmlComponentVisitor();

	for(int i = 0; i < _components.size(); i++)
		_components[i]->accept(saveXmlComponentVisitor);

	outputXmlFile << saveXmlComponentVisitor->getXmlInfo();

	outputXmlFile.close();
}


void ERModel::creatFilePath( string fileName )
{
	string filePath;
	vector<string> splitterFilePath;

	splitterFilePath = Toolkit::splitFunction(fileName,SPLITTERBYBACKSLASH);

	//	最後的部分是檔案名稱
	for (int i = 0; i < (splitterFilePath.size()-1); i++)
	{
		if (splitterFilePath[i] != PARAMETER_NULL)
			filePath += splitterFilePath[i] + SPLITTERBYBACKSLASH;
	}
	_mkdir(filePath.c_str());
}

//	 將ComponentTable輸出為字串
string ERModel::saveComponentTable() 
{
	string componentTableString;

	for (int i = 0; i < _components.size(); i++)
	{
		if (_components[i]->getText() == PARAMETER_NULL)
			componentTableString += _components[i]->getType() + TEXT_ENDLINE;
		else
			componentTableString += _components[i]->getType() + TEXT_COMMASPACE + _components[i]->getText() + TEXT_ENDLINE;
	}

	return componentTableString;
}

//	 將ConnectionTable輸出為字串
string ERModel::saveConnectionTable() 
{
	string connectionTableString;

	for (int i = 0; i < _connections.size(); i++) 
	{
		connectionTableString += Toolkit::integerToString(_connections[i]->getID()) +  TEXT_ONESPACE 
			+ Toolkit::integerToString(static_cast<Connector*>(_connections[i])->getSourceNodeID()) + COMMA
			+ Toolkit::integerToString(static_cast<Connector*>(_connections[i])->getDestinationNodeID()) + TEXT_ENDLINE;
	}
	return connectionTableString;
}

//	 將全部的PK輸出為字串
string ERModel::savePrimaryKeyTable() 
{
	string primaryKeyTableString;
	vector<Component*> allEntityNode = searchSpecificTypeComponentSet(PARAMETER_ENTITY, _components);
	vector<int> pkofEntityNode;

	for (int i = 0; i < allEntityNode.size(); i++) 
	{
		pkofEntityNode = static_cast<NodeEntity*>(allEntityNode[i])->getPrimaryKey();
		if (!pkofEntityNode.empty())					// If the PK in entity node is not empty, output them. 
		{
			primaryKeyTableString += Toolkit::integerToString(allEntityNode[i]->getID()) + TEXT_ONESPACE; 
			for (int j = 0; j < pkofEntityNode.size(); j++)
			{
				if ( j == pkofEntityNode.size()-1 )		// Final PK
					primaryKeyTableString += Toolkit::integerToString(pkofEntityNode[j]);
				else
					primaryKeyTableString += Toolkit::integerToString(pkofEntityNode[j]) + COMMA;
			}
		}
		primaryKeyTableString += TEXT_ENDLINE;
	}
	return primaryKeyTableString;
}

//////////////////////////////////////////////////////////////////////////
//							Delete Node									//
//////////////////////////////////////////////////////////////////////////

//	針對Connector跟其他三種進行刪除
void ERModel::deleteFunction( int componentID )
{
	Component* delComponent = searchComponent(componentID);
	string deleteComponentIDSet;

	if (delComponent->getType() != PARAMETER_CONNECTOR)
	{
		deleteComponentIDSet = deleteComponent(delComponent);
	}
	else
	{
		deleteConnection(delComponent);
		deleteComponentIDSet = Toolkit::integerToString(componentID);
	}

	//	刪除完畢，通知observer更新
	notifyDeleteComplete(deleteComponentIDSet);
}

//	刪除Component(非Connector的)
string ERModel::deleteComponent( Component* delComponent )
{
	//	將要刪除的ID記錄下來
	string deleteComponentIDSet = Toolkit::integerToString(delComponent->getID());
	//	與要刪除的Component有關連的Component
	vector<Component*> relatedComponents = searchRelatedComponent(delComponent->getID());		// ComponentIDSet
	//	與要刪除的Component有關連的Connector ID
	vector<Component*> relatedConnectors = searchConnection(delComponent->getID());				// ConnectionIDSet

	// 記錄要刪除的ConnectorID
	for(int i = 0; i < relatedConnectors.size(); i++)
		deleteComponentIDSet += COMMA + Toolkit::integerToString(relatedConnectors[i]->getID());

	int relatedConnectionsEndIndex = relatedConnectors.size() - 1;
	int delID;

	//	走訪每個有關連的Component，並刪除其中connection的delComponent
	for (int i = 0; i < relatedComponents.size(); i++)
		relatedComponents[i]->deleteConnectedComponent(delComponent->getID());
	
	//	刪除Components中的Connector
	while(relatedConnectors.size() > PARAMETER_ZEROCOMPONENT)
	{
		delID = relatedConnectors[relatedConnectionsEndIndex]->getID();
		deleteTableSet(delID, _components, PARAMETER_COMPONENTSTABLE);
		deleteTableSet(delID, _connections, PARAMETER_CONNECTIONSTABLE);

		relatedConnectors.pop_back();
		relatedConnectionsEndIndex = relatedConnectors.size() - 1;
	}
	deleteTableSet(delComponent->getID(), _components, PARAMETER_COMPONENTSTABLE);

	return deleteComponentIDSet;
}

//	刪除Connector
void ERModel::deleteConnection( Component* delComponent )
{
	Connector* delConnector = static_cast<Connector*>(delComponent);
	Component* sourceNode = searchComponent(delConnector->getSourceNodeID());
	Component* destinationNode = searchComponent(delConnector->getDestinationNodeID());

	sourceNode->deleteConnectedComponent(delConnector->getDestinationNodeID());
	destinationNode->deleteConnectedComponent(delConnector->getSourceNodeID());

	deleteTableSet(delComponent->getID(), _components, PARAMETER_COMPONENTSTABLE);
	deleteTableSet(delComponent->getID(), _connections, PARAMETER_CONNECTIONSTABLE);
}

//	刪除在_components中的
void ERModel::deleteTableSet( int delID, vector<Component*> targetTableSet, int tableType )
{
	for(int i = 0; i < targetTableSet.size(); i++)
	{
		if (delID == targetTableSet[i]->getID())
		{
			switch(tableType)
			{
			case PARAMETER_COMPONENTSTABLE:
				_components.erase(_components.begin()+i);
				break;
			case PARAMETER_CONNECTIONSTABLE:
				_connections.erase(_connections.begin()+i);
				break;
			}
			break;
		}
	}
}

// 提供deleteGroup將componentIDSet的Connector抽出來，放到最前面
vector<int> ERModel::arrangeConnectorFirst( vector<int> componentIDSet )
{
	vector<int> resultSet;
	vector<int> nodeIDSet;

	// 區分Connector與其他Node(Attribute, Entity, Relationship)
	for (int i = 0; i < componentIDSet.size(); i++)
	{
		Component* targetComponent = searchComponent(componentIDSet[i]);
		if (targetComponent->getType() != PARAMETER_CONNECTOR)	// 把Attribute, Entity, Relationship與Connector分離
			nodeIDSet.push_back(componentIDSet[i]);
		else													// 先放入Connector
			resultSet.push_back(componentIDSet[i]);
	}

	// 合併
	for (int i = 0; i < nodeIDSet.size(); i++)
		resultSet.push_back(nodeIDSet[i]);

	return resultSet;
}

//////////////////////////////////////////////////////////////////////////
//							General Function							//
//////////////////////////////////////////////////////////////////////////

//	Using nodeID to search node, and return pointer of node
Component* ERModel::searchComponent( int searchNodeID )
{
	for( int i = 0; i < _components.size(); i++)
	{
		// Find!
		if (_components[i]->getID() == searchNodeID)
			return _components[i];
	}
	// Not find!
	return NULL;
}

//	The method is searching the nodeID in connections of the target component. 
bool ERModel::searchComponentConnection( int searchNodeID, string searchType,vector<Component*> targetComponent )
{
	for( int i = 0; i < targetComponent.size(); i++)
	{
		// Find!
		if (targetComponent[i]->getID() == searchNodeID && targetComponent[i]->getType() == searchType)
			return true;
	}
	// Not find!
	return false;
}

//	The method provides to textUI to check the component is exist.
bool ERModel::searchComponentExist( string searchID, string searchType)
{
	stringstream intNum;
	string intToStringNum;

	for( int i = 0; i < _components.size(); i++)
	{
		// int to string
		intNum << (_components[i]->getID()); // int to stringstream
		intNum >> intToStringNum; // stringstream to string

		// Find!
		if (intToStringNum == searchID && (_components[i]->getType() == searchType || searchType == PARAMETER_ALL))
			return true;

		intNum.clear();
	}
	// Not find!
	return false;
}

// Search components which is specific type in target component set, and return the specific components in target component set.
vector<Component*> ERModel::searchSpecificTypeComponentSet( string type, vector<Component*> targetComponetSet )
{
	vector<Component*> specificTypeComponentSet;

	for( int i = 0; i < targetComponetSet.size(); i++)
	{
		if (targetComponetSet[i]->getType() == type)
			specificTypeComponentSet.push_back(targetComponetSet[i]);
	}

	return specificTypeComponentSet;
}

//	從特定的vector中搜尋指定的type並回傳搜尋結果的內容
string ERModel::getComponentDataList( string type, vector<Component*> targetComponents ) 
{
	string ComponentDataList;

	for (int i = 0; i < targetComponents.size(); i++)
	{
		if (targetComponents[i]->getType() == type || type == PARAMETER_ALL)
		{
			ComponentDataList += TEXT_TWOSPACE + targetComponents[i]->getType() + TEXT_SPACELINE
				+ TEXT_TWOSPACE + Toolkit::integerToString(targetComponents[i]->getID()) + TEXT_SPACELINE
				+ TEXT_TWOSPACE + targetComponents[i]->getText() + TEXT_ENDLINE;
		}
	}	
	return ComponentDataList;
}

//	尋找與ComponentID有關連的Connection
vector<Component*> ERModel::searchConnection( int searchID )
{
	vector<Component*> resultSet;

	for (int i = 0; i < _connections.size(); i++)
	{
		if (static_cast<Connector*>(_connections[i])->getSourceNodeID() == searchID ||
			static_cast<Connector*>(_connections[i])->getDestinationNodeID() == searchID)
			resultSet.push_back(_connections[i]);
	}

	return resultSet;
}
//	搜尋與searchID有關係的Component，並回傳該集合
vector<Component*> ERModel::searchRelatedComponent( int searchID )
{
	vector<Component*> resultSet;

	for (int i = 0; i < _connections.size(); i++)
	{
		if (static_cast<Connector*>(_connections[i])->getSourceNodeID() == searchID)
			resultSet.push_back(searchComponent(static_cast<Connector*>(_connections[i])->getDestinationNodeID()));
		else if(static_cast<Connector*>(_connections[i])->getDestinationNodeID() == searchID)
			resultSet.push_back(searchComponent(static_cast<Connector*>(_connections[i])->getSourceNodeID()));
	}

	return resultSet;
}

int ERModel::getComponentTableSize()
{
	return _components.size();
}

int ERModel::getConnectionTableSize()
{
	return _connections.size();
}

void ERModel::setComponentID( int componentID )
{
	_componentID = componentID;
}

int ERModel::getComponentID()
{
	return _componentID;
}

void ERModel::resetERModel()
{
	for(int i = 0; i < _components.size(); i++)
		delete _components[i];
	_components.clear();
	_connections.clear();
	_componentID = 0;
}

void ERModel::sortCompoentsAndConnection()
{
	vector<Component*> sortComponent;
	vector<Component*> sortConnection;
	int sortNum = PARAMETER_SEARCHNUMBEGIN;

	while(sortComponent.size() != _components.size())
	{
		for (int i = 0; i < _components.size(); i++)
		{
			if (_components[i]->getID() == sortNum)
				sortComponent.push_back(_components[i]);
		}
		sortNum += 1;
	}
	
	sortNum = PARAMETER_SEARCHNUMBEGIN;
	while(sortConnection.size() != _connections.size())
	{
		for (int i = 0; i < _connections.size(); i++)
		{
			if (_connections[i]->getID() == sortNum)
				sortConnection.push_back(_connections[i]);
		}
		sortNum += 1;
	}

	_components = sortComponent;
	_connections = sortConnection;
	sortComponent.clear();
	sortConnection.clear();
}

bool ERModel::getIsModify()
{
	return _isModify;
}

void ERModel::setIsModify( bool isModify )
{
	_isModify = isModify;
}

//////////////////////////////////////////////////////////////////////////
//								GUI Function							//
//////////////////////////////////////////////////////////////////////////
string ERModel::getComponentForGUI()
{
	string ComponentDataList;

	for (int i = 0; i < _components.size(); i++)
		ComponentDataList += _components[i]->getType() + COMMA + _components[i]->getText() + SEMICOLON;

	return ComponentDataList.substr(0, ComponentDataList.size() + PARAMETER_ADJUSTSTRING);
}

string ERModel::getConnectionForGUI()
{
	string ConnectionDataList;

	for (int i = 0; i < _connections.size(); i++){
		ConnectionDataList += Toolkit::integerToString(static_cast<Connector*>(_connections[i])->getSourceNodeID()) + COMMA
						   + Toolkit::integerToString(static_cast<Connector*>(_connections[i])->getDestinationNodeID()) + COMMA
						   + _connections[i]->getText() + SEMICOLON;
	}

	return ConnectionDataList.substr(0, ConnectionDataList.size() + PARAMETER_ADJUSTSTRING);
}

string ERModel::getPrimaryKeyForGUI()
{
	string primaryKeyString;
	vector<Component*> allAttributeNode = searchSpecificTypeComponentSet(PARAMETER_ATTRIBUTE, _components);

	for (int i = 0; i < allAttributeNode.size(); i++) 
	{
		if (static_cast<NodeAttribute*>(allAttributeNode[i])->getIsPrimaryKey())
			primaryKeyString += allAttributeNode[i]->getIDString() + COMMA;
	}
	return primaryKeyString.substr(0, primaryKeyString.size() + PARAMETER_ADJUSTSTRING);
}

void ERModel::changeText( int targetNodeID, string editedText )
{
	searchComponent(targetNodeID)->setText(editedText);
}

bool ERModel::changePrimaryKey( int targetNodeID, bool isPrimaryKey )
{
	Component* targetNode = searchComponent(targetNodeID);
	if (static_cast<NodeAttribute*>(targetNode)->getIsConnectedEntity())
		static_cast<NodeAttribute*>(targetNode)->setIsPrimaryKey(isPrimaryKey);
	else
		return false;

	// Attribute如果有Connect的話，一定是跟Entity並且只有一個，所以當Attribute的PK變化的時候，Entity的PK也會改變
	if (targetNode->getConnections().size() != PARAMETER_ZEROCOMPONENT)
	{
		int relatedEntityID = targetNode->getConnections()[0]->getID();
		NodeEntity* relatedEntity = static_cast<NodeEntity*>(searchComponent(relatedEntityID));

		if (isPrimaryKey)
			relatedEntity->setPrimaryKey(targetNode->getID());
		else
			relatedEntity->deleteKeys(targetNodeID);
	}

	notifyPrimaryKeyChanged(targetNodeID, isPrimaryKey);

	return true;
}

void ERModel::notifyTextChanged( int targetNodeID, string editedText )
{
	Component* connectorComponent = searchComponent(targetNodeID);
	if (connectorComponent->getType() == PARAMETER_CONNECTOR)
	{
		int sourceNodeID = static_cast<Connector*>(connectorComponent)->getSourceNodeID();
		int destinationNodeID = static_cast<Connector*>(connectorComponent)->getDestinationNodeID();
		if (checkSetCardinality(sourceNodeID, destinationNodeID))
		{
			if (editedText != "1" && editedText != "N")
				editedText = "1";
		}
		else
			editedText = "";
	}

	Subject::notifyTextChanged(targetNodeID, editedText);
}

void ERModel::copyComponent( vector<int> targetNodeIDSet )
{
	resetClipboard();
	cloneItemIntoClipboard(targetNodeIDSet);
}

void ERModel::resetClipboard()
{
	for (int i = 0 ; i < _clipboard.size(); i++)
		delete _clipboard[i];
	_clipboard.clear();

	_recoredIDBoard.clear();
	
}

void ERModel::cloneItemIntoClipboard( vector<int> targetNodeIDSet )
{
	Component* targetNode;
	for (int i = 0; i < targetNodeIDSet.size(); i++)
	{
		targetNode = searchComponent(targetNodeIDSet[i]);
		Component* cloneNode = targetNode->deepClone();
		_clipboard.push_back(cloneNode);
	}
}

vector<Component*> ERModel::getClipboard()
{
	return _clipboard;
}

int ERModel::addClone( Component* cloneComponent )
{
	// 處理非Connector
	if (cloneComponent->getType() != PARAMETER_CONNECTOR)
		return addCloneComponent(cloneComponent);
	// 處理Connector
	else
		return addCloneConnection(cloneComponent);
}

// CloneComponent做調整後放入，並回傳調整過後的ID
int ERModel::addCloneComponent( Component* cloneComponent )
{
	// 清除Clone過來的Connection資料，因為Clone的Connection資料是連回原本的Component
	cloneComponent->deleteAllRelatedInfo();

	// 清除Attribute連結過Entity的記錄
	if (cloneComponent->getType() == PARAMETER_ATTRIBUTE)
		static_cast<NodeAttribute*>(cloneComponent)->setIsConnectedEntity(false);

	// 對clone的ComponentID做修正
	cloneComponent->setID(_componentID);
	// ComponentID往後加
	_componentID++;

	// 稍微位移
	cloneComponent->setSx(cloneComponent->getSx() + PARAMETER_ADJUSTPOSITION);
	cloneComponent->setSy(cloneComponent->getSy() + PARAMETER_ADJUSTPOSITION);

	_components.push_back(cloneComponent);

	notifyAddNewNode(cloneComponent->getID(), cloneComponent->getType(), cloneComponent->getText(), cloneComponent->getSx(), cloneComponent->getSy());

	return cloneComponent->getID();
}

int ERModel::addCloneConnection( Component* cloneConnector )
{
	Connector* tempCloneConnector = static_cast<Connector*>(cloneConnector);

	int newSourceID = retrieveNewCloneID(tempCloneConnector->getSourceNodeID());
	int newDestinationID = retrieveNewCloneID(tempCloneConnector->getDestinationNodeID());

	if (newSourceID != PARAMETER_NOTFINDID && newDestinationID != PARAMETER_NOTFINDID)
	{
		Component* newSourceNode = searchComponent(newSourceID);
		Component* newDestinationNode = searchComponent(newDestinationID);

		// 對clone的ComponentID做修正
		tempCloneConnector->setID(_componentID);
		// ComponentID往後加
		_componentID++;
		// 設定兩個Clone的Component重新Connect
		connectionSetting(newSourceNode, newDestinationNode, tempCloneConnector->getText());
		// Connector設定新的Source及Destination
		tempCloneConnector->setSourceNode(newSourceNode);
		tempCloneConnector->setDestinationNode(newDestinationNode);

		_components.push_back(cloneConnector);
		_connections.push_back(tempCloneConnector);

		notifyNewConnection(tempCloneConnector->getID(), tempCloneConnector->getSourceNodeID(), tempCloneConnector->getDestinationNodeID(), tempCloneConnector->getText());

		return cloneConnector->getID();
	}

	return PARAMETER_ISERROR;
}

// 儲存CloneComponent的ID變化
void ERModel::recordNewCloneComponentID( int originalCloneComponentID,int newCloneComponentID )
{
	_recoredIDBoard.push_back(make_pair(originalCloneComponentID, newCloneComponentID));
}

int ERModel::retrieveNewCloneID( int originalID )
{
	int newCloneID = PARAMETER_NOTFINDID;

	for (int i = 0; i < _recoredIDBoard.size(); i++)
	{
		if (_recoredIDBoard[i].first == originalID)
		{
			newCloneID = _recoredIDBoard[i].second;
			_recoredIDBoard.erase(_recoredIDBoard.begin()+i);
			break;
		}
	}
	return newCloneID;
}

bool ERModel::getTargetAttributeIsPrimaryKey( int targetNodeID )
{
	NodeAttribute* targetAttribute = static_cast<NodeAttribute*>(searchComponent(targetNodeID));

	return targetAttribute->getIsPrimaryKey();
}

void ERModel::movedComponentPosition( int targetNodeID, int moveDistance_x, int moveDistance_y )
{
	Component* movedComponent = searchComponent(targetNodeID);
	movedComponent->setSx(movedComponent->getSx() + moveDistance_x);
	movedComponent->setSy(movedComponent->getSy() + moveDistance_y);
}

void ERModel::setInitialPosition( int targetNodeID, int initialSx, int initialSy )
{
	Component* movedComponent = searchComponent(targetNodeID);
	movedComponent->setSx(initialSx);
	movedComponent->setSy(initialSy);
}

// 回傳一個還沒加副檔名的字串
string ERModel::removeExtension( string fileName )
{
	vector<string> fileNameSplitedSet = Toolkit::splitFunction(fileName, DOT);
	string positionFileName;

	// 輸入至少有一個"."存在，表示有副檔名
	if (fileNameSplitedSet.size() > 1)
	{
		// 不要副檔名
		for (int i = 0; i < fileNameSplitedSet.size() + PARAMETER_MINUSONE; i++)
		{
			positionFileName += fileNameSplitedSet[i];
			positionFileName += DOT;
		}
	}
	else
		positionFileName = fileName + DOT;

	// xxxxfile.
	return positionFileName;
}

vector<int> ERModel::getTargetPosition( int targetNodeID )
{
	Component* targetNode = searchComponent(targetNodeID);
	vector<int> positionSet;

	positionSet.push_back(targetNode->getSx());
	positionSet.push_back(targetNode->getSy());

	return positionSet;
}

std::string ERModel::getHTMLERDiagramTable()
{

}
