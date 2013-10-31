#include "NodeAttributeTest.h"

void NodeAttributeTest::SetUp()
{
	_attributeComponent = new NodeAttribute(0, "UnitTestNodeAttributeTest");
}

void NodeAttributeTest::TearDown()
{
	delete _attributeComponent;
}

TEST_F(NodeAttributeTest, connectTo){
	_attributeComponent->connectTo(new NodeEntity(1, "UnitTestNodeAttributeTestE1"));
	EXPECT_TRUE(_attributeComponent->_connectedEntity);
}
TEST_F(NodeAttributeTest, canConnectTo){
	EXPECT_EQ("CANCONNECT", _attributeComponent->canConnectTo(new Component(1, PARAMETER_ENTITY, "UnitTestNodeE1")));
	_attributeComponent->connectTo(new NodeEntity(1, "UnitTestNodeE1"));
	EXPECT_EQ("The node '2' cannot be connected by the node '0'.", _attributeComponent->canConnectTo(new Component(2, PARAMETER_ATTRIBUTE, "UnitTestNodeA2")));
	EXPECT_EQ("The node '0' cannot be connected to itself.", _attributeComponent->canConnectTo(new Component(0, PARAMETER_ATTRIBUTE, "UnitTestNodeAttributeTest")));
	EXPECT_EQ("The node '3' cannot be connected by the node '0'.", _attributeComponent->canConnectTo(new Component(3, PARAMETER_ENTITY, "UnitTestNodeE3")));
}

TEST_F(NodeAttributeTest, deleteConnectedComponent){
	_attributeComponent->connectTo(new NodeEntity(1, "UnitTestNodeE1"));
	_attributeComponent->deleteConnectedComponent(1);
	EXPECT_FALSE(_attributeComponent->_connectedEntity);
}