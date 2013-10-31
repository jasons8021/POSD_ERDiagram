#include "ERModel.h"
#include <algorithm>

ERModel::ERModel()
{
	_componentID = 0;
	_isModify = false;
}

ERModel::~ERModel()
{
	resetERModel();
}

//////////////////////////////////////////////////////////////////////////
//							AddNode										//
//////////////////////////////////////////////////////////////////////////

//	componentID���W�����p�U�ϥ�
int ERModel::addNode( string type, string text )
{
	return addNode(_componentID++, type, text);
}

//	���wcomponentID�����p�ϥΡAeg:undo�Bredo�A�æ^��componentID
int ERModel::addNode( int componentID, string type, string text )
{
	ComponentFactory* componentFactory = new ComponentFactory();
	
	Component* newComponent;
	newComponent = static_cast<Component*>(componentFactory->creatComponent(componentID, type, text));
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
		newConnection = static_cast<Component*>(componentFactory->creatComponent(componentID, PARAMETER_CONNECTOR, text));
		_components.push_back(newConnection);

		static_cast<Connector*>(newConnection)->setConnectedNode(sourceNode, destinationNode);
		_connections.push_back(newConnection);

		sourceNode->connectTo(destinationNode);

		if (checkSetCardinality(sourceNodeID, destinationNodeID))
			setCardinality(sourceNode, destinationNode, text);
	}
	else
		return PARAMETER_ISERROR;
	delete componentFactory;

	return newConnection->getID();
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

//	�j�M�S�w��type(��all type)�A�æ^�Ƿj�M���G��dataList
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

		// �Ϊťն�
		while (erDiagramRowString.size() < PARAMETER_FILLUPSIZE)
			erDiagramRowString += TEXT_ONESPACE;

		erDiagramRowString += TEXT_LINESPACE;
		attributeInEntitySet = searchSpecificTypeComponentSet(PARAMETER_ATTRIBUTE, entitySet[i]->getConnections());
		erDiagramRowString += getAttributeContents(attributeInEntitySet);
		
		for(int j = 0; j < static_cast<NodeEntity*>(entitySet[i])->getForeignKey().size(); j++)
		{
			if (static_cast<NodeEntity*>(entitySet[i])->getIsShowForeignKeyinERTable())
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

	// �R���̫�@��"," �ñN�榡��Jstring��
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
	// entityCardinalitySet�O�z�Lrelationship�۳s�����entity�����X�A�榡��(entity, cardinality).
	vector<pair<int,string>> entityCardinalitySet = relationshipNode->getEntityCardinality();
	vector<int> oneToOneEntities;

	if (entityCardinalitySet.size() < PARAMETER_RELATIONSHIPLOWERBOUND)									// Relationship�u��connect�@��entity.
		return oneToOneEntities;
	else
	{
		if (entityCardinalitySet[PARAMETER_FIRSTENTITY].second == "1" && entityCardinalitySet[PARAMETER_SECONDENTITY].second == "1")				// One to one
		{
			oneToOneEntities.push_back(entityCardinalitySet[PARAMETER_FIRSTENTITY].first);
			oneToOneEntities.push_back(entityCardinalitySet[PARAMETER_SECONDENTITY].first);
			return oneToOneEntities;
		}
		else																							// One to N �� N to one �� N to N
			return oneToOneEntities;
	}
}

