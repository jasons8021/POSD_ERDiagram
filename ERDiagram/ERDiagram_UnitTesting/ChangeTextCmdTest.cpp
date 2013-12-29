#include "ChangeTextCmdTest.h"

void ChangeTextCmdTest::SetUp()
{
	_erModel = new ERModel();
	_erModel->_components.push_back(new NodeAttribute(0, "A0", 0, 0));
	_changeTextCmd = new ChangeTextCmd(_erModel, 0, "Unit Test");
}

void ChangeTextCmdTest::TearDown()
{
	delete _erModel;
	delete _changeTextCmd;
}

TEST_F(ChangeTextCmdTest, execute)
{
	EXPECT_EQ("A0", _erModel->_components[0]->getText());
	_changeTextCmd->execute();
	EXPECT_EQ("Unit Test", _erModel->_components[0]->getText());
}

TEST_F(ChangeTextCmdTest, unexecute)
{
	EXPECT_EQ("A0", _erModel->_components[0]->getText());
	_changeTextCmd->execute();
	EXPECT_EQ("Unit Test", _erModel->_components[0]->getText());

	_changeTextCmd->unexecute();
	EXPECT_EQ("A0", _erModel->_components[0]->getText());
}
