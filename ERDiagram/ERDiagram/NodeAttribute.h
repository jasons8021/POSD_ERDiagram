#ifndef NODEATTRIBUTE_H_
#define NODEATTRIBUTE_H_

#define TEXT_NODENUMBEGIN "The node '"
#define TEXT_CONNECTION_NOTCONNECTION "' cannot be connected by the node '"
#define TEXT_CONNECTION_ITSELFCONNECTION "' cannot be connected to itself."
#define TEXT_CONNECTION_ALREADYCONNECTION "' has already been connected to component '"
#define TEXT_ENDTEXT "'."
#define TEXT_CONNECTION_CANCONNECT "CANCONNECT"

#define PARAMETER_ATTRIBUTE "A"
#define PARAMETER_ENTITY "E"
#define PARAMETER_RELATIONSHIP "R"

#include <vector>
#include "Node.h"
#include "gtest/gtest_prod.h"

using namespace std;

class NodeAttribute : public Node
{
	friend class NodeAttributeTest;
	FRIEND_TEST(NodeAttributeTest, connectTo);
	FRIEND_TEST(NodeAttributeTest, deleteConnectedComponent);
public:
	NodeAttribute(int, string);
	virtual ~NodeAttribute();
	bool getIsPrimaryKey();
	void setIsPrimaryKey(bool);
	virtual void deleteConnectedComponent(int);
	virtual void connectTo(Component*);
	virtual string canConnectTo(Component*);
private:
	bool _connectedEntity;
	bool _isPrimaryKey;
};

#endif