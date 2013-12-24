#include "ConnectorTest.h"

void ConnectorTest::SetUp()
{
	_connectorComponent = new Connector(2, "UnitTestNodeConnectorTest");
}

void ConnectorTest::TearDown()
{
	delete _connectorComponent;
}

TEST_F(ConnectorTest, setConnectedNode){
	_connectorComponent->setConnectedNode(new NodeAttribute(0, "UnitTestNodeA1", 0, 0), new NodeEntity(1, "UnitTestNodeE3", 0, 0));
	EXPECT_EQ(0, _connectorComponent->getSourceNodeID());
	EXPECT_EQ(1, _connectorComponent->getDestinationNodeID());
}

TEST_F(ConnectorTest, deepClone){
	Component* cloneComponent = _connectorComponent->deepClone();

	EXPECT_TRUE(_connectorComponent != static_cast<Connector*>(cloneComponent));

	// ´ú¸Õ¬O§_¬°deep clone
	cloneComponent->setText("cloneTest");
	EXPECT_EQ("cloneTest", cloneComponent->getText());
	EXPECT_EQ("UnitTestNodeConnectorTest", _connectorComponent->getText());

	delete cloneComponent;
	EXPECT_EQ("UnitTestNodeConnectorTest", _connectorComponent->getText());
}