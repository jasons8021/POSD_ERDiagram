#include "NodeEntityTest.h"

void NodeEntityTest::SetUp(){
	_entityComponent = new NodeEntity(0, "UnitTestNodeEntityTest", 0, 0);
}

void NodeEntityTest::TearDown(){
	delete _entityComponent;
}

TEST_F(NodeEntityTest, connectTo){
	_entityComponent->connectTo(new NodeAttribute(1, "UnitTestNodeEntityTestA1", 0, 0));
	EXPECT_EQ(1, _entityComponent->getConnections().size());
}

TEST_F(NodeEntityTest, canConnectTo){
	EXPECT_EQ("CANCONNECT", _entityComponent->canConnectTo(new NodeAttribute(1, "UnitTestNodeA1", 0, 0)));
	_entityComponent->connectTo(new NodeEntity(1, "UnitTestNodeA1", 0, 0));
	EXPECT_EQ("CANCONNECT", _entityComponent->canConnectTo(new NodeRelationship(2, "UnitTestNodeR2", 0, 0)));
	_entityComponent->connectTo(new NodeRelationship(2, "UnitTestNodeR2", 0, 0));

	// 不可連結
	EXPECT_EQ("The node '0' cannot be connected to itself.", _entityComponent->canConnectTo(new Component(0, PARAMETER_ATTRIBUTE, "UnitTestNodeAttributeTest", 0, 0)));
	EXPECT_EQ("The node '3' cannot be connected by the node '0'.", _entityComponent->canConnectTo(new Component(3, PARAMETER_ENTITY, "UnitTestNodeE3", 0, 0)));
}

TEST_F(NodeEntityTest, setPrimaryKey){
	_entityComponent->setPrimaryKey(1);
	EXPECT_EQ(1, _entityComponent->_primaryKeySet.size());
	_entityComponent->setPrimaryKey(1);
	EXPECT_EQ(1, _entityComponent->_primaryKeySet.size());
	_entityComponent->setPrimaryKey(2);
	EXPECT_EQ(2, _entityComponent->_primaryKeySet.size());
}

TEST_F(NodeEntityTest, setForeignKey){
	_entityComponent->setForeignKey(1);
	EXPECT_EQ(1, _entityComponent->_foreignKeySet.size());
	_entityComponent->setForeignKey(1);
	EXPECT_EQ(1, _entityComponent->_foreignKeySet.size());
	_entityComponent->setForeignKey(2);
	EXPECT_EQ(2, _entityComponent->_foreignKeySet.size());
}

TEST_F(NodeEntityTest, deleteConnectedComponent){
	_entityComponent->connectTo(new NodeEntity(1, "UnitTestNodeE1", 0, 0));
	_entityComponent->deleteConnectedComponent(1);
	EXPECT_EQ(0, _entityComponent->getConnections().size());
}

TEST_F(NodeEntityTest, deleteKeys){
	_entityComponent->setPrimaryKey(1);
	EXPECT_EQ(1, _entityComponent->_primaryKeySet.size());
	_entityComponent->setPrimaryKey(2);
	EXPECT_EQ(2, _entityComponent->_primaryKeySet.size());
	_entityComponent->setForeignKey(4);
	EXPECT_EQ(1, _entityComponent->_foreignKeySet.size());

	_entityComponent->deleteKeys(1);
	EXPECT_EQ(1, _entityComponent->_primaryKeySet.size());
	_entityComponent->deleteKeys(4);
	EXPECT_EQ(0, _entityComponent->_foreignKeySet.size());
}

TEST_F(NodeEntityTest, deepClone){
	Component* cloneComponent = _entityComponent->deepClone();

	EXPECT_TRUE(_entityComponent != static_cast<NodeEntity*>(cloneComponent));

	// 測試是否為deep clone
	cloneComponent->setText("cloneTest");
	EXPECT_EQ("cloneTest", cloneComponent->getText());
	EXPECT_EQ("UnitTestNodeEntityTest", _entityComponent->getText());

	delete cloneComponent;
	EXPECT_EQ("UnitTestNodeEntityTest", _entityComponent->getText());
}

// 測試刪除全部的Connect
TEST_F(NodeEntityTest, deleteAllRelatedInfo){
	_entityComponent->connectTo(new NodeEntity(1, "UnitTestNodeE1", 0, 0));
	_entityComponent->connectTo(new NodeEntity(5, "UnitTestNodeE5", 0, 0));

	_entityComponent->setPrimaryKey(1);
	EXPECT_EQ(1, _entityComponent->_primaryKeySet.size());
	_entityComponent->setPrimaryKey(2);
	EXPECT_EQ(2, _entityComponent->_primaryKeySet.size());
	_entityComponent->setForeignKey(4);
	EXPECT_EQ(1, _entityComponent->_foreignKeySet.size());

	_entityComponent->deleteAllRelatedInfo();
	EXPECT_EQ(0, _entityComponent->getConnections().size());
	EXPECT_EQ(0, _entityComponent->_primaryKeySet.size());
	EXPECT_EQ(0, _entityComponent->_foreignKeySet.size());
}