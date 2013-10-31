#include "DeleteComponentCmd.h"

DeleteComponentCmd::DeleteComponentCmd(ERModel* erModel, int componentID)
{
	_erModel = erModel;
	_componentID = componentID;
	_type = PARAMETER_NULLSTRING;
	_text = PARAMETER_NULLSTRING;
	_sourceNodeID = PARAMETER_NOVALUE;
	_destinationNodeID = PARAMETER_NOVALUE;
}

DeleteComponentCmd::~DeleteComponentCmd()
{
}

void DeleteComponentCmd::execute()
{
	if (_type == PARAMETER_NULLSTRING && _text == PARAMETER_NULLSTRING)							//	第一次做Delete，將刪除Component的資訊記錄下來
		storeInformation();
	
	_erModel->deleteFunction(_componentID);
}

void DeleteComponentCmd::unexecute()
{
	if (_type != PARAMETER_CONNECTOR)
	{
		_componentID = _erModel->addNode(_componentID, _type, _text);
		reBuildRelatedConnection();

		if (_type == PARAMETER_ENTITY)
			_erModel->setPrimaryKey(_componentID, _primaryKeys);
		else if (_type == PARAMETER_ATTRIBUTE && !_primaryKeys.empty())
			_erModel->reBuildPrimaryKeyFromAttribute(_componentID, _primaryKeys[0]);
	}
	else	//	Connector
		_componentID = _erModel->addConnection(_componentID, _sourceNodeID, _destinationNodeID, _text);
	_erModel->setIsModify(false);
	_erModel->sortCompoentsAndConnection();
}

//	儲存刪除Component的相關資訊
void DeleteComponentCmd::storeInformation() 
{
	Component* delcomponent = _erModel->searchComponent(_componentID);
	_type = delcomponent->getType();
	_text = delcomponent->getText();

	if (_type == PARAMETER_CONNECTOR)
	{
		_sourceNodeID = static_cast<Connector*>(delcomponent)->getSourceNodeID();
		_destinationNodeID = static_cast<Connector*>(delcomponent)->getDestinationNodeID();
	}
	//	刪除EntityNode，加回來時需要重新設PrimaryKey
	else if (_type == PARAMETER_ENTITY)
		_primaryKeys = static_cast<NodeEntity*>(delcomponent)->getPrimaryKey();
	//	刪除AttributeNode，加回來需要到EntityNode中重新加入PrimaryKey(如果是PK的)
	else if (_type == PARAMETER_ATTRIBUTE)
	{
		if (static_cast<NodeAttribute*>(delcomponent)->getIsPrimaryKey())
		{
			//	delcomponent->getConnections()[0]是與Attribute相連的Entity，因為一個Attribute只能跟一個Entity做Connected且不能跟Attribute或Relationship連，所以寫[0]
			_primaryKeys.push_back(delcomponent->getConnections()[0]->getID());
		}
	}

	if (_type != PARAMETER_CONNECTOR)
		relatedComponentInformation();
}

//	與刪除Component有connected的Component
void DeleteComponentCmd::relatedComponentInformation()
{
	vector<Component*> relatedConnectorSet = _erModel->searchConnection(_componentID);
	Component* relatedConnector;
	vector<int> connectionInfo;

	for(int i = 0; i < relatedConnectorSet.size(); i++)
	{
		relatedConnector = _erModel->searchComponent(relatedConnectorSet[i]->getID());
		connectionInfo.clear();

		//	connectorID, deleteComponentID, 與deleteComponent做connected的component的ID
		connectionInfo.push_back(static_cast<Connector*>(relatedConnectorSet[i])->getID());
		
		if (static_cast<Connector*>(relatedConnectorSet[i])->getSourceNodeID() == _componentID)
		{
			connectionInfo.push_back(_componentID);
			connectionInfo.push_back(static_cast<Connector*>(relatedConnectorSet[i])->getDestinationNodeID());
		}
		else
		{
			connectionInfo.push_back(static_cast<Connector*>(relatedConnectorSet[i])->getSourceNodeID());
			connectionInfo.push_back(_componentID);
		}
		_relatedConnections.push_back(connectionInfo);

		//	connectorID的Text (紀錄Cardinality)
		_relatedConnectionText.push_back(relatedConnector->getText());
	}
}

//	重建被刪掉Component的Connection
void DeleteComponentCmd::reBuildRelatedConnection()
{
	for (int i = 0; i < _relatedConnections.size(); i++)
	{
		//	_relatedConnections[i][0]是ConnectorID、_relatedConnections[i][1]是sourceNodeID、_relatedConnections[i][2]是destinationNodeID、_relatedConnectionText[i]是Cardinatlity
		_erModel->addConnection(_relatedConnections[i][0], _relatedConnections[i][1], _relatedConnections[i][2], _relatedConnectionText[i]);
	}
}
