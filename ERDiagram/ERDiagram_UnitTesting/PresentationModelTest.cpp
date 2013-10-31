#include "PresentationModelTest.h"


void PresentationModelTest::SetUp()
{
	_erModel = new ERModel();
	_presentationModel = new PresentationModel(_erModel);

	const int testNum = 10;
	const string testType[testNum] = {"A", "E", "R", "A", "A", "A", "E", "R", "E", "E"};
	const string testText[testNum] = {"A0", "E1", "R2", "A3", "A4", "A5", "E6", "R7", "E8", "E9"};

	// 新增component進vector中
	for (int i = 0; i < testNum; i++)
	{
		if (testType[i] == PARAMETER_ATTRIBUTE)
			_presentationModel->_erModel->_components.push_back(new NodeAttribute(i, testText[i]));
		else if (testType[i] == PARAMETER_ENTITY)
			_presentationModel->_erModel->_components.push_back(new NodeEntity(i, testText[i]));
		else
			_presentationModel->_erModel->_components.push_back(new NodeRelationship(i, testText[i]));
		_presentationModel->_erModel->_componentID++;
	}
}

void PresentationModelTest::TearDown()
{
	delete _presentationModel;
	delete _erModel;
}

TEST_F(PresentationModelTest, addNodeCmd)
{
	EXPECT_TRUE(_presentationModel->addNodeCmd("A", "CmdA10"));

	// 錯誤型態測試
	EXPECT_FALSE(_presentationModel->addNodeCmd("Y", "ErrorType0"));
	EXPECT_FALSE(_presentationModel->addNodeCmd("A C", "ErrorType1"));
}

TEST_F(PresentationModelTest, addConnectionCmd)
{
	EXPECT_EQ("1", _presentationModel->addConnectionCmd(0, 1, ""));
	EXPECT_EQ("2", _presentationModel->addConnectionCmd(1, 2, "1"));

	// 錯誤型態測試
	EXPECT_EQ("The node '1' cannot be connected to itself.", _presentationModel->addConnectionCmd(1, 1, ""));
	EXPECT_EQ("The node '3' cannot be connected by the node '0'.", _presentationModel->addConnectionCmd(0, 3, ""));
	EXPECT_EQ("The node '2' cannot be connected to itself.", _presentationModel->addConnectionCmd(2, 2, ""));
}

TEST_F(PresentationModelTest, searchComponent_TextUI)
{
	EXPECT_EQ("0", _presentationModel->searchComponent_TextUI("0", PARAMETER_ALL));

	// 錯誤型態測試
	EXPECT_EQ("The node ID you entered does not exist. Please enter a valid one again.\n> ", _presentationModel->searchComponent_TextUI("11", PARAMETER_ALL));
}

TEST_F(PresentationModelTest, getComponentTableSize)
{
	EXPECT_EQ(10, _presentationModel->getComponentTableSize());
}

TEST_F(PresentationModelTest, getConnectionTableSize)
{
	EXPECT_EQ(0, _presentationModel->getConnectionTableSize());

	// 新增Connector
	_presentationModel->_erModel->addConnection(10, 1, 2, "1");

	EXPECT_EQ(1, _presentationModel->getConnectionTableSize());
}

TEST_F(PresentationModelTest, searchEntity_TextUI)
{
	EXPECT_EQ("1", _presentationModel->searchEntity_TextUI("1"));

	EXPECT_EQ("The node ID you entered does not exist. Please enter a valid one again.\n> ", _presentationModel->searchEntity_TextUI("11"));
	
	EXPECT_EQ("The node '0' is not an entity. Please enter a valid one again.\n> ", _presentationModel->searchEntity_TextUI("0"));
}

TEST_F(PresentationModelTest, displayEntityTable_TextUI)
{
	EXPECT_EQ("Entities:\n------------------------------------\n TYPE |  ID  |  NAME\n------+------+----------------------\n  E   |  1   |  E1\n  E   |  6   |  E6\n  E   |  8   |  E8\n  E   |  9   |  E9\n------------------------------------\n", _presentationModel->displayEntityTable_TextUI());
}

