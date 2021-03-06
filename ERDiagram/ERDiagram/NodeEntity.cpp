#include "NodeEntity.h"
#include "ComponentVisitor.h"

NodeEntity::NodeEntity( int id, string text, int sx, int sy ) : Node( id, PARAMETER_ENTITY, text, sx, sy )
{
	_isShowForeignKeyinERTable.clear();
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
	if (targetNode->getType() == PARAMETER_ATTRIBUTE)
		static_cast<NodeAttribute*>(targetNode)->setIsConnectedEntity(true);
}

string NodeEntity::canConnectTo( Component* targetNode )
{
	string checkMessage = Component::canConnectTo(targetNode);

	if (checkMessage == TEXT_CONNECTION_CANCONNECT)
	{
		if (targetNode->getType() == PARAMETER_RELATIONSHIP)
			return TEXT_CONNECTION_CANCONNECT;		// 可以相連
		else if (targetNode->getType() == PARAMETER_ATTRIBUTE)
		{
			if (!static_cast<NodeAttribute*>(targetNode)->getIsConnectedEntity())
				return TEXT_CONNECTION_CANCONNECT;		// 可以相連
			else
				return TEXT_NODENUMBEGIN + targetNode->getIDString() + TEXT_CONNECTION_NOTCONNECTION + this->getIDString() + TEXT_ENDTEXT;
		}
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

vector<int> NodeEntity::getIsShowForeignKeyinERTable()
{
	return _isShowForeignKeyinERTable;
}

void NodeEntity::setIsShowForeignKeyinERTable( bool isShowForeignKey )
{
	if (isShowForeignKey)
		_isShowForeignKeyinERTable.push_back(PARAMETER_TRUE);
	else
		_isShowForeignKeyinERTable.push_back(PARAMETER_FALSE);
}

Component* NodeEntity::deepClone()
{
	return new NodeEntity(*this);
}

void NodeEntity::deleteAllRelatedInfo()
{
	Component::deleteAllRelatedInfo();
	_isShowForeignKeyinERTable.clear();
	_primaryKeySet.clear();
	_foreignKeySet.clear();
}

void NodeEntity::accept( ComponentVisitor* visitor )
{
	visitor->visit(this);
}
