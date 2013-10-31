#include "NodeEntity.h"

NodeEntity::NodeEntity( int id, string text ) : Node( id, PARAMETER_ENTITY, text )
{
	_isShowForeignKeyinERTable = false;
	_foreignKeySet.clear();
}

NodeEntity::~NodeEntity()
{
	_foreignKeySet.clear();
	_primaryKeySet.clear();
}

void NodeEntity::connectTo( Component* targetNode )
{	
	Component::connectTo(targetNode);
}

string NodeEntity::canConnectTo( Component* targetNode )
{
	string checkMessage = Component::canConnectTo(targetNode);

	if (checkMessage == TEXT_CONNECTION_CANCONNECT)
	{
		if (targetNode->getType() == PARAMETER_RELATIONSHIP || targetNode->getType() == PARAMETER_ATTRIBUTE)	
			return TEXT_CONNECTION_CANCONNECT;		// 可以相連
		else										// 型態錯誤
			return TEXT_NODENUMBEGIN + targetNode->getIDString() + TEXT_CONNECTION_NOTCONNECTION + this->getIDString() + TEXT_ENDTEXT;
	}
	else											// 已經連過或連到自己
		return checkMessage;
}

void NodeEntity::setForeignKey( int foreignKeyOfEntityID )
{
	bool checkExist = false;
	//	pair<int,vector<int>> means that (entityID, entityPKset).
	for (int i = 0; i < _foreignKeySet.size(); i++)
	{
		if (_foreignKeySet[i] == foreignKeyOfEntityID)
			checkExist = true;
	}
	if (!checkExist)
		_foreignKeySet.push_back(foreignKeyOfEntityID);
}

vector<int> NodeEntity::getForeignKey()
{
	return this->_foreignKeySet;
}

void NodeEntity::setPrimaryKey( int primaryKeyOfAttributeID )
{
	bool checkExist = false;
	//	Check is existed or not.
	for (int i = 0; i < _primaryKeySet.size(); i++)
	{
		if (_primaryKeySet[i] == primaryKeyOfAttributeID)
			checkExist = true;
	}
	if (!checkExist)
		_primaryKeySet.push_back(primaryKeyOfAttributeID);
}

vector<int> NodeEntity::getPrimaryKey()
{
	return this->_primaryKeySet;
}

void NodeEntity::deleteConnectedComponent( int delComponentID )
{
	Component::deleteConnectedComponent(delComponentID);
	deleteKeys(delComponentID);
}

//	刪除與delComponent有關的ForeignKey跟PrimaryKey
void NodeEntity::deleteKeys( int delComponentID )
{
	for (int i = 0; i < _primaryKeySet.size(); i++)
	{
		if (_primaryKeySet[i] == delComponentID)
			_primaryKeySet.erase(_primaryKeySet.begin()+i);
	}

	for (int i = 0; i < _foreignKeySet.size(); i++)
	{
		if (_foreignKeySet[i] == delComponentID)
			_foreignKeySet.erase(_foreignKeySet.begin()+i);
	}
}

bool NodeEntity::getIsShowForeignKeyinERTable()
{
	return _isShowForeignKeyinERTable;
}

void NodeEntity::setIsShowForeignKeyinERTable( bool isShowForeignKey )
{
	_isShowForeignKeyinERTable = isShowForeignKey;
}