bool ERModel::checkOneToOne()
{
	vector<Component*> relationshipNode = searchSpecificTypeComponentSet(PARAMETER_RELATIONSHIP, _components);

	// entityCardinalitySet�O�z�Lrelationship�۳s�����entity�����X�A�榡��(entity, cardinality).
	vector<pair<int,string>> entityCardinalitySet;

	for(int i = 0; i < relationshipNode.size(); i++){
		entityCardinalitySet = static_cast<NodeRelationship*>(relationshipNode[i])->getEntityCardinality();
		if (entityCardinalitySet.size() >= PARAMETER_RELATIONSHIPLOWERBOUND)								// �ˬdrelationship�ܤֳs����entity.
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

	// �R���̫�@��", "�ñN�榡��Jstring��
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
	ifstream inputERDiagramFile;
	inputERDiagramFile.open(fileName);

	if (!inputERDiagramFile.is_open())
		return TEXT_LOADSAVE_OPENFAILED;

	resetERModel();

	//	���o��J�ɮת��Ҧ��r��
	string erDiagramText((istreambuf_iterator<char>(inputERDiagramFile)),istreambuf_iterator<char>());
	inputERDiagramFile.close();

	recoveryFile(classifyInputFile(erDiagramText));
	
	return TEXT_LOADSAVE_OPENSUCCESS;
}

//	�N��J��.erd�ɤ������e�����A����ComponentsSet�BConnectionsSet�BPrimaryKeysSet
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

//	���ؿ�J(.erd)��ERDiagram. FileSet's vector[PARAMETER_COMPONENTSET]�Ocomponents�����X, vector[PARAMETER_CONNECTIONSET]�Oconnections�����X, vector[PARAMETER_PRIMARYKEYSET]�Oprimary keys�����X
void ERModel::recoveryFile( vector<vector<string>> fileSet )
{
 	recoveryAllComponent(fileSet[PARAMETER_COMPONENTSET], fileSet[PARAMETER_CONNECTIONSET]);
 	recoveryPrimaryKey(fileSet[PARAMETER_PRIMARYKEYSET]);
}
//	�N���Ϋ᪺�ɮפ��e�A�w��Component��Connection���[�J
void ERModel::recoveryAllComponent( vector<string> componentsSet, vector<string> connectionsSet )
{
	vector<string> componentData;
	
	int connectionSetCount = 0;
	for(int i = 0; i < componentsSet.size(); i++)
	{
		componentData = Toolkit::splitFunction(componentsSet[i], TEXT_COMMASPACE);

		if ( componentData[PARAMETER_TYPE] == PARAMETER_CONNECTOR && componentData.size() > PARAMETER_HAVETYPEANDTEXT )			//	Connector��Cardinality
			connectionSetCount = recoveryConnection(connectionsSet, connectionSetCount, componentData[1]);
		else if ( componentData[PARAMETER_TYPE] == PARAMETER_CONNECTOR )														//	Connector�S��Cardinality
			connectionSetCount = recoveryConnection(connectionsSet, connectionSetCount, PARAMETER_NULL);
		else																													//	Component�OAttribute, Entity��Relationship�䤤�@��
 			addNode(componentData[PARAMETER_TYPE],componentData[PARAMETER_TEXT]);
	}
}

//	����Connection
int ERModel::recoveryConnection( vector<string> connectionsSet, int connectionCount, string connectionText)
{
	vector<string> connectionData;

	//	splitter.first�OcomponentID, splitter.second�O�s����Ӥ���Connect��ID
	connectionData = splitter(connectionsSet[connectionCount]).second;
	addConnection(atoi(connectionData[PARAMETER_SOURCENODE].c_str()),atoi(connectionData[PARAMETER_DESTINATIONNODE].c_str()),connectionText);
	connectionCount += 1;

	return connectionCount;
}

//	����PK
void ERModel::recoveryPrimaryKey( vector<string> primaryKeysSet )
{
	//	primaryKeysSet is composed by entityNodeID, primarykey set.
	pair<string,vector<string>> splitPrimaryKey;
	vector<int> primaryKeyData;

	for (int i = 0; i < primaryKeysSet.size(); i++)
	{
		primaryKeyData.clear();

		//	splitter.first�OEntityID, splitter.second�Oprimary key�����X
		splitPrimaryKey = splitter(primaryKeysSet[i]);
		for (int j = 0; j < splitPrimaryKey.second.size(); j++)
			primaryKeyData.push_back(atoi(splitPrimaryKey.second[j].c_str()));

		setPrimaryKey(atoi(splitPrimaryKey.first.c_str()), primaryKeyData);
	}
}
//	�^�Ǧ�ID�H�λP��ID������ID,�Ҧp�G PK�G 5 1,10,31 ,ID=5�B������ID=1,10,31
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

	outputERDiagramFile << saveComponentTable();
	outputERDiagramFile << TEXT_ENDLINE;
	outputERDiagramFile << saveConnectionTable();
	outputERDiagramFile << TEXT_ENDLINE;
	outputERDiagramFile << savePrimaryKeyTable();

	outputERDiagramFile.close();
}

