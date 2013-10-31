#include "ComponentTest.h"

void ComponentTest::SetUp(){
	_component = new Component(0, PARAMETER_ENTITY, "UnitTestComponentTest");
	_component->_connections.push_back(new Component(1, PARAMETER_ATTRIBUTE, "UnitTestComponentTestA1"));
}

void ComponentTest::TearDown(){
	delete _component;
}

TEST_F(ComponentTest, searchConnections){
	EXPECT_TRUE(_component->searchConnections(1));
	EXPECT_FALSE(_component->searchConnections(2));
}

TEST_F(ComponentTest, pushConnection){
	_component->pushConnection(new Component(2, PARAMETER_ATTRIBUTE, "UnitTestComponentTestA2"));
	EXPECT_EQ(2, _component->getConnections()[1]->getID());
}

TEST_F(ComponentTest, getIDString){
	EXPECT_EQ("0", _component->getIDString());
}

TEST_F(ComponentTest, connectTo){
	_component->connectTo(new Component(2, PARAMETER_ATTRIBUTE, "UnitTestComponentTestA2"));
	EXPECT_EQ(2, _component->getConnections()[1]->getID());

	_component->connectTo(new Component(1, PARAMETER_ATTRIBUTE, "UnitTestComponentTestA2"));
	EXPECT_EQ(2, _component->getConnections().size());
}

TEST_F(ComponentTest, canConnectTo){
	EXPECT_EQ("CANCONNECT", _component->canConnectTo(new Component(2, PARAMETER_ATTRIBUTE, "UnitTestComponentTestA2")));
	EXPECT_EQ("The node '0' has already been connected to component '1'.", _component->canConnectTo(new Component(1, PARAMETER_ATTRIBUTE, "UnitTestComponentTestA2")));
	EXPECT_EQ("The node '0' cannot be connected to itself.", _component->canConnectTo(new Component(0, PARAMETER_ENTITY, "UnitTestComponentTest")));
}
