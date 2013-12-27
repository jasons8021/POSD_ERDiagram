#include "NodeAttributeTest.h"

void NodeAttributeTest::SetUp()
{
	_attributeComponent = new NodeAttribute(0, "UnitTestNodeAttributeTest", 0, 0);
}

void NodeAttributeTest::TearDown()
{
	delete _attributeComponent;
}

TEST_F(NodeAttributeTest, connectTo){
	_attributeComponent->connectTo(new NodeEntity(1, "UnitTestNodeAttributeTestE1", 0, 0));
	EXPECT_TRUE(_attributeComponent->_connectedEntity);
}
TEST_F(NodeAttributeTest, canConnectTo){
	EXPECT_EQ("CANCONNECT", _attributeComponent->canConnectTo(new Component(1, PARAMETER_ENTITY, "UnitTestNodeE1", 0, 0)));
	_attributeComponent->connectTo(new NodeEntity(1, "UnitTestNodeE1", 0, 0));
	EXPECT_EQ("The node '2' cannot be connected by the node '0'.", _attributeComponent->canConnectTo(new Component(2, PARAMETER_ATTRIBUTE, "UnitTestNodeA2", 0, 0)));
	EXPECT_EQ("The node '0' cannot be connected to itself.", _attributeComponent->canConnectTo(new Component(0, PARAMETER_ATTRIBUTE, "UnitTestNodeAttributeTest", 0, 0)));
	EXPECT_EQ("The node '3' cannot be connected by the node '0'.", _attributeComponent->canConnectTo(new Component(3, PARAMETER_ENTITY, "UnitTestNodeE3", 0, 0)));
}

TEST_F(NodeAttributeTest, deleteConnectedComponent){
	_attributeComponent->connectTo(new NodeEntity(1, "UnitTestNodeE1", 0, 0));
	_attributeComponent->deleteConnectedComponent(1);
	EXPECT_FALSE(_attributeComponent->_connectedEntity);
}

TEST_F(NodeAttributeTest, deepClone){
	Component* cloneComponent = _attributeComponent->deepClone();

	EXPECT_TRUE(_attributeComponent != static_cast<NodeAttribute*>(cloneComponent));

	// 測試是否為deep clone
	cloneComponent->setText("cloneTest");
	EXPECT_EQ("cloneTest", cloneComponent->getText());
	EXPECT_EQ("UnitTestNodeAttributeTest", _attributeComponent->getText());

	delete cloneComponent;
	EXPECT_EQ("UnitTestNodeAttributeTest", _attributeComponent->getText());
}

// Attribute只能連一個Entity，因此測試與deleteConnectedComponent相同
TEST_F(NodeAttributeTest, deleteAllConnected){
	_attributeComponent->connectTo(new NodeEntity(1, "UnitTestNodeE1", 0, 0));
	_attributeComponent->deleteConnectedComponent(1);
	EXPECT_FALSE(_attributeComponent->_connectedEntity);
}