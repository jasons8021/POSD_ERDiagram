#include "Node.h"

Node::Node() : Component()
{
}

Node::Node( int id, string type, string text ) : Component( id, type, text )
{
}

Node::~Node()
{
}

void Node::connectTo( Component* TargetNode )
{

}

string Node::canConnectTo( Component* TargetNode )
{
	// The method is implemeneted by derived class.
	return PARAMETER_NULL;
}

void Node::deleteConnectedComponent( int )
{

}
