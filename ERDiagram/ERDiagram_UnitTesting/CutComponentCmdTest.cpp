#include "CutComponentCmdTest.h"

void CutComponentCmdTest::SetUp()
{
	_erModel = new ERModel();

	_erModel->_components.push_back(new NodeAttribute(0, "A0", 0, 0));
	_erModel->_componentID++;
	_erModel->_components.push_back(new NodeEntity(1, "E1", 0, 0));
	_erModel->_componentID++;
	_erModel->addConnection(2, 0, 1, PARAMETER_NULL);
	_erModel->_componentID++;

	vector<int> testIDSet;
	testIDSet.push_back(0);
	testIDSet.push_back(1);
	testIDSet.push_back(2);

	_cutComponentCmd = new CutComponentCmd(_erModel, testIDSet);
}

void CutComponentCmdTest::TearDown()
{
	delete _erModel;
	delete _cutComponentCmd;
}

// 剪下後會先複製Item到剪貼簿再刪除
TEST_F(CutComponentCmdTest, execute)
{
	EXPECT_EQ(3, _erModel->_components.size());
	_cutComponentCmd->execute();
	EXPECT_EQ(0, _erModel->_components.size());
	EXPECT_EQ(3, _erModel->_clipboard.size());
	EXPECT_EQ(0, _erModel->_clipboard[0]->getID());
	EXPECT_EQ(1, _erModel->_clipboard[1]->getID());
	EXPECT_EQ(2, _erModel->_clipboard[2]->getID());
}

// 復原刪除，剪貼簿不動
TEST_F(CutComponentCmdTest, unexecute)
{
	EXPECT_EQ(3, _erModel->_components.size());
	_cutComponentCmd->execute();
	EXPECT_EQ(0, _erModel->_components.size());

	EXPECT_EQ(3, _erModel->_clipboard.size());
	EXPECT_EQ(0, _erModel->_clipboard[0]->getID());
	EXPECT_EQ(1, _erModel->_clipboard[1]->getID());
	EXPECT_EQ(2, _erModel->_clipboard[2]->getID());

	_cutComponentCmd->unexecute();
	EXPECT_EQ(3, _erModel->_components.size());
	
	EXPECT_EQ(3, _erModel->_clipboard.size());
	EXPECT_EQ(0, _erModel->_clipboard[0]->getID());
	EXPECT_EQ(1, _erModel->_clipboard[1]->getID());
	EXPECT_EQ(2, _erModel->_clipboard[2]->getID());
}
