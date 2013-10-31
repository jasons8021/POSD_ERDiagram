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
	_connectorComponent->setConnectedNode(new NodeAttribute(0, "UnitTestNodeA1"), new NodeEntity(1, "UnitTestNodeE3"));
	EXPECT_EQ(0, _connectorComponent->getSourceNodeID());
	EXPECT_EQ(1, _connectorComponent->getDestinationNodeID());
}
