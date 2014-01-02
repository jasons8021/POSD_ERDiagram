#ifndef NODEENTITY_H_
#define NODEENTITY_H_

#define TEXT_NODENUMBEGIN "The node '"
#define TEXT_CONNECTION_NOTCONNECTION "' cannot be connected by the node '"
#define TEXT_ENDTEXT "'."
#define TEXT_CONNECTION_CANCONNECT "CANCONNECT"

#define PARAMETER_ATTRIBUTE "A"
#define PARAMETER_ENTITY "E"
#define PARAMETER_RELATIONSHIP "R"

#define PARAMETER_TRUE 1
#define PARAMETER_FALSE 0

#include <vector>
#include <gtest/gtest_prod.h>
#include "Node.h"
#include "NodeAttribute.h"

using namespace std;

class ComponentVisitor;

class NodeEntity : public Node
{
	friend class NodeEntityTest;
	FRIEND_TEST(NodeEntityTest, connectTo);
	FRIEND_TEST(NodeEntityTest, setPrimaryKey);
	FRIEND_TEST(NodeEntityTest, setForeignKey);
	FRIEND_TEST(NodeEntityTest, deleteKeys);
	FRIEND_TEST(NodeEntityTest, deleteAllRelatedInfo);
public:
	NodeEntity(int, string, int, int);
	virtual ~NodeEntity();
	void setPrimaryKey(int);
	void setForeignKey(int);
	void setIsShowForeignKeyinERTable(bool);
	void deleteKeys(int);
	vector<int> getIsShowForeignKeyinERTable();
	vector<int> getPrimaryKey();
	vector<int> getForeignKey();
	virtual void deleteConnectedComponent(int);
	virtual void deleteAllRelatedInfo();
	virtual void connectTo(Component*);
	virtual string canConnectTo(Component*);
	virtual Component* deepClone();
	virtual void accept(ComponentVisitor*);
private:
	vector<int> _isShowForeignKeyinERTable;
	vector<int> _primaryKeySet;
	vector<int> _foreignKeySet;
};

#endif