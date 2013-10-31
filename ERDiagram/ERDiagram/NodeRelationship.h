#ifndef NODERELATIONAL_H_
#define NODERELATIONAL_H_

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
#include <gtest/gtest_prod.h>
#include "Node.h"
#include "Toolkit.h"

using namespace std;

class NodeRelationship : public Node
{
	friend class NodeRelationshipTest;
	FRIEND_TEST(NodeRelationshipTest, connectTo);
	FRIEND_TEST(NodeRelationshipTest, setEntityCardinality);
	FRIEND_TEST(NodeRelationshipTest, deleteConnectedComponent);
	FRIEND_TEST(NodeRelationshipTest, deleteEntityCardinality);
public:
	NodeRelationship(int, string);
	virtual ~NodeRelationship();
	void setEntityCardinality(pair<int, string>);
	void deleteEntityCardinality(int);
	vector<pair<int, string>> getEntityCardinality();
	virtual void deleteConnectedComponent(int);
	virtual void connectTo(Component*);
	virtual string canConnectTo(Component*);
private:
	vector<pair<int, string>> _entityCardinalitySet;
};

#endif