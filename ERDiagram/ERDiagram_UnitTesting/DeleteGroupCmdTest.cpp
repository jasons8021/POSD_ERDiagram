#include "DeleteGroupCmdTest.h"

void DeleteGroupCmdTest::SetUp()
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

	_deleteGroupCmd = new DeleteGroupCmd(_erModel, testIDSet);
}

void DeleteGroupCmdTest::TearDown()
{
	delete _erModel;
	delete _deleteGroupCmd;
}

// �ŤU��|���ƻsItem��ŶKï�A�R��
TEST_F(DeleteGroupCmdTest, execute)
{
	EXPECT_EQ(3, _erModel->_components.size());
	_deleteGroupCmd->execute();
	EXPECT_EQ(0, _erModel->_components.size());
}

// �_��R���A�ŶKï����
TEST_F(DeleteGroupCmdTest, unexecute)
{
	EXPECT_EQ(3, _erModel->_components.size());
	_deleteGroupCmd->execute();
	EXPECT_EQ(0, _erModel->_components.size());
	_deleteGroupCmd->unexecute();
	EXPECT_EQ(3, _erModel->_components.size());
}
