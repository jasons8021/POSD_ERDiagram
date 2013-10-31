#include "NodeRelationshipTest.h"

void NodeRelationshipTest::SetUp()
{
	_relationshipComponent = new NodeRelationship(0, "UnitTestRelationshipComponent");
}

void NodeRelationshipTest::TearDown()
{
	delete _relationshipComponent;
}

TEST_F(NodeRelationshipTest, connectTo){
	_relationshipComponent->connectTo(new NodeEntity(1, "UnitTestNodeEntityTestE1"));
	EXPECT_EQ(1, _relationshipComponent->getConnections().size());
}

TEST_F(NodeRelationshipTest, canConnectTo){
	EXPECT_EQ("CANCONNECT", _relationshipComponent->canConnectTo(new Component(1, PARAMETER_ENTITY, "UnitTestNodeE1")));
	_relationshipComponent->connectTo(new NodeEntity(1, "UnitTestNodeE1"));
	EXPECT_EQ("CANCONNECT", _relationshipComponent->canConnectTo(new Component(2, PARAMETER_ENTITY, "UnitTestNodeE2")));
	_relationshipComponent->connectTo(new NodeRelationship(2, "UnitTestNodeE2"));

	// 不可連結
	EXPECT_EQ("The node '0' cannot be connected to itself.", _relationshipComponent->canConnectTo(new Component(0, PARAMETER_RELATIONSHIP, "UnitTestRelationshipComponent")));
	EXPECT_EQ("The node '3' cannot be connected by the node '0'.", _relationshipComponent->canConnectTo(new Component(3, PARAMETER_RELATIONSHIP, "UnitTestNodeR3")));
}

TEST_F(NodeRelationshipTest, setEntityCardinality){
	_relationshipComponent->setEntityCardinality(make_pair(1,"1"));
	EXPECT_EQ(1, _relationshipComponent->_entityCardinalitySet.size());
	_relationshipComponent->setEntityCardinality(make_pair(5,"1"));
	EXPECT_EQ(2, _relationshipComponent->_entityCardinalitySet.size());
}

TEST_F(NodeRelationshipTest, deleteConnectedComponent){
	_relationshipComponent->connectTo(new NodeEntity(1, "UnitTestNodeE1"));
	_relationshipComponent->connectTo(new NodeEntity(5, "UnitTestNodeE5"));

	_relationshipComponent->setEntityCardinality(make_pair(1,"1"));
	EXPECT_EQ(1, _relationshipComponent->_entityCardinalitySet.size());
	_relationshipComponent->setEntityCardinality(make_pair(5,"1"));
	EXPECT_EQ(2, _relationshipComponent->_entityCardinalitySet.size());

	_relationshipComponent->deleteConnectedComponent(5);
	EXPECT_EQ(1, _relationshipComponent->getConnections().size());
	EXPECT_EQ(1, _relationshipComponent->_entityCardinalitySet.size());

	_relationshipComponent->deleteConnectedComponent(1);
	EXPECT_EQ(0, _relationshipComponent->getConnections().size());
	EXPECT_EQ(0, _relationshipComponent->_entityCardinalitySet.size());
}

TEST_F(NodeRelationshipTest, deleteEntityCardinality){
	_relationshipComponent->connectTo(new NodeEntity(1, "UnitTestNodeE1"));
	_relationshipComponent->connectTo(new NodeEntity(5, "UnitTestNodeE5"));

	_relationshipComponent->setEntityCardinality(make_pair(1,"1"));
	EXPECT_EQ(1, _relationshipComponent->_entityCardinalitySet.size());
	_relationshipComponent->setEntityCardinality(make_pair(5,"1"));
	EXPECT_EQ(2, _relationshipComponent->_entityCardinalitySet.size());

	_relationshipComponent->deleteEntityCardinality(5);
	EXPECT_EQ(1, _relationshipComponent->_entityCardinalitySet.size());
}