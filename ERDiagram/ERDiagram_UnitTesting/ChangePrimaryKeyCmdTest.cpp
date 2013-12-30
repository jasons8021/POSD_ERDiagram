#include "ChangePrimaryKeyCmdTest.h"

void ChangePrimaryKeyCmdTest::SetUp()
{
	_erModel = new ERModel();
	_erModel->_components.push_back(new NodeAttribute(0, "A0", 0, 0));
	_erModel->_componentID++;
	_erModel->_components.push_back(new NodeEntity(1, "E1", 0, 0));
	_erModel->_componentID++;
	_changePrimaryKeyCmd = new ChangePrimaryKeyCmd(_erModel, 0, true);
}

void ChangePrimaryKeyCmdTest::TearDown()
{
	delete _erModel;
	delete _changePrimaryKeyCmd;
}

TEST_F(ChangePrimaryKeyCmdTest, execute)
{
	_changePrimaryKeyCmd->execute();
	EXPECT_FALSE(static_cast<NodeAttribute*>(_erModel->_components[0])->getIsPrimaryKey());

	_erModel->addConnection(10, 0, 1, "");
	_changePrimaryKeyCmd->execute();
	EXPECT_TRUE(static_cast<NodeAttribute*>(_erModel->_components[0])->getIsPrimaryKey());
}

TEST_F(ChangePrimaryKeyCmdTest, unexecute)
{
	_erModel->addConnection(10, 0, 1, "");
	_changePrimaryKeyCmd->execute();
 	EXPECT_TRUE(static_cast<NodeAttribute*>(_erModel->_components[0])->getIsPrimaryKey());
	_changePrimaryKeyCmd->unexecute();
	EXPECT_FALSE(static_cast<NodeAttribute*>(_erModel->_components[0])->getIsPrimaryKey());
}