TEST_F(PresentationModelTest, displayConnectionTable_TextUI)
{
	EXPECT_EQ(PARAMETER_NULL, _presentationModel->displayConnectionTable_TextUI());

	// 新增Connector
	_presentationModel->_erModel->addConnection(10, 1, 0, "");
	_presentationModel->_erModel->addConnection(11, 1, 3, "");

	EXPECT_EQ("Connections:\n--------------------------\nConnection | node | node |\n-----------+------+------|\n     10     |  1   |  0   |\n     11     |  1   |  3   |\n--------------------------\n", _presentationModel->displayConnectionTable_TextUI());
}

TEST_F(PresentationModelTest, displayComponentTable_TextUI)
{
	EXPECT_EQ("Components:\n------------------------------------\n TYPE |  ID  |  NAME\n------+------+----------------------\n  A   |  0   |  A0\n  E   |  1   |  E1\n  R   |  2   |  R2\n  A   |  3   |  A3\n  A   |  4   |  A4\n  A   |  5   |  A5\n  E   |  6   |  E6\n  R   |  7   |  R7\n  E   |  8   |  E8\n  E   |  9   |  E9\n------------------------------------\n", _presentationModel->displayComponentTable_TextUI());

	// 新增Connector
	_presentationModel->_erModel->addConnection(10, 1, 0, "");
	_presentationModel->_erModel->addConnection(11, 1, 3, "");

	EXPECT_EQ("Components:\n------------------------------------\n TYPE |  ID  |  NAME\n------+------+----------------------\n  A   |  0   |  A0\n  E   |  1   |  E1\n  R   |  2   |  R2\n  A   |  3   |  A3\n  A   |  4   |  A4\n  A   |  5   |  A5\n  E   |  6   |  E6\n  R   |  7   |  R7\n  E   |  8   |  E8\n  E   |  9   |  E9\n  C   |  10   |  \n  C   |  11   |  \n------------------------------------\n", _presentationModel->displayComponentTable_TextUI());
}

TEST_F(PresentationModelTest, displayAttributeTable_TextUI)
{
	_presentationModel->_erModel->addConnection(10, 1, 0, "");
	_presentationModel->_erModel->addConnection(11, 1, 3, "");
	EXPECT_EQ("Attributes of the entity '1'\n------------------------------------\n TYPE |  ID  |  NAME\n------+------+----------------------\n  A   |  0   |  A0\n  A   |  3   |  A3\n------------------------------------\n", _presentationModel->displayAttributeTable_TextUI(1));
}

TEST_F(PresentationModelTest, checkAttributeInEntity_TextUI)
{
	vector<int> testPrimaryKeys;

	_presentationModel->_erModel->addConnection(10, 1, 0, "");
	_presentationModel->_erModel->addConnection(11, 1, 3, "");

	// 在Entity中有這些Attribute
	testPrimaryKeys.push_back(0);
	testPrimaryKeys.push_back(3);
	EXPECT_EQ(PARAMETER_NULL, _presentationModel->checkAttributeInEntity_TextUI("1", testPrimaryKeys));
	while(testPrimaryKeys.size() > 0)
		testPrimaryKeys.pop_back();

	// 不存在此component
	testPrimaryKeys.push_back(15);
	EXPECT_EQ("The node ID you entered does not exist. Please enter a valid one again.\n> ", _presentationModel->checkAttributeInEntity_TextUI("1", testPrimaryKeys));
	while(testPrimaryKeys.size() > 0)
		testPrimaryKeys.pop_back();

	// Attribute不在Entity中
	testPrimaryKeys.push_back(0);
	testPrimaryKeys.push_back(5);
	EXPECT_EQ("The node '5' does not belong to Entity '1'. Please enter a valid one again.\n> ", _presentationModel->checkAttributeInEntity_TextUI("1", testPrimaryKeys));
	while(testPrimaryKeys.size() > 0)
		testPrimaryKeys.pop_back();
}

TEST_F(PresentationModelTest, setPrimaryKeys_TextUI)
{
	vector<int> testPrimaryKeys;

	_presentationModel->_erModel->addConnection(10, 1, 0, "");
	_presentationModel->_erModel->addConnection(11, 1, 3, "");

	// 在Entity中有這些Attribute
	testPrimaryKeys.push_back(0);
	testPrimaryKeys.push_back(3);
	EXPECT_EQ("The node '1' has the primary key (0,3).", _presentationModel->setPrimaryKeys_TextUI(1, testPrimaryKeys));	
}

