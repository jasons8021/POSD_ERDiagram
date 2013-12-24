#include "NodeRelationshipTest.h"

void NodeRelationshipTest::SetUp()
{
	_relationshipComponent = new NodeRelationship(0, "UnitTestRelationshipComponent", 0, 0);
}

void NodeRelationshipTest::TearDown()
{
	delete _relationshipComponent;
}

TEST_F(NodeRelationshipTest, connectTo){
	_relationshipComponent->connectTo(new NodeEntity(1, "UnitTestNodeEntityTestE1", 0, 0));
	EXPECT_EQ(1, _relationshipComponent->getConnections().size());
}

TEST_F(NodeRelationshipTest, canConnectTo){
	EXPECT_EQ("CANCONNECT", _relationshipComponent->canConnectTo(new Component(1, PARAMETER_ENTITY, "UnitTestNodeE1", 0, 0)));
	_relationshipComponent->connectTo(new NodeEntity(1, "UnitTestNodeE1", 0, 0));
	EXPECT_EQ("CANCONNECT", _relationshipComponent->canConnectTo(new Component(2, PARAMETER_ENTITY, "UnitTestNodeE2", 0, 0)));
	_relationshipComponent->connectTo(new NodeRelationship(2, "UnitTestNodeE2", 0, 0));

	// 不可連結
	EXPECT_EQ("The node '0' cannot be connected to itself.", _relationshipComponent->canConnectTo(new Component(0, PARAMETER_RELATIONSHIP, "UnitTestRelationshipComponent", 0, 0)));
	EXPECT_EQ("The node '3' cannot be connected by the node '0'.", _relationshipComponent->canConnectTo(new Component(3, PARAMETER_RELATIONSHIP, "UnitTestNodeR3", 0, 0)));
}

TEST_F(NodeRelationshipTest, setEntityCardinality){
	_relationshipComponent->setEntityCardinality(make_pair(1,"1"));
	EXPECT_EQ(1, _relationshipComponent->_entityCardinalitySet.size());
	_relationshipComponent->setEntityCardinality(make_pair(5,"1"));
	EXPECT_EQ(2, _relationshipComponent->_entityCardinalitySet.size());
}

TEST_F(NodeRelationshipTest, deleteConnectedComponent){
	_relationshipComponent->connectTo(new NodeEntity(1, "UnitTestNodeE1", 0, 0));
	_relationshipComponent->connectTo(new NodeEntity(5, "UnitTestNodeE5", 0, 0));

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
	_relationshipComponent->connectTo(new NodeEntity(1, "UnitTestNodeE1", 0, 0));
	_relationshipComponent->connectTo(new NodeEntity(5, "UnitTestNodeE5", 0, 0));

	_relationshipComponent->setEntityCardinality(make_pair(1,"1"));
	EXPECT_EQ(1, _relationshipComponent->_entityCardinalitySet.size());
	_relationshipComponent->setEntityCardinality(make_pair(5,"1"));
	EXPECT_EQ(2, _relationshipComponent->_entityCardinalitySet.size());

	_relationshipComponent->deleteEntityCardinality(5);
	EXPECT_EQ(1, _relationshipComponent->_entityCardinalitySet.size());
}

TEST_F(NodeRelationshipTest, deepClone){
	Component* cloneComponent = _relationshipComponent->deepClone();

	EXPECT_TRUE(_relationshipComponent != static_cast<NodeRelationship*>(cloneComponent));

	// 測試是否為deep clone
	cloneComponent->setText("cloneTest");
	EXPECT_EQ("cloneTest", cloneComponent->getText());
	EXPECT_EQ("UnitTestRelationshipComponent", _relationshipComponent->getText());

	delete cloneComponent;
	EXPECT_EQ("UnitTestRelationshipComponent", _relationshipComponent->getText());
}