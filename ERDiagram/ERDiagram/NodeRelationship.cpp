#include "NodeRelationship.h"
#include <iostream>

NodeRelationship::NodeRelationship( int id, string text ) : Node( id, PARAMETER_RELATIONSHIP, text )
{
}

NodeRelationship::~NodeRelationship()
{
 	_entityCardinalitySet.clear();
}

void NodeRelationship::connectTo( Component* targetNode )
{
	Component::connectTo(targetNode);
}

string NodeRelationship::canConnectTo( Component* targetNode )
{
	string checkMessage = Component::canConnectTo(targetNode);

	if (checkMessage == TEXT_CONNECTION_CANCONNECT && !(this->searchConnections(targetNode->getID())))		// TargetNode can be connected.
	{
		if (targetNode->getType() == PARAMETER_ENTITY)	// 可以連結
			return TEXT_CONNECTION_CANCONNECT;
		else											// 型態錯誤
			return TEXT_NODENUMBEGIN + targetNode->getIDString() + TEXT_CONNECTION_NOTCONNECTION + this->getIDString() + TEXT_ENDTEXT;
	}
	else												// 已經連過或連結自己
		return checkMessage;
}

void NodeRelationship::setEntityCardinality( pair<int, string> entityCardinality )
{
	this->_entityCardinalitySet.push_back(entityCardinality);
}

vector<pair<int, string>> NodeRelationship::getEntityCardinality()
{
	return this->_entityCardinalitySet;
}

void NodeRelationship::deleteConnectedComponent( int delComponentID )
{
	Component::deleteConnectedComponent(delComponentID);
	deleteEntityCardinality(delComponentID);
}

void NodeRelationship::deleteEntityCardinality( int delComponentID )
{
	for(int i = 0; i < _entityCardinalitySet.size(); i++)
	{
		if (_entityCardinalitySet[i].first == delComponentID)
			_entityCardinalitySet.erase(_entityCardinalitySet.begin()+i);
	}
}
