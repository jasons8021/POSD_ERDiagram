#include "Component.h"

Component::Component()
{
}

Component::Component(int id, string type, string text)
{
	this->_id = id;
	this->_type = type;
	this->_text = text;
}

Component::~Component()
{
	_connections.clear();
}

int Component::getID()
{
	return this->_id;
}

string Component::getType()
{
	return this->_type;
}

string Component::getText()
{
	return this->_text;
}

void Component::connectTo( Component* targetNode )
{
	// TargetNode isn't in connections
	if(!(searchConnections(targetNode->getID())))
	{
		pushConnection(targetNode);								// Push connected Node into connections
		targetNode->pushConnection(this);
	}
}

string Component::canConnectTo( Component* targetNode )
{
	if (this->getID() != targetNode->getID())
	{
		if (!(this->searchConnections(targetNode->getID())))
			return TEXT_CONNECTION_CANCONNECT;
		else
			return TEXT_NODENUMBEGIN + this->getIDString() + TEXT_CONNECTION_ALREADYCONNECTION + targetNode->getIDString() + TEXT_ENDTEXT;
	}
	else
		return TEXT_NODENUMBEGIN + this->getIDString() + TEXT_CONNECTION_ITSELFCONNECTION;
}

bool Component::searchConnections( int searchID )
{
	for( int i = 0; i < _connections.size(); i++)
	{
		//	已經有connect了
		if (_connections[i]->getID() == searchID)
			return true;
	}
	//	還沒connect
	return false;
}

void Component::pushConnection( Component* connectedNode)
{
	_connections.push_back(connectedNode);
}

string Component::getIDString()
{
	stringstream intNum;
	string intToStringNum;

	// int to string
	intNum << this->getID(); // int to stringstream
	intNum >> intToStringNum; // stringstream to string

	return intToStringNum;
}

vector<Component*> Component::getConnections()
{
	return this->_connections;
}

void Component::deleteConnectedComponent( int delComponentID )
{
	for (int i = 0; i < _connections.size(); i++)
	{
		if (_connections[i]->getID() == delComponentID)
			_connections.erase(_connections.begin()+i);
	}
}

