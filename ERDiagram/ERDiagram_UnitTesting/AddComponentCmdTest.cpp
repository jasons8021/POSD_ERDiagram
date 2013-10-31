#include "AddComponentCmdTest.h"

void AddComponentCmdTest::SetUp(){
	_erModel = new ERModel();
	_addComponentCmd = new AddComponentCmd(_erModel, PARAMETER_ATTRIBUTE, "UnitTest AddComponentCmd");
}

void AddComponentCmdTest::TearDown(){
	delete _erModel;
	delete _addComponentCmd;
}

TEST_F(AddComponentCmdTest, execute){
	_addComponentCmd->execute();
	EXPECT_EQ("UnitTest AddComponentCmd", _erModel->_components[0]->getText());

	_erModel->_components.pop_back();

	_addComponentCmd->_componentID = PARAMETER_NOVALUE;
	_addComponentCmd->execute();
	EXPECT_EQ("UnitTest AddComponentCmd", _erModel->_components[0]->getText());
}

TEST_F(AddComponentCmdTest, unexecute){
	_addComponentCmd->execute();
	_addComponentCmd->unexecute();
	EXPECT_EQ(0, _erModel->_components.size());
}
