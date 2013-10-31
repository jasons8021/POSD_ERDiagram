#include "ConnectComponentsCmdTest.h"

void ConnectComponentsCmdTest::SetUp(){
	_erModel = new ERModel();

	_erModel->setComponentID(0);
	_erModel->setIsModify(false);

	const int testNum = 10;
	const string testType[testNum] = {"A", "E", "R", "A", "A", "A", "E", "R", "E", "E"};
	const string testText[testNum] = {"A0", "E1", "R2", "A3", "A4", "A5", "E6", "R7", "E8", "E9"};

	// 新增component進vector中
	for (int i = 0; i < testNum; i++)
	{
		if (testType[i] == PARAMETER_ATTRIBUTE)
			_erModel->_components.push_back(new NodeAttribute(i, testText[i]));
		else if (testType[i] == PARAMETER_ENTITY)
			_erModel->_components.push_back(new NodeEntity(i, testText[i]));
		else
			_erModel->_components.push_back(new NodeRelationship(i, testText[i]));
		_erModel->_componentID++;
	}

	_connectComponentsCmd = new ConnectComponentsCmd(_erModel, 0, 1, "UnitTest ConnectComponentsCmd");
	_connectComponentsRedoCmd = new ConnectComponentsCmd(_erModel, 1, 3, "UnitTest ConnectComponentsRedoCmd");
}

void ConnectComponentsCmdTest::TearDown(){
	delete _erModel;
	delete _connectComponentsCmd;
}

TEST_F(ConnectComponentsCmdTest, execute){
	_connectComponentsCmd->execute();
	EXPECT_EQ(10, _erModel->_connections[0]->getID());

	_connectComponentsRedoCmd->_componentID = 11;
	_connectComponentsRedoCmd->execute();
	EXPECT_EQ(11, _erModel->_connections[1]->getID());
}