void ERModel::creatFilePath( string fileName )
{
	string filePath;
	vector<string> splitterFilePath;

	splitterFilePath = Toolkit::splitFunction(fileName,SPLITTERBYBACKSLASH);

	//	�̫᪺�����O�ɮצW��
	for (int i = 0; i < (splitterFilePath.size()-1); i++)
	{
		if (splitterFilePath[i] != PARAMETER_NULL)
			filePath += splitterFilePath[i] + SPLITTERBYBACKSLASH;
	}
	_mkdir(filePath.c_str());
}

//	 �NComponentTable��X���r��
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

//	 �NConnectionTable��X���r��
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

//	 �N������PK��X���r��
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

//	�w��Connector���L�T�ضi��R��
void ERModel::deleteFunction( int componentID )
{
	Component* delComponent = searchComponent(componentID);

	if (delComponent->getType() != PARAMETER_CONNECTOR)
		deleteComponent(delComponent);
	else
		deleteConnection(delComponent);
}

//	�R��Component(�DConnector��)
void ERModel::deleteComponent( Component* delComponent )
{
	//	�P�n�R����Component�����s��Component
	vector<Component*> relatedComponents = searchRelatedComponent(delComponent->getID());
	//	�P�n�R����Component�����s��Connector ID
	vector<Component*> relatedConnectors = searchConnection(delComponent->getID());
	int relatedConnectionsEndIndex = relatedConnectors.size() - 1;
	int delID;

	//	���X�C�Ӧ����s��Component�A�çR���䤤connection��delComponent
	for (int i = 0; i < relatedComponents.size(); i++)
		relatedComponents[i]->deleteConnectedComponent(delComponent->getID());
	
	//	�R��Components����Connector
	while(relatedConnectors.size() > PARAMETER_ZEROCOMPONENT)
	{
		delID = relatedConnectors[relatedConnectionsEndIndex]->getID();
		deleteTableSet(delID, _components, PARAMETER_COMPONENTSTABLE);
		deleteTableSet(delID, _connections, PARAMETER_CONNECTIONSTABLE);

		relatedConnectors.pop_back();
		relatedConnectionsEndIndex = relatedConnectors.size() - 1;
	}
	deleteTableSet(delComponent->getID(), _components, PARAMETER_COMPONENTSTABLE);
}

//	�R��Connector
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

//	�R���b_components����
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

//////////////////////////////////////////////////////////////////////////
//							Command Pattern								//
//////////////////////////////////////////////////////////////////////////

void ERModel::addNodeCmd( string type, string text )
{
	_cmdManager.execute(new AddComponentCmd(this, type, text));
}

void ERModel::addConnectionCmd( int sourceNodeID, int destinationNodeID, string text )
{
	_cmdManager.execute(new ConnectComponentsCmd(this, sourceNodeID, destinationNodeID, text));
}

void ERModel::deleteCmd( int delComponentID )
{
	_cmdManager.execute(new DeleteComponentCmd(this, delComponentID));
}

bool ERModel::undoCmd()
{
	return _cmdManager.undo();
}

bool ERModel::redoCmd()
{
	return _cmdManager.redo();
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

//	�q�S�w��vector���j�M���w��type�æ^�Ƿj�M���G�����e
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

//	�M��PComponentID�����s��Connection
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
//	�j�M�PsearchID�����Y��Component�A�æ^�ǸӶ��X
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
