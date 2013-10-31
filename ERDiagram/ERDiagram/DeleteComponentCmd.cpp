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
	if (_type == PARAMETER_NULLSTRING && _text == PARAMETER_NULLSTRING)							//	�Ĥ@����Delete�A�N�R��Component����T�O���U��
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

//	�x�s�R��Component��������T
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
	//	�R��EntityNode�A�[�^�Ӯɻݭn���s�]PrimaryKey
	else if (_type == PARAMETER_ENTITY)
		_primaryKeys = static_cast<NodeEntity*>(delcomponent)->getPrimaryKey();
	//	�R��AttributeNode�A�[�^�ӻݭn��EntityNode�����s�[�JPrimaryKey(�p�G�OPK��)
	else if (_type == PARAMETER_ATTRIBUTE)
	{
		if (static_cast<NodeAttribute*>(delcomponent)->getIsPrimaryKey())
		{
			//	delcomponent->getConnections()[0]�O�PAttribute�۳s��Entity�A�]���@��Attribute�u���@��Entity��Connected�B�����Attribute��Relationship�s�A�ҥH�g[0]
			_primaryKeys.push_back(delcomponent->getConnections()[0]->getID());
		}
	}

	if (_type != PARAMETER_CONNECTOR)
		relatedComponentInformation();
}

//	�P�R��Component��connected��Component
void DeleteComponentCmd::relatedComponentInformation()
{
	vector<Component*> relatedConnectorSet = _erModel->searchConnection(_componentID);
	Component* relatedConnector;
	vector<int> connectionInfo;

	for(int i = 0; i < relatedConnectorSet.size(); i++)
	{
		relatedConnector = _erModel->searchComponent(relatedConnectorSet[i]->getID());
		connectionInfo.clear();

		//	connectorID, deleteComponentID, �PdeleteComponent��connected��component��ID
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

		//	connectorID��Text (����Cardinality)
		_relatedConnectionText.push_back(relatedConnector->getText());
	}
}

//	���سQ�R��Component��Connection
void DeleteComponentCmd::reBuildRelatedConnection()
{
	for (int i = 0; i < _relatedConnections.size(); i++)
	{
		//	_relatedConnections[i][0]�OConnectorID�B_relatedConnections[i][1]�OsourceNodeID�B_relatedConnections[i][2]�OdestinationNodeID�B_relatedConnectionText[i]�OCardinatlity
		_erModel->addConnection(_relatedConnections[i][0], _relatedConnections[i][1], _relatedConnections[i][2], _relatedConnectionText[i]);
	}
}
