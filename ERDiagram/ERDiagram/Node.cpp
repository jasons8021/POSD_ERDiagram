#include "Node.h"

Node::Node() : Component()
{
}

Node::Node( int id, string type, string text, int sx, int sy ) : Component( id, type, text, sx, sy )
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
