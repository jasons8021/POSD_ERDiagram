#include "ComponentTest.h"

void ComponentTest::SetUp(){
	_component = new Component(0, PARAMETER_ENTITY, "UnitTestComponentTest", 0, 0);
	_component->_connections.push_back(new Component(1, PARAMETER_ATTRIBUTE, "UnitTestComponentTestA1", 0, 0));
}

void ComponentTest::TearDown(){
	delete _component;
}
// �j�MConnection
TEST_F(ComponentTest, searchConnections){
	EXPECT_TRUE(_component->searchConnections(1));
	EXPECT_FALSE(_component->searchConnections(2));
}

// �[�JConnection
TEST_F(ComponentTest, pushConnection){
	_component->pushConnection(new Component(2, PARAMETER_ATTRIBUTE, "UnitTestComponentTestA2", 0, 0));
	EXPECT_EQ(2, _component->getConnections()[1]->getID());
}

// ���oID(String���A)
TEST_F(ComponentTest, getIDString){
	EXPECT_EQ("0", _component->getIDString());
}

// �s�V����Component
TEST_F(ComponentTest, connectTo){
	_component->connectTo(new Component(2, PARAMETER_ATTRIBUTE, "UnitTestComponentTestA2", 0, 0));
	EXPECT_EQ(2, _component->getConnections()[1]->getID());

	_component->connectTo(new Component(1, PARAMETER_ATTRIBUTE, "UnitTestComponentTestA2", 0, 0));
	EXPECT_EQ(2, _component->getConnections().size());
}

// �i�_�s��target Component
TEST_F(ComponentTest, canConnectTo){
	EXPECT_EQ("CANCONNECT", _component->canConnectTo(new Component(2, PARAMETER_ATTRIBUTE, "UnitTestComponentTestA2", 0, 0)));
	EXPECT_EQ("The node '0' has already been connected to component '1'.", _component->canConnectTo(new Component(1, PARAMETER_ATTRIBUTE, "UnitTestComponentTestA2", 0, 0)));
	EXPECT_EQ("The node '0' cannot be connected to itself.", _component->canConnectTo(new Component(0, PARAMETER_ENTITY, "UnitTestComponentTest", 0, 0)));
}

// �R���������s��
TEST_F(ComponentTest, deleteAllRelatedInfo){
	_component->connectTo(new NodeEntity(1, "UnitTestNodeE1", 0, 0));
	_component->connectTo(new NodeEntity(5, "UnitTestNodeE5", 0, 0));

	_component->deleteAllRelatedInfo();
	EXPECT_EQ(0, _component->getConnections().size());
}
