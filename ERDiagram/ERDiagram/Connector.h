#ifndef CONNECTOR_H_
#define CONNECTOR_H_

#define TEXT_ERRORCONNECT "Connector can't connect to each other."

#define PARAMETER_CONNECTOR "C"
#define PARAMETER_NULL ""

#include <vector>
#include <gtest/gtest_prod.h>
#include "Component.h"

class Connector : public Component
{
	friend class ConnectorTest;
public:
	Connector(int, string);
	~Connector();
	int getSourceNodeID();
	int getDestinationNodeID();
	void setConnectedNode(Component*,Component*);
	void setSourceNode(Component*);
	void setDestinationNode(Component*);
	string getSourceNodeType();
	string getDestinationNodeType();
	string getSourceNodeText();
	string getDestinationNodeText();
	Component* getSourceNode();
	Component* getDestinationNode();
	virtual void connectTo(Component*);
	virtual string canConnectTo(Component*);
private:
	Component* _sourceNode;
	Component* _destinationNode;
};

#endif