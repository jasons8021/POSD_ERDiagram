#include "NodeAttribute.h"
#include "ComponentVisitor.h"

NodeAttribute::NodeAttribute( int id, string text, int sx, int sy ) : Node( id, PARAMETER_ATTRIBUTE, text, sx, sy )
{
	_connectedEntity = false;
	_isPrimaryKey = false;
}

NodeAttribute::~NodeAttribute()
{

}

void NodeAttribute::connectTo( Component* targetNode )
{
	Component::connectTo(targetNode);
	_connectedEntity = true;
}

string NodeAttribute::canConnectTo( Component* targetNode )
{
	string checkMessage = Component::canConnectTo(targetNode);

	if (checkMessage == TEXT_CONNECTION_CANCONNECT)		
	{
		if (!(_connectedEntity) && targetNode->getType() == PARAMETER_ENTITY)						// 可以跟目標Component進行connect
			return TEXT_CONNECTION_CANCONNECT;
		else											// 此Attribute已經連結過Entity
			return TEXT_NODENUMBEGIN + targetNode->getIDString() + TEXT_CONNECTION_NOTCONNECTION + this->getIDString() + TEXT_ENDTEXT;
	}
	else												// 與目標Component已經connect 或 connect自己
		return checkMessage;
	
}

bool NodeAttribute::getIsPrimaryKey()
{
	return this->_isPrimaryKey;
}

void NodeAttribute::setIsPrimaryKey( bool isPK )
{
	this->_isPrimaryKey = isPK;
}

void NodeAttribute::deleteConnectedComponent( int delComponentID )
{
	Component::deleteConnectedComponent(delComponentID);
	_connectedEntity = false;
	_isPrimaryKey = false;
}

bool NodeAttribute::getIsConnectedEntity()
{
	return _connectedEntity;
}

Component* NodeAttribute::deepClone()
{
	return new NodeAttribute(*this);
}

void NodeAttribute::deleteAllRelatedInfo()
{
	Component::deleteAllRelatedInfo();
	_connectedEntity = false;
	_isPrimaryKey = false;
}

void NodeAttribute::setIsConnectedEntity( bool isConnectedEntity )
{
	_connectedEntity = isConnectedEntity;
}

void NodeAttribute::accept( ComponentVisitor* visitor )
{
	visitor->visit(this);
}
