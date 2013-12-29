#include "MoveComponentCmdTest.h"

void MoveComponentCmdTest::SetUp()
{
	_erModel = new ERModel();

	_erModel->_components.push_back(new NodeAttribute(0, "A0", 0, 0));
	_erModel->_componentID++;
	_erModel->_components.push_back(new NodeEntity(1, "E1", 10, 10));
	_erModel->_componentID++;

	vector<int> testIDSet;
	testIDSet.push_back(0);
	testIDSet.push_back(1);

	_moveComponentCmd = new MoveComponentCmd(_erModel, testIDSet, 50, 50);
}

void MoveComponentCmdTest::TearDown()
{
	delete _erModel;
	delete _moveComponentCmd;
}

// 剪下後會先複製Item到剪貼簿再刪除
TEST_F(MoveComponentCmdTest, execute)
{
	EXPECT_EQ(0, _erModel->_components[0]->getSx());
	EXPECT_EQ(0, _erModel->_components[0]->getSy());
	EXPECT_EQ(10, _erModel->_components[1]->getSx());
	EXPECT_EQ(10, _erModel->_components[1]->getSy());

	_moveComponentCmd->execute();
	EXPECT_EQ(50, _erModel->_components[0]->getSx());
	EXPECT_EQ(50, _erModel->_components[0]->getSy());
	EXPECT_EQ(60, _erModel->_components[1]->getSx());
	EXPECT_EQ(60, _erModel->_components[1]->getSy());

	_moveComponentCmd->_isRedo = true;

	_moveComponentCmd->execute();
	EXPECT_EQ(100, _erModel->_components[0]->getSx());
	EXPECT_EQ(100, _erModel->_components[0]->getSy());
	EXPECT_EQ(110, _erModel->_components[1]->getSx());
	EXPECT_EQ(110, _erModel->_components[1]->getSy());
}

// 復原刪除，剪貼簿不動
TEST_F(MoveComponentCmdTest, unexecute)
{
	EXPECT_EQ(0, _erModel->_components[0]->getSx());
	EXPECT_EQ(0, _erModel->_components[0]->getSy());
	EXPECT_EQ(10, _erModel->_components[1]->getSx());
	EXPECT_EQ(10, _erModel->_components[1]->getSy());

	_moveComponentCmd->unexecute();
	EXPECT_EQ(-50, _erModel->_components[0]->getSx());
	EXPECT_EQ(-50, _erModel->_components[0]->getSy());
	EXPECT_EQ(-40, _erModel->_components[1]->getSx());
	EXPECT_EQ(-40, _erModel->_components[1]->getSy());
}