TEST_F(PresentationModelTest, displayERDiagramTable_TextUI)
{
	vector<int> testPrimaryKeys;

	EXPECT_EQ("It has no table to display.\n", _presentationModel->displayERDiagramTable_TextUI());

	_presentationModel->_erModel->addConnection(10, 1, 0, "");
	_presentationModel->_erModel->addConnection(11, 1, 3, "");

	// 在Entity中有這些Attribute
	testPrimaryKeys.push_back(0);
	testPrimaryKeys.push_back(3);
	EXPECT_EQ("The node '1' has the primary key (0,3).", _presentationModel->setPrimaryKeys_TextUI(1, testPrimaryKeys));

	while(testPrimaryKeys.size() > 0)
		testPrimaryKeys.pop_back();

	_presentationModel->_erModel->addConnection(12, 5, 6, "");
	_presentationModel->_erModel->addConnection(13, 4, 6, "");

	testPrimaryKeys.push_back(4);
	EXPECT_EQ("The node '6' has the primary key (4).", _presentationModel->setPrimaryKeys_TextUI(6, testPrimaryKeys));

	_presentationModel->_erModel->addConnection(14, 1, 2, "1");
	_presentationModel->_erModel->addConnection(15, 2, 6, "1");

	EXPECT_EQ("----------------------------------------------------------\n Entity   |  Attribute\n----------+-----------------------------------------------\n E1       |  PK(A0, A3), FK(A4)\n E6       |  PK(A4), A5\n E8       |  \n E9       |  \n----------------------------------------------------------\n", _presentationModel->displayERDiagramTable_TextUI());
}

TEST_F(PresentationModelTest, saveERDiagram_TextUI)
{
	ifstream inputERDiagramFile;

	// 確認無檔案
	inputERDiagramFile.open(".\\unitTest\\UnitTestSaveERDiagram.erd");
	EXPECT_FALSE(inputERDiagramFile.is_open());
	inputERDiagramFile.close();

	_presentationModel->saveERDiagram_TextUI(".\\unitTest\\UnitTestSaveERDiagram.erd");

	// 確認存檔是否成功新增
	inputERDiagramFile.open(".\\unitTest\\UnitTestSaveERDiagram.erd");
	EXPECT_TRUE(inputERDiagramFile.is_open());
	inputERDiagramFile.close();

	remove("unitTest/UnitTestSaveERDiagram.erd");
	_rmdir("unitTest");
}

TEST_F(PresentationModelTest, loadERDiagram_TextUI)
{
	// 錯誤的檔名
	EXPECT_EQ("File not found!!\n", _presentationModel->loadERDiagram_TextUI("File.erd"));
	// 正確的檔名
	EXPECT_EQ("The ER diagram is displayed as follows:\nComponents:\n------------------------------------\n TYPE |  ID  |  NAME\n------+------+----------------------\n  E   |  0   |  Engineer\n  A   |  1   |  Emp_ID\n  R   |  2   |  Has\n  A   |  3   |  Name\n  E   |  4   |  PC\n  A   |  5   |  PC_ID\n  A   |  6   |  Purchase_Date\n  C   |  7   |  \n  C   |  8   |  \n  C   |  9   |  \n  C   |  10   |  \n  C   |  11   |  1\n  C   |  12   |  1\n  A   |  13   |  Department\n  C   |  14   |  \n------------------------------------\n\nConnections:\n--------------------------\nConnection | node | node |\n-----------+------+------|\n     7     |  0   |  1   |\n     8     |  0   |  3   |\n     9     |  4   |  5   |\n     10     |  4   |  6   |\n     11     |  0   |  2   |\n     12     |  2   |  4   |\n     14     |  0   |  13   |\n--------------------------\n", _presentationModel->loadERDiagram_TextUI("UnitTestFile.erd"));
}
//"A0", "E1", "R2", "A3", "A4", "A5", "E6", "R7", "E8", "E9"};
TEST_F(PresentationModelTest, checkSetCardinality)
{
	EXPECT_TRUE(_presentationModel->checkSetCardinality(1,2));
	EXPECT_TRUE(_presentationModel->checkSetCardinality(2,1));
	EXPECT_TRUE(_presentationModel->checkSetCardinality(6,7));
	EXPECT_TRUE(_presentationModel->checkSetCardinality(7,6));
	EXPECT_FALSE(_presentationModel->checkSetCardinality(0,9));
	EXPECT_FALSE(_presentationModel->checkSetCardinality(9,0));
	EXPECT_FALSE(_presentationModel->checkSetCardinality(2,3));
	EXPECT_FALSE(_presentationModel->checkSetCardinality(3,2));
}

