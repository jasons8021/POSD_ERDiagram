#include "DeleteComponentCmdTest.h"

void DeleteComponentCmdTest::SetUp(){
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

	_erModel->addConnection(10, 0, 1, PARAMETER_NULL);
	_erModel->addConnection(11, 1, 3, PARAMETER_NULL);
	_erModel->addConnection(12, 1, 4, PARAMETER_NULL);
	_erModel->addConnection(13, 5, 8, PARAMETER_NULL);

	_deleteAttributeCmd = new DeleteComponentCmd(_erModel, 3);
	_deleteEntityCmd = new DeleteComponentCmd(_erModel, 1);
	_deleteConnectorCmd = new DeleteComponentCmd(_erModel, 12);
}

void DeleteComponentCmdTest::TearDown(){
	delete _erModel;
	delete _deleteAttributeCmd;
	delete _deleteEntityCmd;
	delete _deleteConnectorCmd;
}

TEST_F(DeleteComponentCmdTest, execute){
	bool checkDeleteResult = false;

	_deleteConnectorCmd->execute();
	// 檢查Component中的12是否移除
	checkDeleteResult = false;
	for (int i = 0; i < _erModel->_components.size(); i++)
	{
		if (_erModel->_components[i]->getID() == 12)
		{
			checkDeleteResult = false;
			break;
		}
		else
			checkDeleteResult = true;
	}
	EXPECT_TRUE(checkDeleteResult);
	checkDeleteResult = false;

	_deleteAttributeCmd->execute();
	// 檢查與刪除掉的component的連接關係是否被移除
	for (int i = 0; i < _erModel->_components[1]->getConnections().size(); i++)
	{
		if (_erModel->_components[1]->getConnections()[i]->getID() == 3)
		{
			checkDeleteResult = false;
			break;
		}
		else
			checkDeleteResult = true;
	}
	EXPECT_TRUE(checkDeleteResult);
	checkDeleteResult = false;

	// 檢查與A3有關係的Connector是否移除
	checkDeleteResult = false;
	for (int i = 0; i < _erModel->_connections.size(); i++)
	{
		if (_erModel->_connections[i]->getID() == 11)
		{
			checkDeleteResult = false;
			break;
		}
		else
			checkDeleteResult = true;
	}
	EXPECT_TRUE(checkDeleteResult);
	checkDeleteResult = false;

	// 檢查Component中的3是否移除
	checkDeleteResult = false;
	for (int i = 0; i < _erModel->_components.size(); i++)
	{
		if (_erModel->_components[i]->getID() == 3)
		{
			checkDeleteResult = false;
			break;
		}
		else
			checkDeleteResult = true;
	}
	EXPECT_TRUE(checkDeleteResult);
	checkDeleteResult = false;

	_deleteEntityCmd->execute();
	// 1
	// 檢查Component中的1是否移除
	checkDeleteResult = false;
	for (int i = 0; i < _erModel->_components.size(); i++)
	{
		if (_erModel->_components[i]->getID() == 1)
		{
			checkDeleteResult = false;
			break;
		}
		else
			checkDeleteResult = true;
	}
	EXPECT_TRUE(checkDeleteResult);
	checkDeleteResult = false;
	
}

TEST_F(DeleteComponentCmdTest, unexecute){
	bool checkResult = false;
	vector<int> primaryKeys;

	// 測試PK沒有設的情況
	_erModel->setPrimaryKey(1, primaryKeys);
	EXPECT_EQ(0, static_cast<NodeEntity*>(_erModel->_components[1])->getPrimaryKey().size());

	// 加入PK
	primaryKeys.push_back(0);
	primaryKeys.push_back(3);
	primaryKeys.push_back(4);
	_erModel->setPrimaryKey(1, primaryKeys);
	EXPECT_EQ(3, static_cast<NodeEntity*>(_erModel->_components[1])->getPrimaryKey().size());

	while(primaryKeys.size() > 0)
		primaryKeys.pop_back();

	primaryKeys.push_back(5);
	_erModel->setPrimaryKey(6, primaryKeys);
	EXPECT_EQ(1, static_cast<NodeEntity*>(_erModel->_components[6])->getPrimaryKey().size());

	//	測試delete Attribute
	_deleteAttributeCmd->execute();
	_deleteAttributeCmd->unexecute();

	// 檢查大小是否有回來12(砍掉C11跟A3)->14
	if (_erModel->_components.size() == 14)
		checkResult = true;
	else
		checkResult = false;

	// 測試delete Entity
	_deleteEntityCmd->execute();
	_deleteEntityCmd->unexecute();
	// 檢查大小
	if (_erModel->_components.size() == 14)
		checkResult = true;
	else
		checkResult = false;

	EXPECT_TRUE(checkResult);
	checkResult = false;

	_deleteConnectorCmd->execute();
	_deleteConnectorCmd->unexecute();

	// 檢查大小是否有回來12(砍掉C11跟A3)->14
	if (_erModel->_components.size() == 14)
		checkResult = true;
	else
		checkResult = false;

	EXPECT_TRUE(checkResult);
	checkResult = false;
}
