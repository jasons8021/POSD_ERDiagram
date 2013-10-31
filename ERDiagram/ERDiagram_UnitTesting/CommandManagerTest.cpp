#include "CommandManagerTest.h"

void CommandManagerTest::SetUp()
{
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

	_cmdManager = new CommandManager();
}

void CommandManagerTest::TearDown(){
	delete _erModel;
	delete _cmdManager;
}

TEST_F(CommandManagerTest, execute){
	_cmdManager->execute(new AddComponentCmd(_erModel, PARAMETER_ATTRIBUTE, "UnitTest AddComponentCmd"));
	EXPECT_EQ("UnitTest AddComponentCmd",_erModel->_components[10]->getText());
	_cmdManager->execute(new ConnectComponentsCmd(_erModel, 1, 3, "UnitTest ConnectComponentsCmd"));
	EXPECT_EQ(11,_erModel->_components[11]->getID());
	_cmdManager->execute(new DeleteComponentCmd(_erModel, 3));
	EXPECT_EQ(10,_erModel->_components.size());
}

TEST_F(CommandManagerTest, redo){
	EXPECT_FALSE(_cmdManager->redo());
	_cmdManager->execute(new AddComponentCmd(_erModel, PARAMETER_ATTRIBUTE, "UnitTest AddComponentCmd"));
	EXPECT_TRUE(_cmdManager->undo());
	EXPECT_TRUE(_cmdManager->redo());
}

TEST_F(CommandManagerTest, undo){
	EXPECT_FALSE(_cmdManager->undo());
	_cmdManager->execute(new AddComponentCmd(_erModel, PARAMETER_ATTRIBUTE, "UnitTest AddComponentCmd"));
	EXPECT_TRUE(_cmdManager->undo());
}

