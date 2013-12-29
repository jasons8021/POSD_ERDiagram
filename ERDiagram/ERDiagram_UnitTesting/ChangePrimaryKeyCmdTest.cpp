#include "ChangePrimaryKeyCmdTest.h"

void ChangePrimaryKeyCmdTest::SetUp(){
	_erModel = new ERModel();
	_erModel->_components.push_back(new NodeAttribute(0, "A0", 0, 0));
	_changePrimaryKeyCmd = new ChangePrimaryKeyCmd(_erModel, 0, true);
}

void ChangePrimaryKeyCmdTest::TearDown(){
	delete _erModel;
	delete _changePrimaryKeyCmd;
}

TEST_F(ChangePrimaryKeyCmdTest, execute){
	EXPECT_FALSE(static_cast<NodeAttribute*>(_erModel->_components[0])->getIsPrimaryKey());
	_changePrimaryKeyCmd->execute();
	EXPECT_TRUE(static_cast<NodeAttribute*>(_erModel->_components[0])->getIsPrimaryKey());
}

TEST_F(ChangePrimaryKeyCmdTest, unexecute){

	EXPECT_FALSE(static_cast<NodeAttribute*>(_erModel->_components[0])->getIsPrimaryKey());
	_changePrimaryKeyCmd->execute();
	EXPECT_TRUE(static_cast<NodeAttribute*>(_erModel->_components[0])->getIsPrimaryKey());

	_changePrimaryKeyCmd->unexecute();
	EXPECT_FALSE(static_cast<NodeAttribute*>(_erModel->_components[0])->getIsPrimaryKey());
}
