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

class ComponentVisitor;

class NodeAttribute : public Node
{
	friend class NodeAttributeTest;
	FRIEND_TEST(NodeAttributeTest, connectTo);
	FRIEND_TEST(NodeAttributeTest, deleteConnectedComponent);
	FRIEND_TEST(NodeAttributeTest, deleteAllRelatedInfo);
public:
	NodeAttribute(int, string, int, int);
	virtual ~NodeAttribute();
	bool getIsPrimaryKey();
	void setIsPrimaryKey(bool);
	bool getIsConnectedEntity();
	void setIsConnectedEntity(bool);
	virtual void deleteConnectedComponent(int);
	virtual void deleteAllRelatedInfo();
	virtual void connectTo(Component*);
	virtual string canConnectTo(Component*);
	virtual Component* deepClone();
	virtual void accept(ComponentVisitor*);
private:
	bool _connectedEntity;
	bool _isPrimaryKey;
};

#endif