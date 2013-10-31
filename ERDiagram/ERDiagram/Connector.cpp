#include "Connector.h"

Connector::Connector( int id, string text ) : Component( id, PARAMETER_CONNECTOR, text )
{

}

Connector::~Connector()
{
}

void Connector::connectTo( Component* )
{
}

string Connector::canConnectTo( Component* )
{
	return TEXT_ERRORCONNECT;
}

void Connector::setSourceNode( Component* sourceNode )
{
	this->_sourceNode = sourceNode;
}

void Connector::setDestinationNode( Component* destinationNode )
{
	this->_destinationNode = destinationNode;
}

Component* Connector::getSourceNode()
{
	return this->_sourceNode;
}

Component* Connector::getDestinationNode()
{
	return this->_destinationNode;
}


void Connector::setConnectedNode( Component* sourceNode, Component* destinationNode )
{
	setSourceNode(sourceNode);
	setDestinationNode(destinationNode);
}

int Connector::getSourceNodeID()
{
	return (this->_sourceNode)->getID();
}

int Connector::getDestinationNodeID()
{
	return (this->_destinationNode)->getID();
}

std::string Connector::getSourceNodeType()
{
	return (this->_sourceNode)->getType();
}

std::string Connector::getDestinationNodeType()
{
	return (this->_destinationNode)->getType();
}

std::string Connector::getSourceNodeText()
{
	return (this->_sourceNode)->getText();
}

std::string Connector::getDestinationNodeText()
{
	return (this->_destinationNode)->getText();
}