TEST_F(PresentationModelTest, splitPrimaryKey)
{
	vector<int> testPrimaryKeySet;

	testPrimaryKeySet.push_back(1);
	testPrimaryKeySet.push_back(3);
	EXPECT_EQ(testPrimaryKeySet, _presentationModel->splitPrimaryKey("1,3"));

	testPrimaryKeySet.push_back(5);
	EXPECT_EQ(testPrimaryKeySet, _presentationModel->splitPrimaryKey("1,3,5"));

}

TEST_F(PresentationModelTest, deleteComponent_TextUI)
{
	EXPECT_EQ("The component '0' has been deleted.\nComponents:\n------------------------------------\n TYPE |  ID  |  NAME\n------+------+----------------------\n  E   |  1   |  E1\n  R   |  2   |  R2\n  A   |  3   |  A3\n  A   |  4   |  A4\n  A   |  5   |  A5\n  E   |  6   |  E6\n  R   |  7   |  R7\n  E   |  8   |  E8\n  E   |  9   |  E9\n------------------------------------\n\n", _presentationModel->deleteComponent_TextUI("0"));
	EXPECT_EQ("The component '9' has been deleted.\nComponents:\n------------------------------------\n TYPE |  ID  |  NAME\n------+------+----------------------\n  E   |  1   |  E1\n  R   |  2   |  R2\n  A   |  3   |  A3\n  A   |  4   |  A4\n  A   |  5   |  A5\n  E   |  6   |  E6\n  R   |  7   |  R7\n  E   |  8   |  E8\n------------------------------------\n\n", _presentationModel->deleteComponent_TextUI("9"));
}

TEST_F(PresentationModelTest, undo_TextUI)
{
	EXPECT_EQ("Undo failed\n", _presentationModel->undo_TextUI());
	EXPECT_TRUE(_presentationModel->addNodeCmd("A", "CmdA10"));
	EXPECT_EQ("Undo succeed\nComponents:\n------------------------------------\n TYPE |  ID  |  NAME\n------+------+----------------------\n  A   |  0   |  A0\n  E   |  1   |  E1\n  R   |  2   |  R2\n  A   |  3   |  A3\n  A   |  4   |  A4\n  A   |  5   |  A5\n  E   |  6   |  E6\n  R   |  7   |  R7\n  E   |  8   |  E8\n  E   |  9   |  E9\n------------------------------------\n\n", _presentationModel->undo_TextUI());}

TEST_F(PresentationModelTest, redo_TextUI)
{
	EXPECT_EQ("Redo failed\n", _presentationModel->redo_TextUI());
	EXPECT_TRUE(_presentationModel->addNodeCmd("A", "CmdA10"));
	EXPECT_EQ("Undo succeed\nComponents:\n------------------------------------\n TYPE |  ID  |  NAME\n------+------+----------------------\n  A   |  0   |  A0\n  E   |  1   |  E1\n  R   |  2   |  R2\n  A   |  3   |  A3\n  A   |  4   |  A4\n  A   |  5   |  A5\n  E   |  6   |  E6\n  R   |  7   |  R7\n  E   |  8   |  E8\n  E   |  9   |  E9\n------------------------------------\n\n", _presentationModel->undo_TextUI());
	EXPECT_EQ("Redo succeed\nComponents:\n------------------------------------\n TYPE |  ID  |  NAME\n------+------+----------------------\n  A   |  0   |  A0\n  E   |  1   |  E1\n  R   |  2   |  R2\n  A   |  3   |  A3\n  A   |  4   |  A4\n  A   |  5   |  A5\n  E   |  6   |  E6\n  R   |  7   |  R7\n  E   |  8   |  E8\n  E   |  9   |  E9\n  A   |  10   |  CmdA10\n------------------------------------\n\n", _presentationModel->redo_TextUI());
}