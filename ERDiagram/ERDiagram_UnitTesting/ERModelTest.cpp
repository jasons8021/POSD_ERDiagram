#include "ERModelTest.h"

void ERModelTest::SetUp()
{
	_erModel = new ERModel();

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
}

void ERModelTest::TearDown()
{
	delete _erModel;
	remove("unitTest/UnitTestSaveERDiagram.erd");
	_rmdir("unitTest");
	remove("CreatPathTest/CreatPathTestFile.txt");
	_rmdir("CreatPathTest");
}

TEST_F(ERModelTest, addNode)
{
	while(_erModel->_components.size() > 0)
		_erModel->_components.pop_back();
	EXPECT_EQ(0, _erModel->_components.size());
	const int testNum = 7;
	const int testCorrectNum = 4;
	const string testType[testNum] = {"A", "E", "R", "C", "T", " A", "A E"};
	const string testText[testNum] = {"A0", "E1", "R2", "C3", "T4", " A5", "A E6"};
	
	// 正確的輸入type
	for (int i = 0; i < testCorrectNum; i++)
		EXPECT_EQ(i, _erModel->addNode(i,testType[i],testText[i]));

	// 錯誤的輸入
	EXPECT_EQ(PARAMETER_ISERROR, _erModel->addNode(5,testType[5],testText[5]));
	EXPECT_EQ(PARAMETER_ISERROR, _erModel->addNode(6,testType[6],testText[6]));
}

TEST_F(ERModelTest, addConnection)
{
	// Attribute和Entity相連
	EXPECT_EQ(10, _erModel->addConnection(10, 0, 1, "Connection0,1"));
 	EXPECT_EQ(11, _erModel->addConnection(11, 5, 6, "Connection5,6"));
	// Entity 和Relation 相連，設Cardinality
	EXPECT_EQ(12, _erModel->addConnection(12, 1, 2, "1"));
  	EXPECT_EQ(13, _erModel->addConnection(13, 7, 8, "N"));

	// 錯誤的型態相連

	// 查無此Node
	EXPECT_EQ(PARAMETER_ISERROR, _erModel->addConnection(14, 100, 5, "Connection100,5"));
	EXPECT_EQ(PARAMETER_ISERROR, _erModel->addConnection(14, 10, 500, "Connection10,500"));
	// Attribute跟Relation相連
	EXPECT_EQ(PARAMETER_ISERROR, _erModel->addConnection(14, 7, 5, "Connection7,5"));
	EXPECT_EQ(PARAMETER_ISERROR, _erModel->addConnection(14, 5, 7, "Connection5,7"));
	// 自己與自己相連
	EXPECT_EQ(PARAMETER_ISERROR, _erModel->addConnection(14, 0, 0, "Connection0,0"));
	// 已經連過
	EXPECT_EQ(PARAMETER_ISERROR, _erModel->addConnection(14, 0, 1, "Connection0,1"));
	EXPECT_EQ(PARAMETER_ISERROR, _erModel->addConnection(14, 1, 0, "Connection1,0"));
	// 已經連過的Attribute與其他Entity相連
	EXPECT_EQ(PARAMETER_ISERROR, _erModel->addConnection(14, 0, 8, "Connection0,8"));
	EXPECT_EQ(PARAMETER_ISERROR, _erModel->addConnection(14, 5, 9, "Connection5,9"));
	// 相同型態相連
	// Entity連Entity
	EXPECT_EQ(PARAMETER_ISERROR, _erModel->addConnection(14, 1, 8, "Connection1,8"));
	// Attribute連Attribute
	EXPECT_EQ(PARAMETER_ISERROR, _erModel->addConnection(14, 0, 3, "Connection0,3"));
	// Relationship連Relationship
	EXPECT_EQ(PARAMETER_ISERROR, _erModel->addConnection(14, 2, 7, "Connection2,7"));
}

TEST_F(ERModelTest, checkConnectionState)
{
	EXPECT_EQ("CANCONNECT", _erModel->checkConnectionState(_erModel->_components[1], _erModel->_components[0]));
	EXPECT_EQ("CANCONNECT", _erModel->checkConnectionState(_erModel->_components[1], _erModel->_components[2]));
	EXPECT_EQ("CANCONNECT", _erModel->checkConnectionState(_erModel->_components[5], _erModel->_components[6]));

	// 已經連過
	_erModel->addConnection(10, 0, 1, "Connection0,1");
	EXPECT_EQ("The node '1' has already been connected to component '0'.", _erModel->checkConnectionState(_erModel->_components[1], _erModel->_components[0]));
	EXPECT_EQ("The node '0' has already been connected to component '1'.", _erModel->checkConnectionState(_erModel->_components[0], _erModel->_components[1]));
	_erModel->addConnection(11, 1, 2, "1");
	EXPECT_EQ("The node '2' has already been connected to component '1'.", _erModel->checkConnectionState(_erModel->_components[2], _erModel->_components[1]));
	EXPECT_EQ("The node '1' has already been connected to component '2'.", _erModel->checkConnectionState(_erModel->_components[1], _erModel->_components[2]));

	// 型態不符
	// A-A
	EXPECT_EQ("The node '0' cannot be connected by the node '3'.", _erModel->checkConnectionState(_erModel->_components[3], _erModel->_components[0]));
	// E-E
	EXPECT_EQ("The node '9' cannot be connected by the node '6'.", _erModel->checkConnectionState(_erModel->_components[6], _erModel->_components[9]));
	// R-R
	EXPECT_EQ("The node '7' cannot be connected by the node '2'.", _erModel->checkConnectionState(_erModel->_components[2], _erModel->_components[7]));
	// A-R
	EXPECT_EQ("The node '7' cannot be connected by the node '0'.", _erModel->checkConnectionState(_erModel->_components[0], _erModel->_components[7]));

	// 連到自己
	EXPECT_EQ("The node '0' cannot be connected to itself.", _erModel->checkConnectionState(_erModel->_components[0], _erModel->_components[0]));
	EXPECT_EQ("The node '9' cannot be connected to itself.", _erModel->checkConnectionState(_erModel->_components[9], _erModel->_components[9]));
	EXPECT_EQ("The node '7' cannot be connected to itself.", _erModel->checkConnectionState(_erModel->_components[7], _erModel->_components[7]));

	// Attribute已經與其他Entity連結過
	EXPECT_EQ("The node '6' cannot be connected by the node '0'.", _erModel->checkConnectionState(_erModel->_components[0], _erModel->_components[6]));
	_erModel->addConnection(12, 3, 9, "1");
	EXPECT_EQ("The node '8' cannot be connected by the node '3'.", _erModel->checkConnectionState(_erModel->_components[3], _erModel->_components[8]));
}

TEST_F(ERModelTest, getCheckConnectionStateMessage)
{
	EXPECT_EQ("CANCONNECT", _erModel->getCheckConnectionStateMessage(1, 0));
	EXPECT_EQ("CANCONNECT", _erModel->getCheckConnectionStateMessage(1, 2));
	EXPECT_EQ("CANCONNECT", _erModel->getCheckConnectionStateMessage(5, 6));

	// 已經連過
	_erModel->addConnection(10, 0, 1, "Connection0,1");
	EXPECT_EQ("The node '1' has already been connected to component '0'.", _erModel->getCheckConnectionStateMessage(1, 0));
	EXPECT_EQ("The node '0' has already been connected to component '1'.", _erModel->getCheckConnectionStateMessage(0, 1));
	_erModel->addConnection(11, 1, 2, "1");
	EXPECT_EQ("The node '2' has already been connected to component '1'.", _erModel->getCheckConnectionStateMessage(2, 1));
	EXPECT_EQ("The node '1' has already been connected to component '2'.", _erModel->getCheckConnectionStateMessage(1, 2));

	// 型態不符
	// A-A
	EXPECT_EQ("The node '0' cannot be connected by the node '3'.", _erModel->getCheckConnectionStateMessage(3, 0));
	// E-E
	EXPECT_EQ("The node '9' cannot be connected by the node '6'.", _erModel->getCheckConnectionStateMessage(6, 9));
	// R-R
	EXPECT_EQ("The node '7' cannot be connected by the node '2'.", _erModel->getCheckConnectionStateMessage(2, 7));
	// A-R
	EXPECT_EQ("The node '7' cannot be connected by the node '0'.", _erModel->getCheckConnectionStateMessage(0, 7));

	// 連到自己
	EXPECT_EQ("The node '0' cannot be connected to itself.", _erModel->getCheckConnectionStateMessage(0, 0));
	EXPECT_EQ("The node '9' cannot be connected to itself.", _erModel->getCheckConnectionStateMessage(9, 9));
	EXPECT_EQ("The node '7' cannot be connected to itself.", _erModel->getCheckConnectionStateMessage(7, 7));

	// Attribute已經與其他Entity連結過
	EXPECT_EQ("The node '6' cannot be connected by the node '0'.", _erModel->getCheckConnectionStateMessage(0, 6));
	_erModel->addConnection(12, 3, 9, "1");
	EXPECT_EQ("The node '8' cannot be connected by the node '3'.", _erModel->getCheckConnectionStateMessage(3, 8));
}

TEST_F(ERModelTest, checkSetCardinality)
{
	// E-R 或 R-E
	EXPECT_TRUE(_erModel->checkSetCardinality(1, 2));
	EXPECT_TRUE(_erModel->checkSetCardinality(2, 1));
	EXPECT_TRUE(_erModel->checkSetCardinality(7, 9));
	EXPECT_TRUE(_erModel->checkSetCardinality(9, 7));

	// A-E 或 A-R 或 A-A
	EXPECT_FALSE(_erModel->checkSetCardinality(0, 1));
	EXPECT_FALSE(_erModel->checkSetCardinality(1, 0));
	EXPECT_FALSE(_erModel->checkSetCardinality(2, 0));
	EXPECT_FALSE(_erModel->checkSetCardinality(0, 2));
	EXPECT_FALSE(_erModel->checkSetCardinality(0, 0));
	EXPECT_FALSE(_erModel->checkSetCardinality(3, 5));
}

TEST_F(ERModelTest, setCardinality)
{
	// 一開始空的
	EXPECT_TRUE(static_cast<NodeRelationship*>(_erModel->_components[2])->getEntityCardinality().empty());
	//	E-R, 在R中設定EntityCardinality
	_erModel->setCardinality(_erModel->_components[1], _erModel->_components[2], "1");
	EXPECT_FALSE(static_cast<NodeRelationship*>(_erModel->_components[2])->getEntityCardinality().empty());
	_erModel->setCardinality(_erModel->_components[6], _erModel->_components[7], "N");
	EXPECT_FALSE(static_cast<NodeRelationship*>(_erModel->_components[2])->getEntityCardinality().empty());
	_erModel->setCardinality(_erModel->_components[6], _erModel->_components[2], "N");
	EXPECT_EQ(2, static_cast<NodeRelationship*>(_erModel->_components[2])->getEntityCardinality().size());

	//	R-E, 在R中設定EntityCardinality
	_erModel->setCardinality(_erModel->_components[2], _erModel->_components[9], "N");
	EXPECT_EQ(3, static_cast<NodeRelationship*>(_erModel->_components[2])->getEntityCardinality().size());
	_erModel->setCardinality(_erModel->_components[2], _erModel->_components[8], "1");
	EXPECT_EQ(4, static_cast<NodeRelationship*>(_erModel->_components[2])->getEntityCardinality().size());

}

TEST_F(ERModelTest, getComponentsTable)
{
	string allTypeResult = "  A   |  0   |  A0\n";
	allTypeResult += "  E   |  1   |  E1\n";
	allTypeResult += "  R   |  2   |  R2\n";
	allTypeResult += "  A   |  3   |  A3\n";
	allTypeResult += "  A   |  4   |  A4\n";
	allTypeResult += "  A   |  5   |  A5\n";
	allTypeResult += "  E   |  6   |  E6\n";
	allTypeResult += "  R   |  7   |  R7\n";
	allTypeResult += "  E   |  8   |  E8\n";
	allTypeResult += "  E   |  9   |  E9\n";

	string attributeTypeResult = "  A   |  0   |  A0\n";
	attributeTypeResult += "  A   |  3   |  A3\n";
	attributeTypeResult += "  A   |  4   |  A4\n";
	attributeTypeResult += "  A   |  5   |  A5\n";

	string entityTypeResult = "  E   |  1   |  E1\n";
	entityTypeResult += "  E   |  6   |  E6\n";
	entityTypeResult += "  E   |  8   |  E8\n";
	entityTypeResult += "  E   |  9   |  E9\n";

	string relationshipTypeResult = "  R   |  2   |  R2\n";
	relationshipTypeResult += "  R   |  7   |  R7\n";


	EXPECT_EQ(allTypeResult, _erModel->getComponentsTable(PARAMETER_ALL));
	EXPECT_EQ(attributeTypeResult, _erModel->getComponentsTable(PARAMETER_ATTRIBUTE));
	EXPECT_EQ(entityTypeResult, _erModel->getComponentsTable(PARAMETER_ENTITY));
	EXPECT_EQ(relationshipTypeResult, _erModel->getComponentsTable(PARAMETER_RELATIONSHIP));

	// 因為初始化中沒有Connector，所以用來測試搜尋不到結果的情況
	EXPECT_EQ(PARAMETER_NULL, _erModel->getComponentsTable(PARAMETER_CONNECTOR));

	while(_erModel->_components.size() > 0 )
		_erModel->_components.pop_back();
	EXPECT_EQ(PARAMETER_NULL, _erModel->getComponentsTable(PARAMETER_ALL));
}

TEST_F(ERModelTest, getConnectionTable)
{
	// 一開始connections是空的沒東西，用來判斷大小為0的情況
	EXPECT_EQ(PARAMETER_NULL, _erModel->getConnectionTable());
	
	string resultTwoConnection = "     10     |  0   |  1   |\n";
	resultTwoConnection += "     11     |  5   |  6   |\n";

	string resultFourConnection = resultTwoConnection;
	resultFourConnection += "     12     |  1   |  2   |\n";
	resultFourConnection += "     13     |  7   |  8   |\n";

	// 加入兩個Connection後，有兩個連結
	EXPECT_EQ(10, _erModel->addConnection(10, 0, 1, "Connection0,1"));
	EXPECT_EQ(11, _erModel->addConnection(11, 5, 6, "Connection5,6"));
	EXPECT_EQ(resultTwoConnection, _erModel->getConnectionTable());

	// 再加入兩個Connection後，有四個連結
	EXPECT_EQ(12, _erModel->addConnection(12, 1, 2, "1"));
	EXPECT_EQ(13, _erModel->addConnection(13, 7, 8, "N"));
	EXPECT_EQ(resultFourConnection, _erModel->getConnectionTable());
}

TEST_F(ERModelTest, setPrimaryKey)
{
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

	// 測試加入同樣的Attribute是否會增加PK數
	primaryKeys.pop_back();
	primaryKeys.pop_back();
	primaryKeys.push_back(3);
	_erModel->setPrimaryKey(1, primaryKeys);
	EXPECT_EQ(3, static_cast<NodeEntity*>(_erModel->_components[1])->getPrimaryKey().size());

	primaryKeys.push_back(4);
	primaryKeys.push_back(5);
	_erModel->setPrimaryKey(6, primaryKeys);
	EXPECT_EQ(4, static_cast<NodeEntity*>(_erModel->_components[6])->getPrimaryKey().size());
}

TEST_F(ERModelTest, reBuildPrimaryKeyFromAttribute)
{
	// 起始無東西，用來確認加入後是否有改變
	EXPECT_FALSE(static_cast<NodeAttribute*>(_erModel->_components[0])->getIsPrimaryKey());
	EXPECT_FALSE(static_cast<NodeAttribute*>(_erModel->_components[3])->getIsPrimaryKey());
	EXPECT_EQ(0, static_cast<NodeEntity*>(_erModel->_components[1])->getPrimaryKey().size());

	// 加入後
	_erModel->reBuildPrimaryKeyFromAttribute(0,1);
	EXPECT_TRUE(static_cast<NodeAttribute*>(_erModel->_components[0])->getIsPrimaryKey());
	EXPECT_EQ(1, static_cast<NodeEntity*>(_erModel->_components[1])->getPrimaryKey().size());

	// 第二組
	_erModel->reBuildPrimaryKeyFromAttribute(3,1);
	EXPECT_TRUE(static_cast<NodeAttribute*>(_erModel->_components[3])->getIsPrimaryKey());
	EXPECT_EQ(2, static_cast<NodeEntity*>(_erModel->_components[1])->getPrimaryKey().size());

}

TEST_F(ERModelTest, searchAttributeOfEntity)
{
	// 起始空的
	EXPECT_EQ(PARAMETER_NULL, _erModel->searchAttributeOfEntity(1));

	string result = "  A   |  0   |  A0\n";
	result +=  "  A   |  3   |  A3\n";
	result +=  "  A   |  4   |  A4\n";

	EXPECT_EQ(10, _erModel->addConnection(10, 0, 6, "Connection0,6"));
	EXPECT_EQ(11, _erModel->addConnection(11, 3, 6, "Connection3,6"));
	EXPECT_EQ(12, _erModel->addConnection(12, 4, 6, "Connection4,6"));
	
	
	EXPECT_EQ(result, _erModel->searchAttributeOfEntity(6));
}

TEST_F(ERModelTest, searchEntityConnection)
{
	EXPECT_EQ(10, _erModel->addConnection(10, 0, 6, "Connection0,6"));
	EXPECT_EQ(11, _erModel->addConnection(11, 3, 6, "Connection3,6"));
	EXPECT_EQ(12, _erModel->addConnection(12, 4, 6, "Connection4,6"));

	EXPECT_TRUE(_erModel->searchEntityConnection(6, 0, PARAMETER_ATTRIBUTE));
	EXPECT_TRUE(_erModel->searchEntityConnection(6, 3, PARAMETER_ATTRIBUTE));
	EXPECT_TRUE(_erModel->searchEntityConnection(6, 4, PARAMETER_ATTRIBUTE));

	EXPECT_FALSE(_erModel->searchEntityConnection(1, 0, PARAMETER_ATTRIBUTE));
	EXPECT_FALSE(_erModel->searchEntityConnection(1, 3, PARAMETER_ATTRIBUTE));
	EXPECT_FALSE(_erModel->searchEntityConnection(1, 4, PARAMETER_ATTRIBUTE));

}

TEST_F(ERModelTest, getERDiagramTable)
{
	vector<int> primaryKeys;
	string result = " E1       |  PK(A5), FK(A0, A4)\n E6       |  PK(A0, A4), A3\n E8       |  \n E9       |  \n";

	// A-E
	EXPECT_EQ(10, _erModel->addConnection(10, 0, 6, "Connection0,6"));
	EXPECT_EQ(11, _erModel->addConnection(11, 3, 6, "Connection3,6"));
	EXPECT_EQ(12, _erModel->addConnection(12, 4, 6, "Connection4,6"));
	EXPECT_EQ(13, _erModel->addConnection(13, 1, 5, "Connection1,5"));

	// E-R
	EXPECT_EQ(14, _erModel->addConnection(14, 1, 2, "1"));
	EXPECT_EQ(15, _erModel->addConnection(15, 6, 2, "1"));

	// 加入PK
	primaryKeys.push_back(5);
	_erModel->setPrimaryKey(1, primaryKeys);
	EXPECT_EQ(1, static_cast<NodeEntity*>(_erModel->_components[1])->getPrimaryKey().size());
	primaryKeys.pop_back();

	primaryKeys.push_back(0);
	primaryKeys.push_back(4);
	_erModel->setPrimaryKey(6, primaryKeys);
	EXPECT_EQ(2, static_cast<NodeEntity*>(_erModel->_components[6])->getPrimaryKey().size());

	EXPECT_EQ(result, _erModel->getERDiagramTable());
}

TEST_F(ERModelTest, getAttributeContents)
{
	vector<Component*> testAttributeComponents;
	vector<int> primaryKeys;

	// 與E6有連接的Attribute
	testAttributeComponents.push_back(_erModel->_components[0]);
	testAttributeComponents.push_back(_erModel->_components[3]);
	testAttributeComponents.push_back(_erModel->_components[4]);

	EXPECT_EQ(10, _erModel->addConnection(10, 0, 6, "Connection0,6"));
	EXPECT_EQ(11, _erModel->addConnection(11, 3, 6, "Connection3,6"));
	EXPECT_EQ(12, _erModel->addConnection(12, 4, 6, "Connection4,6"));

	// 加入PK
	primaryKeys.push_back(0);
	primaryKeys.push_back(4);
	_erModel->setPrimaryKey(6, primaryKeys);
	EXPECT_EQ(2, static_cast<NodeEntity*>(_erModel->_components[6])->getPrimaryKey().size());

	// 測試E6的PK與非PK的字串輸出
	EXPECT_EQ("PK(A0, A4), A3", _erModel->getAttributeContents(testAttributeComponents));

	// 測試另外一組E1，只有一個A5的Attribute
	while(testAttributeComponents.size() > 0)
		testAttributeComponents.pop_back();

	testAttributeComponents.push_back(_erModel->_components[5]);
	EXPECT_EQ(13, _erModel->addConnection(13, 1, 5, "Connection1,5"));
	EXPECT_EQ("A5", _erModel->getAttributeContents(testAttributeComponents));
}

TEST_F(ERModelTest, setForeignKey)
{
	vector<int> primaryKeys;
	
	// A-E
	EXPECT_EQ(10, _erModel->addConnection(10, 0, 6, "Connection0,6"));
	EXPECT_EQ(11, _erModel->addConnection(11, 3, 6, "Connection3,6"));
	EXPECT_EQ(12, _erModel->addConnection(12, 4, 6, "Connection4,6"));
	EXPECT_EQ(13, _erModel->addConnection(13, 1, 5, "Connection1,5"));

	// E-R
	EXPECT_EQ(14, _erModel->addConnection(14, 1, 2, "1"));
	EXPECT_EQ(15, _erModel->addConnection(15, 6, 2, "1"));

	// 加入PK
	primaryKeys.push_back(5);
	_erModel->setPrimaryKey(1, primaryKeys);
	EXPECT_EQ(1, static_cast<NodeEntity*>(_erModel->_components[1])->getPrimaryKey().size());
	primaryKeys.pop_back();

	primaryKeys.push_back(0);
	primaryKeys.push_back(4);
	_erModel->setPrimaryKey(6, primaryKeys);
	EXPECT_EQ(2, static_cast<NodeEntity*>(_erModel->_components[6])->getPrimaryKey().size());

	// 加入FK
	_erModel->setForeignKey();
	EXPECT_EQ(1, static_cast<NodeEntity*>(_erModel->_components[1])->getForeignKey().size());
	EXPECT_EQ(1, static_cast<NodeEntity*>(_erModel->_components[6])->getForeignKey().size());

	// 加入第二組FK
	EXPECT_EQ(16,_erModel->addNode(16, PARAMETER_ATTRIBUTE, "A16"));
	EXPECT_EQ(17, _erModel->addConnection(17, 16, 9, "Connection16,9"));
	
	while(primaryKeys.size() > 0)
		primaryKeys.pop_back();
	primaryKeys.push_back(16);
	_erModel->setPrimaryKey(9, primaryKeys);
	EXPECT_EQ(1, static_cast<NodeEntity*>(_erModel->_components[9])->getPrimaryKey().size());

	EXPECT_EQ(18, _erModel->addConnection(18, 9, 7, "1"));
	EXPECT_EQ(19, _erModel->addConnection(19, 1, 7, "1"));

	_erModel->setForeignKey();
	EXPECT_EQ(2, static_cast<NodeEntity*>(_erModel->_components[1])->getForeignKey().size());
	EXPECT_EQ(1, static_cast<NodeEntity*>(_erModel->_components[9])->getForeignKey().size());
}

TEST_F(ERModelTest, oneToOne)
{
	// 一開始Entity都沒跟Relationship相連，因此用來測試沒有的情況
	EXPECT_EQ(0, _erModel->oneToOne(static_cast<NodeRelationship*>(_erModel->_components[2])).size());

	// A-E
	EXPECT_EQ(10, _erModel->addConnection(10, 0, 6, "Connection0,6"));
	EXPECT_EQ(11, _erModel->addConnection(11, 3, 6, "Connection3,6"));
	EXPECT_EQ(12, _erModel->addConnection(12, 4, 6, "Connection4,6"));
	EXPECT_EQ(13, _erModel->addConnection(13, 1, 5, "Connection1,5"));

	// E-R with Cardinality = 1,1
	EXPECT_EQ(14, _erModel->addConnection(14, 1, 2, "1"));
	EXPECT_EQ(15, _erModel->addConnection(15, 6, 2, "1"));

	EXPECT_EQ(2, _erModel->oneToOne(static_cast<NodeRelationship*>(_erModel->_components[2])).size());

	// E-R with Cardinality = N,N
	EXPECT_EQ(16, _erModel->addConnection(16, 1, 7, "N"));
	EXPECT_EQ(17, _erModel->addConnection(17, 9, 7, "N"));
	EXPECT_EQ(0, _erModel->oneToOne(static_cast<NodeRelationship*>(_erModel->_components[7])).size());

	// E-R with Cardinality = 1,N
	EXPECT_EQ(18,_erModel->addNode(18, PARAMETER_RELATIONSHIP, "R18"));
	EXPECT_EQ(19, _erModel->addConnection(19, 1, 18, "1"));
	EXPECT_EQ(20, _erModel->addConnection(20, 8, 18, "N"));
	EXPECT_EQ(0, _erModel->oneToOne(static_cast<NodeRelationship*>(_erModel->_components[18])).size());

	// E-R with Cardinality = N,1
	EXPECT_EQ(21,_erModel->addNode(21, PARAMETER_RELATIONSHIP, "R21"));
	EXPECT_EQ(22, _erModel->addConnection(22, 6, 21, "N"));
	EXPECT_EQ(23, _erModel->addConnection(23, 9, 21, "1"));
	EXPECT_EQ(0, _erModel->oneToOne(static_cast<NodeRelationship*>(_erModel->_components[21])).size());
}

TEST_F(ERModelTest, checkOneToOne)
{
	// 一開始Entity都沒跟Relationship相連，因此用來測試沒有的情況
	EXPECT_FALSE(_erModel->checkOneToOne());

	// A-E
	EXPECT_EQ(10, _erModel->addConnection(10, 0, 6, "Connection0,6"));
	EXPECT_EQ(11, _erModel->addConnection(11, 3, 6, "Connection3,6"));
	EXPECT_EQ(12, _erModel->addConnection(12, 4, 6, "Connection4,6"));
	EXPECT_EQ(13, _erModel->addConnection(13, 1, 5, "Connection1,5"));

	// E-R with Cardinality = 1,N
	EXPECT_EQ(16,_erModel->addNode(16, PARAMETER_RELATIONSHIP, "R18"));
	EXPECT_EQ(17, _erModel->addConnection(17, 1, 16, "1"));
	EXPECT_EQ(18, _erModel->addConnection(18, 8, 16, "N"));
	EXPECT_FALSE(_erModel->checkOneToOne());

	// E-R with Cardinality = N,1
	EXPECT_EQ(19,_erModel->addNode(19, PARAMETER_RELATIONSHIP, "R21"));
	EXPECT_EQ(20, _erModel->addConnection(20, 6, 19, "N"));
	EXPECT_EQ(21, _erModel->addConnection(21, 9, 19, "1"));
	EXPECT_FALSE(_erModel->checkOneToOne());

	// E-R with Cardinality = 1,1
	EXPECT_EQ(22, _erModel->addConnection(22, 1, 2, "1"));
	EXPECT_EQ(23, _erModel->addConnection(23, 6, 2, "1"));
	EXPECT_TRUE(_erModel->checkOneToOne());

	// E-R with Cardinality = N,N
	EXPECT_EQ(14, _erModel->addConnection(14, 1, 7, "N"));
	EXPECT_EQ(15, _erModel->addConnection(15, 9, 7, "N"));
	EXPECT_TRUE(_erModel->checkOneToOne());
	
}

TEST_F(ERModelTest, searchForeignKey)
{
	vector<int> primaryKeys;

	// A-E
	EXPECT_EQ(10, _erModel->addConnection(10, 0, 6, "Connection0,6"));
	EXPECT_EQ(11, _erModel->addConnection(11, 3, 6, "Connection3,6"));
	EXPECT_EQ(12, _erModel->addConnection(12, 4, 6, "Connection4,6"));
	EXPECT_EQ(13, _erModel->addConnection(13, 1, 5, "Connection1,5"));

	// E-R
	EXPECT_EQ(14, _erModel->addConnection(14, 1, 2, "1"));
	EXPECT_EQ(15, _erModel->addConnection(15, 6, 2, "1"));

	// 加入PK
	primaryKeys.push_back(5);
	_erModel->setPrimaryKey(1, primaryKeys);
	EXPECT_EQ(1, static_cast<NodeEntity*>(_erModel->_components[1])->getPrimaryKey().size());
	primaryKeys.pop_back();

	primaryKeys.push_back(0);
	primaryKeys.push_back(4);
	_erModel->setPrimaryKey(6, primaryKeys);

	// 加入FK
	_erModel->setForeignKey();
	EXPECT_EQ(1, static_cast<NodeEntity*>(_erModel->_components[1])->getForeignKey().size());
	EXPECT_EQ(1, static_cast<NodeEntity*>(_erModel->_components[6])->getForeignKey().size());

	// 顯示輸出
	EXPECT_EQ(", FK(A5)", _erModel->searchForeignKey(1));
	EXPECT_EQ(", FK(A0, A4)", _erModel->searchForeignKey(6));

	// 加入第二組FK
	EXPECT_EQ(16,_erModel->addNode(16, PARAMETER_ATTRIBUTE, "A16"));
	EXPECT_EQ(17, _erModel->addConnection(17, 16, 9, "Connection16,9"));

	while(primaryKeys.size() > 0)
		primaryKeys.pop_back();
	primaryKeys.push_back(16);
	_erModel->setPrimaryKey(9, primaryKeys);
	EXPECT_EQ(1, static_cast<NodeEntity*>(_erModel->_components[9])->getPrimaryKey().size());

	EXPECT_EQ(18, _erModel->addConnection(18, 9, 7, "1"));
	EXPECT_EQ(19, _erModel->addConnection(19, 1, 7, "1"));

	_erModel->setForeignKey();
	EXPECT_EQ(2, static_cast<NodeEntity*>(_erModel->_components[1])->getForeignKey().size());
	EXPECT_EQ(1, static_cast<NodeEntity*>(_erModel->_components[9])->getForeignKey().size());

	EXPECT_EQ(", FK(A5)", _erModel->searchForeignKey(1));
	EXPECT_EQ(", FK(A16)", _erModel->searchForeignKey(9));

}

TEST_F(ERModelTest, loadERDiagram)
{
	// 清空ERModel中的components，用來確認之後是否有恢復檔案
	while(_erModel->_components.size() > 0)
		_erModel->_components.pop_back();
	_erModel->_componentID = 0;
	EXPECT_TRUE(_erModel->_components.empty());

	// 錯誤的檔名
	EXPECT_EQ("File not found!!\n", _erModel->loadERDiagram("File.erd"));
	// 正確的檔名
	EXPECT_EQ("The ER diagram is displayed as follows:\n", _erModel->loadERDiagram("UnitTestFile.erd"));

	EXPECT_FALSE(_erModel->_components.empty());
}

TEST_F(ERModelTest, classifyInputFile)
{
	vector<vector<string>> resultSet = _erModel->classifyInputFile("E, Engineer\nA, Emp_ID\nR, Has\nA, Name\nC\nC\n\n4 0,1\n5 0,3\n\n0 1,3");
	vector<string> resultComponentSet, resultConnectionSet, resultPrimaryKeysSet;
	string testComponentString, testConnectionString, testPrimaryKeysString;

	EXPECT_FALSE(resultSet.empty());

	resultComponentSet = resultSet[0];
	resultConnectionSet = resultSet[1];
	resultPrimaryKeysSet = resultSet[2];

	while(resultComponentSet.size() > 0)
	{
		testComponentString = resultComponentSet[resultComponentSet.size()-1] + "\n" + testComponentString;
		resultComponentSet.pop_back();
	}
	EXPECT_EQ("E, Engineer\nA, Emp_ID\nR, Has\nA, Name\nC\nC\n",testComponentString);

	while(resultConnectionSet.size() > 0)
	{
		testConnectionString = resultConnectionSet[resultConnectionSet.size()-1] + "\n" + testConnectionString;
		resultConnectionSet.pop_back();
	}
	EXPECT_EQ("4 0,1\n5 0,3\n",testConnectionString);

	while(resultPrimaryKeysSet.size() > 0)
	{
		testPrimaryKeysString = resultPrimaryKeysSet[resultPrimaryKeysSet.size()-1] + "\n" + testPrimaryKeysString;
		resultPrimaryKeysSet.pop_back();
	}
	EXPECT_EQ("0 1,3\n",testPrimaryKeysString);
}

TEST_F(ERModelTest, recoveryFile)
{
	vector<vector<string>> resultSet;

	// 清空ERModel中的components，用來確認之後是否有恢復檔案
	while(_erModel->_components.size() > 0)
		_erModel->_components.pop_back();
	_erModel->_componentID = 0;
	EXPECT_TRUE(_erModel->_components.empty());

	resultSet = _erModel->classifyInputFile("E, Engineer\nA, Emp_ID\nR, Has\nA, Name\nC\nC\n\n4 0,1\n5 0,3\n\n0 1,3");
	EXPECT_FALSE(resultSet.empty());

	_erModel->recoveryFile(resultSet);
	// 檢查_components大小
	EXPECT_EQ(6, _erModel->_components.size());
	// 檢查_connection大小
	EXPECT_EQ(2, _erModel->_connections.size());
	// 檢查PK是否設定回去
	EXPECT_TRUE(static_cast<NodeAttribute*>(_erModel->_components[1])->getIsPrimaryKey());
	EXPECT_TRUE(static_cast<NodeAttribute*>(_erModel->_components[3])->getIsPrimaryKey());
}

TEST_F(ERModelTest, recoveryAllComponent)
{
	vector<string> testComponentSet;
	vector<string> testConnectionSet;

	// 清空ERModel中的components，用來確認之後是否有恢復檔案
	while(_erModel->_components.size() > 0)
		_erModel->_components.pop_back();
	_erModel->_componentID = 0;

	EXPECT_TRUE(_erModel->_components.empty());

	testComponentSet.push_back("E, Engineer");
	testComponentSet.push_back("A, Emp_ID");
	testComponentSet.push_back("R, Has");
	testComponentSet.push_back("A, Name");
	testComponentSet.push_back("C");
	testComponentSet.push_back("C");
	testComponentSet.push_back("C, 1");

	testConnectionSet.push_back("4 0,1");
	testConnectionSet.push_back("5 0,3");
	testConnectionSet.push_back("6 0,2");

	_erModel->recoveryAllComponent(testComponentSet, testConnectionSet);

	// 檢查_components大小
	EXPECT_EQ(7, _erModel->_components.size());
	// 檢查_connection大小
	EXPECT_EQ(3, _erModel->_connections.size());
}

TEST_F(ERModelTest, recoveryConnection)
{
	int connectionSetCount = 0;
	vector<string> testConnectionSet;

	// 清空ERModel中的components，用來確認之後是否有恢復檔案
	while(_erModel->_components.size() > 0)
		_erModel->_components.pop_back();
	_erModel->_componentID = 0;
	EXPECT_TRUE(_erModel->_components.empty());

	EXPECT_EQ(0, _erModel->addNode(0, "E", "Engineer"));
	EXPECT_EQ(1, _erModel->addNode(1, "A", "Emp_ID"));
	EXPECT_EQ(2, _erModel->addNode(2, "R", "Has"));
	EXPECT_EQ(3, _erModel->addNode(3, "A", "Name"));

	testConnectionSet.push_back("4 0,1");
	testConnectionSet.push_back("5 0,3");
	testConnectionSet.push_back("6 0,2");

	// Connector 4 : Engineer - Emp_ID
	connectionSetCount = _erModel->recoveryConnection(testConnectionSet, connectionSetCount, PARAMETER_NULL);
	EXPECT_EQ(1, _erModel->_connections.size());
	EXPECT_EQ(1, connectionSetCount);
	EXPECT_EQ(0, static_cast<Connector*>(_erModel->_connections[0])->getSourceNodeID());
	EXPECT_EQ(1, static_cast<Connector*>(_erModel->_connections[0])->getDestinationNodeID());
	// Connector 5 : Engineer - Name
	connectionSetCount = _erModel->recoveryConnection(testConnectionSet, connectionSetCount, PARAMETER_NULL);
	EXPECT_EQ(2, _erModel->_connections.size());
	EXPECT_EQ(2, connectionSetCount);
	EXPECT_EQ(0, static_cast<Connector*>(_erModel->_connections[1])->getSourceNodeID());
	EXPECT_EQ(3, static_cast<Connector*>(_erModel->_connections[1])->getDestinationNodeID());
	// Connector 6 : Engineer - Has with cardinality = 1.
	connectionSetCount = _erModel->recoveryConnection(testConnectionSet, connectionSetCount, "1");
	EXPECT_EQ(3, _erModel->_connections.size());
	EXPECT_EQ(3, connectionSetCount);
	EXPECT_EQ(0, static_cast<Connector*>(_erModel->_connections[2])->getSourceNodeID());
	EXPECT_EQ(2, static_cast<Connector*>(_erModel->_connections[2])->getDestinationNodeID());
	EXPECT_EQ("1", _erModel->_components[6]->getText());
}

TEST_F(ERModelTest, recoveryPrimaryKey)
{
	vector<string> testPrimaryKeysSet;

	// 清空ERModel中的components，用來確認之後是否有恢復檔案
	while(_erModel->_components.size() > 0)
		_erModel->_components.pop_back();
	_erModel->_componentID = 0;
	EXPECT_TRUE(_erModel->_components.empty());

	
	testPrimaryKeysSet.push_back("0 1,3\n");

	EXPECT_EQ(0, _erModel->addNode(0, "E", "Engineer"));
	EXPECT_EQ(1, _erModel->addNode(1, "A", "Emp_ID"));
	EXPECT_EQ(2, _erModel->addNode(2, "R", "Has"));
	EXPECT_EQ(3, _erModel->addNode(3, "A", "Name"));

	_erModel->recoveryPrimaryKey(testPrimaryKeysSet);
	// 檢查PK是否設定回去
	EXPECT_TRUE(static_cast<NodeAttribute*>(_erModel->_components[1])->getIsPrimaryKey());
	EXPECT_TRUE(static_cast<NodeAttribute*>(_erModel->_components[3])->getIsPrimaryKey());
}

TEST_F(ERModelTest, splitter)
{
	string testPKString = "0 1,3,4,5";
	string testConnectionString = "7 9,10";
	pair<string,vector<string>> result_PK, result_Connection;
	vector<string> result_PKStringSet, result_ConnectionStringSet;
	string result_PKString, result_ConnectionString;

	result_PK = _erModel->splitter(testPKString);
	result_Connection = _erModel->splitter(testConnectionString);

	result_PKStringSet = result_PK.second;
	result_ConnectionStringSet = result_Connection.second;

	while(result_PKStringSet.size() > 0)
	{
		result_PKString = result_PKStringSet[result_PKStringSet.size() - 1] + " " + result_PKString;
		result_PKStringSet.pop_back();
	}

	while(result_ConnectionStringSet.size() > 0)
	{
		result_ConnectionString = result_ConnectionStringSet[result_ConnectionStringSet.size() - 1] + " " + result_ConnectionString;
		result_ConnectionStringSet.pop_back();
	}

	EXPECT_EQ("0", result_PK.first);
	EXPECT_EQ("1 3 4 5 ", result_PKString);
	EXPECT_EQ("7", result_Connection.first);
	EXPECT_EQ("9 10 ", result_ConnectionString);

}

TEST_F(ERModelTest, saveERDiagram)
{
	vector<int> primaryKeys;
	ifstream inputERDiagramTestFile;

	// 清空ERModel中的components，用來確認之後是否有恢復檔案
	while(_erModel->_components.size() > 0)
		_erModel->_components.pop_back();
	_erModel->_componentID = 0;
	EXPECT_TRUE(_erModel->_components.empty());

	EXPECT_EQ(0, _erModel->addNode(0, "E", "Engineer"));
	EXPECT_EQ(1, _erModel->addNode(1, "A", "Emp_ID"));
	EXPECT_EQ(2, _erModel->addNode(2, "R", "Has"));
	EXPECT_EQ(3, _erModel->addNode(3, "A", "Name"));
	EXPECT_EQ(4, _erModel->addConnection(4, 0, 1, PARAMETER_NULL));
	EXPECT_EQ(5, _erModel->addConnection(5, 0, 3, PARAMETER_NULL));
	EXPECT_EQ(6, _erModel->addConnection(6, 0, 2, "1"));

	primaryKeys.push_back(1);
	primaryKeys.push_back(3);
	_erModel->setPrimaryKey(0, primaryKeys);
	EXPECT_EQ(2, static_cast<NodeEntity*>(_erModel->_components[0])->getPrimaryKey().size());

	// 存檔
	_erModel->saveERDiagram(".\\unitTest\\UnitTestSaveERDiagram.erd");

	// 讀檔檢查
	inputERDiagramTestFile.open(".\\unitTest\\UnitTestSaveERDiagram.erd");
	string TestFileSaveText((istreambuf_iterator<char>(inputERDiagramTestFile)),istreambuf_iterator<char>());
	inputERDiagramTestFile.close();

	EXPECT_EQ("E, Engineer\nA, Emp_ID\nR, Has\nA, Name\nC\nC\nC, 1\n\n4 0,1\n5 0,3\n6 0,2\n\n0 1,3\n", TestFileSaveText);

}

TEST_F(ERModelTest, creatFilePath)
{
	ofstream saveCreatPathTestFile;
	saveCreatPathTestFile.open(".\\CreatPathTest\\CreatPathTestFile.txt");

	_erModel->creatFilePath(".\\CreatPathTest\\CreatPathTestFile.txt");
	saveCreatPathTestFile.open(".\\CreatPathTest\\CreatPathTestFile.txt");
	EXPECT_TRUE(saveCreatPathTestFile.is_open());
}

TEST_F(ERModelTest, saveComponentTable)
{
	EXPECT_EQ(10, _erModel->addConnection(10, 0, 1, PARAMETER_NULL));
	EXPECT_EQ(11, _erModel->addConnection(11, 1, 3, PARAMETER_NULL));
	EXPECT_EQ("A, A0\nE, E1\nR, R2\nA, A3\nA, A4\nA, A5\nE, E6\nR, R7\nE, E8\nE, E9\nC\nC\n",_erModel->saveComponentTable());

	EXPECT_EQ(12, _erModel->addConnection(12, 1, 2, "1"));
	EXPECT_EQ("A, A0\nE, E1\nR, R2\nA, A3\nA, A4\nA, A5\nE, E6\nR, R7\nE, E8\nE, E9\nC\nC\nC, 1\n",_erModel->saveComponentTable());
}

TEST_F(ERModelTest, saveConnectionTable)
{
	EXPECT_EQ(10, _erModel->addConnection(10, 0, 1, PARAMETER_NULL));
	EXPECT_EQ(11, _erModel->addConnection(11, 1, 3, PARAMETER_NULL));
	EXPECT_EQ("10 0,1\n11 1,3\n",_erModel->saveConnectionTable());

	EXPECT_EQ(12, _erModel->addConnection(12, 1, 2, "1"));
	EXPECT_EQ("10 0,1\n11 1,3\n12 1,2\n",_erModel->saveConnectionTable());
}

TEST_F(ERModelTest, savePrimaryKeyTable)
{
	vector<int> primaryKeys;

	EXPECT_EQ(10, _erModel->addConnection(10, 0, 1, PARAMETER_NULL));
	EXPECT_EQ(11, _erModel->addConnection(11, 1, 3, PARAMETER_NULL));
	EXPECT_EQ(12, _erModel->addConnection(12, 1, 4, PARAMETER_NULL));
	EXPECT_EQ(13, _erModel->addConnection(13, 5, 8, PARAMETER_NULL));

	// 加入PK
	primaryKeys.push_back(0);
	primaryKeys.push_back(3);
	primaryKeys.push_back(4);
	_erModel->setPrimaryKey(1, primaryKeys);
	EXPECT_EQ(3, static_cast<NodeEntity*>(_erModel->_components[1])->getPrimaryKey().size());
	// 測試輸出
	EXPECT_EQ("1 0,3,4\n\n\n\n", _erModel->savePrimaryKeyTable());

	while(primaryKeys.size() > 0)
		primaryKeys.pop_back();
	primaryKeys.push_back(5);
	_erModel->setPrimaryKey(8, primaryKeys);
	EXPECT_EQ(1, static_cast<NodeEntity*>(_erModel->_components[8])->getPrimaryKey().size());
	// 測試輸出
	EXPECT_EQ("1 0,3,4\n\n8 5\n\n", _erModel->savePrimaryKeyTable());
}

TEST_F(ERModelTest, deleteFunction)
{
	bool checkDeleteResult = false;

	EXPECT_EQ(10, _erModel->addConnection(10, 0, 1, PARAMETER_NULL));
	EXPECT_EQ(11, _erModel->addConnection(11, 1, 3, PARAMETER_NULL));
	EXPECT_EQ(12, _erModel->addConnection(12, 1, 4, PARAMETER_NULL));
	EXPECT_EQ(13, _erModel->addConnection(13, 5, 8, PARAMETER_NULL));

	_erModel->deleteFunction(5);

	for (int i = 0; i < _erModel->_components.size(); i++)
	{
		if (_erModel->_components[i]->getID() == 5)
		{
			checkDeleteResult = false;
			break;
		}
		else
			checkDeleteResult = true;
	}
	EXPECT_TRUE(checkDeleteResult);

	checkDeleteResult = false;
	_erModel->deleteFunction(12);
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
}

TEST_F(ERModelTest, deleteComponent)
{
	bool checkDeleteResult = false;

	EXPECT_EQ(10, _erModel->addConnection(10, 0, 1, PARAMETER_NULL));
	EXPECT_EQ(11, _erModel->addConnection(11, 1, 3, PARAMETER_NULL));
	EXPECT_EQ(12, _erModel->addConnection(12, 1, 4, PARAMETER_NULL));
	EXPECT_EQ(13, _erModel->addConnection(13, 5, 8, PARAMETER_NULL));

	_erModel->deleteComponent(_erModel->_components[3]);
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
}

TEST_F(ERModelTest, deleteConnection)
{
	bool checkDeleteResult = false;

	EXPECT_EQ(10, _erModel->addConnection(10, 0, 1, PARAMETER_NULL));
	EXPECT_EQ(11, _erModel->addConnection(11, 1, 3, PARAMETER_NULL));
	EXPECT_EQ(12, _erModel->addConnection(12, 1, 4, PARAMETER_NULL));
	EXPECT_EQ(13, _erModel->addConnection(13, 5, 8, PARAMETER_NULL));

	_erModel->deleteConnection(_erModel->_components[13]);

	// 檢查Connector是否移除
	checkDeleteResult = false;
	for (int i = 0; i < _erModel->_connections.size(); i++)
	{
		if (_erModel->_connections[i]->getID() == 13)
		{
			checkDeleteResult = false;
			break;
		}
		else
			checkDeleteResult = true;
	}
	EXPECT_TRUE(checkDeleteResult);

	// 檢查與刪除掉的Connector連接的兩個Component，彼此關係是否被移除
	// SourceNode檢查
	for (int i = 0; i < _erModel->_components[5]->getConnections().size(); i++)
	{
		if (_erModel->_components[5]->getConnections()[i]->getID() == 8)
		{
			checkDeleteResult = false;
			break;
		}
		else
			checkDeleteResult = true;
	}
	// destinationNode檢查
	for (int i = 0; i < _erModel->_components[8]->getConnections().size(); i++)
	{
		if (_erModel->_components[8]->getConnections()[i]->getID() == 5)
		{
			checkDeleteResult = false;
			break;
		}
		else
			checkDeleteResult = true;
	}

	EXPECT_TRUE(checkDeleteResult);
}

TEST_F(ERModelTest, deleteTableSet)
{
	bool checkDeleteResult = false;

	EXPECT_EQ(10, _erModel->addConnection(10, 0, 1, PARAMETER_NULL));
	EXPECT_EQ(11, _erModel->addConnection(11, 1, 3, PARAMETER_NULL));
	EXPECT_EQ(12, _erModel->addConnection(12, 1, 4, PARAMETER_NULL));
	EXPECT_EQ(13, _erModel->addConnection(13, 5, 8, PARAMETER_NULL));

	_erModel->deleteTableSet(13, _erModel->_connections, PARAMETER_CONNECTIONSTABLE);

	// 檢查Connector是否移除
	checkDeleteResult = false;
	for (int i = 0; i < _erModel->_connections.size(); i++)
	{
		if (_erModel->_connections[i]->getID() == 13)
		{
			checkDeleteResult = false;
			break;
		}
		else
			checkDeleteResult = true;
	}
	EXPECT_TRUE(checkDeleteResult);
	checkDeleteResult = false;

	_erModel->deleteTableSet(3, _erModel->_components, PARAMETER_COMPONENTSTABLE);
	for (int i = 0; i < _erModel->_connections.size(); i++)
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
}

TEST_F(ERModelTest, addNodeCmd)
{
	bool checkResult = false;

	_erModel->addNodeCmd("A", "CmdA10");

	if (_erModel->_components[10]->getID() == 10 && _erModel->_components[10]->getText() == "CmdA10")
		checkResult = true;
	else
		checkResult = false;

	EXPECT_TRUE(checkResult);
}

TEST_F(ERModelTest, addConnectionCmd)
{
	bool checkResult = false;

	_erModel->addConnectionCmd(0, 1, PARAMETER_NULL);

	if (_erModel->_connections[0]->getID() == 10 && _erModel->_connections[0]->getText() == PARAMETER_NULL)
		checkResult = true;
	else
		checkResult = false;

	EXPECT_TRUE(checkResult);
	checkResult = false;

	_erModel->addConnectionCmd(1, 2, "N");

	if (_erModel->_connections[1]->getID() == 11 && _erModel->_connections[1]->getText() == "N")
		checkResult = true;
	else
		checkResult = false;
	EXPECT_TRUE(checkResult);
}

TEST_F(ERModelTest, deleteCmd)
{
	bool checkDeleteResult = false;

	EXPECT_EQ(10, _erModel->addConnection(10, 0, 1, PARAMETER_NULL));
	EXPECT_EQ(11, _erModel->addConnection(11, 1, 3, PARAMETER_NULL));
	EXPECT_EQ(12, _erModel->addConnection(12, 1, 4, PARAMETER_NULL));
	EXPECT_EQ(13, _erModel->addConnection(13, 5, 8, PARAMETER_NULL));

	_erModel->deleteCmd(3);
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
}

TEST_F(ERModelTest, undoCmd)
{
	bool checkResult = false;

	// 還沒有用Command，所以不能undo
	EXPECT_FALSE(_erModel->undoCmd());

	//
	// 測試AddNode的undo
	//
	_erModel->addNodeCmd("A", "CmdA10");
	EXPECT_TRUE(_erModel->undoCmd());

	// 檢查Component中的10是否被還原，看大小就可以知道
	checkResult = false;
	if (_erModel->_components.size() == 11)
		checkResult = false;
	else
		checkResult = true;
	EXPECT_TRUE(checkResult);
	checkResult = false;

	//
	// 測試Connection的undo
	//
	_erModel->addConnectionCmd(0, 1, PARAMETER_NULL);
	EXPECT_TRUE(_erModel->undoCmd());

	// 檢查與A3有關係的Connector是否移除
	checkResult = false;
	
	if (_erModel->_connections.size() == 1)
		checkResult = false;
	else
		checkResult = true;
	
	EXPECT_TRUE(checkResult);
	checkResult = false;

	// 檢查與刪除掉的Connector連接的兩個Component，彼此關係是否被移除
	// SourceNode檢查Connection大小，因為都沒有連接，所以是0
	if (_erModel->_components[0]->getConnections().size() == 0)
		checkResult = true;
	else
		checkResult = false;

	// destinationNode檢查Connection大小，因為都沒有連接，所以是0
	if (_erModel->_components[1]->getConnections().size() == 0)
		checkResult = true;
	else
		checkResult = false;

	EXPECT_TRUE(checkResult);
	checkResult = false;

	//
	// 測試Delete的undo
	//
	EXPECT_EQ(10, _erModel->addConnection(10, 0, 1, PARAMETER_NULL));
	EXPECT_EQ(11, _erModel->addConnection(11, 1, 3, PARAMETER_NULL));
	EXPECT_EQ(12, _erModel->addConnection(12, 1, 4, PARAMETER_NULL));
	EXPECT_EQ(13, _erModel->addConnection(13, 5, 8, PARAMETER_NULL));

	_erModel->deleteCmd(3);
	EXPECT_TRUE(_erModel->undoCmd());

	// 檢查大小是否有回來12(砍掉C11跟A3)->14
	if (_erModel->_components.size() == 14)
		checkResult = true;
	else
		checkResult = false;

	EXPECT_TRUE(checkResult);
	checkResult = false;

	// 檢查與刪除掉的component的連接關係是否被還原
	for (int i = 0; i < _erModel->_components[1]->getConnections().size(); i++)
	{
		if (_erModel->_components[1]->getConnections()[i]->getID() == 3)
		{
			checkResult = true;
			break;
		}
		else
			checkResult = false;
	}
	EXPECT_TRUE(checkResult);
	checkResult = false;

	// 檢查與A3有關係的Connector是否還原
	for (int i = 0; i < _erModel->_connections.size(); i++)
	{
		if (_erModel->_connections[i]->getID() == 11)
		{
			checkResult = true;
			break;
		}
		else
			checkResult = false;
	}
	EXPECT_TRUE(checkResult);
	checkResult = false;

	// 檢查Component中的3是否還原
	for (int i = 0; i < _erModel->_components.size(); i++)
	{
		if (_erModel->_components[i]->getID() == 3)
		{
			checkResult = true;
			break;
		}
		else
			checkResult = false;
	}
	EXPECT_TRUE(checkResult);
	checkResult = false;
}

TEST_F(ERModelTest, redoCmd)
{
	bool checkResult = false;

	//
	// 檢查Add的redo
	//
	_erModel->addNodeCmd("A", "CmdA10");
	EXPECT_TRUE(_erModel->undoCmd());
	EXPECT_TRUE(_erModel->redoCmd());

	if (_erModel->_components[10]->getID() == 10 && _erModel->_components[10]->getText() == "CmdA10")
		checkResult = true;
	else
		checkResult = false;

	EXPECT_TRUE(checkResult);
	checkResult = false;

	//
	// 檢查Connection的redo
	//
	_erModel->addConnectionCmd(1, 2, "N");
	EXPECT_TRUE(_erModel->undoCmd());
	EXPECT_TRUE(_erModel->redoCmd());

	if (_erModel->_connections[0]->getID() == 11 && _erModel->_connections[0]->getText() == "N")
		checkResult = true;
	else
		checkResult = false;
	EXPECT_TRUE(checkResult);
	checkResult = false;

	//
	// 檢查Delete的redo
	//
	EXPECT_EQ(12, _erModel->addConnection(12, 0, 1, PARAMETER_NULL));
	EXPECT_EQ(13, _erModel->addConnection(13, 1, 3, PARAMETER_NULL));
	EXPECT_EQ(14, _erModel->addConnection(14, 1, 4, PARAMETER_NULL));
	EXPECT_EQ(15, _erModel->addConnection(15, 5, 8, PARAMETER_NULL));

	_erModel->deleteCmd(3);
	EXPECT_TRUE(_erModel->undoCmd());
	EXPECT_TRUE(_erModel->redoCmd());

	// 檢查與刪除掉的component的連接關係是否被移除
	for (int i = 0; i < _erModel->_components[1]->getConnections().size(); i++)
	{
		if (_erModel->_components[1]->getConnections()[i]->getID() == 3)
		{
			checkResult = false;
			break;
		}
		else
			checkResult = true;
	}
	EXPECT_TRUE(checkResult);
	checkResult = false;

	// 檢查與A3有關係的Connector是否移除
	for (int i = 0; i < _erModel->_connections.size(); i++)
	{
		if (_erModel->_connections[i]->getID() == 13)
		{
			checkResult = false;
			break;
		}
		else
			checkResult = true;
	}
	EXPECT_TRUE(checkResult);
	checkResult = false;

	// 檢查Component中的3是否移除
	for (int i = 0; i < _erModel->_components.size(); i++)
	{
		if (_erModel->_components[i]->getID() == 3)
		{
			checkResult = false;
			break;
		}
		else
			checkResult = true;
	}
	EXPECT_TRUE(checkResult);
	checkResult = false;
}

TEST_F(ERModelTest, searchComponent)
{
	EXPECT_EQ(_erModel->_components[0]->getText(), _erModel->searchComponent(0)->getText());
	EXPECT_EQ(NULL, _erModel->searchComponent(10));
}

TEST_F(ERModelTest, searchComponentConnection)
{
	_erModel->addConnectionCmd(1, 2, "N");
	EXPECT_TRUE(_erModel->searchComponentConnection(1, PARAMETER_ENTITY, _erModel->_components[2]->getConnections()));
	EXPECT_FALSE(_erModel->searchComponentConnection(1, PARAMETER_ATTRIBUTE, _erModel->_components[2]->getConnections()));
	EXPECT_FALSE(_erModel->searchComponentConnection(2, PARAMETER_ENTITY, _erModel->_components[1]->getConnections()));
	EXPECT_TRUE(_erModel->searchComponentConnection(2, PARAMETER_RELATIONSHIP, _erModel->_components[1]->getConnections()));
}

TEST_F(ERModelTest, searchComponentExist)
{
	EXPECT_TRUE(_erModel->searchComponentExist("0", PARAMETER_ATTRIBUTE));
	EXPECT_FALSE(_erModel->searchComponentExist("10", PARAMETER_ATTRIBUTE));
	EXPECT_FALSE(_erModel->searchComponentExist("0", PARAMETER_ENTITY));
	EXPECT_FALSE(_erModel->searchComponentExist("0", PARAMETER_RELATIONSHIP));
}

TEST_F(ERModelTest, searchSpecificTypeComponentSet)
{
	vector<Component*> testAttributeSet;
	vector<Component*> testEntitySet;
	vector<Component*> testRelationshipSet;
	vector<Component*> testConnectorSet;

	testAttributeSet.push_back(_erModel->_components[0]);
	testAttributeSet.push_back(_erModel->_components[3]);
	testAttributeSet.push_back(_erModel->_components[4]);
	testAttributeSet.push_back(_erModel->_components[5]);

	testEntitySet.push_back(_erModel->_components[1]);
	testEntitySet.push_back(_erModel->_components[6]);
	testEntitySet.push_back(_erModel->_components[8]);
	testEntitySet.push_back(_erModel->_components[9]);

	testRelationshipSet.push_back(_erModel->_components[2]);
	testRelationshipSet.push_back(_erModel->_components[7]);

	_erModel->addConnectionCmd(1, 2, "N");
	testConnectorSet.push_back(_erModel->_components[10]);

	EXPECT_EQ(testAttributeSet, _erModel->searchSpecificTypeComponentSet(PARAMETER_ATTRIBUTE, _erModel->_components));
	EXPECT_EQ(testEntitySet, _erModel->searchSpecificTypeComponentSet(PARAMETER_ENTITY, _erModel->_components));
	EXPECT_EQ(testRelationshipSet, _erModel->searchSpecificTypeComponentSet(PARAMETER_RELATIONSHIP, _erModel->_components));
	EXPECT_EQ(testConnectorSet, _erModel->searchSpecificTypeComponentSet(PARAMETER_CONNECTOR, _erModel->_components));
}

TEST_F(ERModelTest, getComponentDataList)
{
	string allTypeResult = "  A   |  0   |  A0\n";
	allTypeResult += "  E   |  1   |  E1\n";
	allTypeResult += "  R   |  2   |  R2\n";
	allTypeResult += "  A   |  3   |  A3\n";
	allTypeResult += "  A   |  4   |  A4\n";
	allTypeResult += "  A   |  5   |  A5\n";
	allTypeResult += "  E   |  6   |  E6\n";
	allTypeResult += "  R   |  7   |  R7\n";
	allTypeResult += "  E   |  8   |  E8\n";
	allTypeResult += "  E   |  9   |  E9\n";

	string attributeTypeResult = "  A   |  0   |  A0\n";
	attributeTypeResult += "  A   |  3   |  A3\n";
	attributeTypeResult += "  A   |  4   |  A4\n";
	attributeTypeResult += "  A   |  5   |  A5\n";

	string entityTypeResult = "  E   |  1   |  E1\n";
	entityTypeResult += "  E   |  6   |  E6\n";
	entityTypeResult += "  E   |  8   |  E8\n";
	entityTypeResult += "  E   |  9   |  E9\n";

	string relationshipTypeResult = "  R   |  2   |  R2\n";
	relationshipTypeResult += "  R   |  7   |  R7\n";


	EXPECT_EQ(allTypeResult, _erModel->getComponentDataList(PARAMETER_ALL, _erModel->_components));
	EXPECT_EQ(attributeTypeResult, _erModel->getComponentDataList(PARAMETER_ATTRIBUTE, _erModel->_components));
	EXPECT_EQ(entityTypeResult, _erModel->getComponentDataList(PARAMETER_ENTITY, _erModel->_components));
	EXPECT_EQ(relationshipTypeResult, _erModel->getComponentDataList(PARAMETER_RELATIONSHIP, _erModel->_components));

	// 因為初始化中沒有Connector，所以用來測試搜尋不到結果的情況
	EXPECT_EQ(PARAMETER_NULL, _erModel->getComponentDataList(PARAMETER_CONNECTOR, _erModel->_components));

	while(_erModel->_components.size() > 0 )
		_erModel->_components.pop_back();
	EXPECT_EQ(PARAMETER_NULL, _erModel->getComponentDataList(PARAMETER_ALL, _erModel->_components));
}

TEST_F(ERModelTest, searchConnection)
{
	vector<Component*> testRelatedConnectionSet;

	EXPECT_EQ(10, _erModel->addConnection(10, 0, 1, PARAMETER_NULL));
	EXPECT_EQ(11, _erModel->addConnection(11, 1, 3, PARAMETER_NULL));

	testRelatedConnectionSet.push_back(_erModel->_connections[0]);
	testRelatedConnectionSet.push_back(_erModel->_connections[1]);

	EXPECT_EQ(testRelatedConnectionSet, _erModel->searchConnection(1));
}

TEST_F(ERModelTest, searchRelatedComponent)
{
	vector<Component*> testRelatedComponentSet;

	EXPECT_EQ(10, _erModel->addConnection(10, 0, 1, PARAMETER_NULL));
	EXPECT_EQ(11, _erModel->addConnection(11, 1, 3, PARAMETER_NULL));

	testRelatedComponentSet.push_back(_erModel->_components[0]);
	testRelatedComponentSet.push_back(_erModel->_components[3]);

	EXPECT_EQ(testRelatedComponentSet, _erModel->searchRelatedComponent(1));
}

TEST_F(ERModelTest, resetERModel)
{
	EXPECT_EQ(10, _erModel->addConnection(10, 0, 1, PARAMETER_NULL));
	EXPECT_EQ(11, _erModel->addConnection(11, 1, 3, PARAMETER_NULL));

	_erModel->resetERModel();
	EXPECT_EQ(0,_erModel->_components.size());
	EXPECT_EQ(0,_erModel->_connections.size());
	EXPECT_EQ(0,_erModel->_componentID);
}

TEST_F(ERModelTest, sortCompoentsAndConnection)
{
	const string testSortComponentsTextSet[6] = {"A0", "E1", "R2", "A3", "A4", "A5"};
	const string testNotSortComponentsTextSet[6] = {"E1", "A0", "A3", "A5", "A4", "R2"};

	EXPECT_EQ(11, _erModel->addConnection(11, 0, 1, "Connection0,1"));
	EXPECT_EQ(10, _erModel->addConnection(10, 1, 3, "Connection1,3"));

	EXPECT_EQ(1, static_cast<Connector*>(_erModel->_connections[0])->getDestinationNodeID());
	EXPECT_EQ(3, static_cast<Connector*>(_erModel->_connections[1])->getDestinationNodeID());

	_erModel->sortCompoentsAndConnection();

	EXPECT_EQ(3, static_cast<Connector*>(_erModel->_connections[0])->getDestinationNodeID());
	EXPECT_EQ(1, static_cast<Connector*>(_erModel->_connections[1])->getDestinationNodeID());

	// 清空ERModel中的components，用來確認之後是否有恢復檔案
	while(_erModel->_components.size() > 0)
		_erModel->_components.pop_back();
	_erModel->_componentID = 0;

	EXPECT_TRUE(_erModel->_components.empty());

	EXPECT_EQ(1, _erModel->addNode(1, "E", "E1"));
	EXPECT_EQ(0, _erModel->addNode(0, "A", "A0"));
	EXPECT_EQ(3, _erModel->addNode(3, "A", "A3"));
	EXPECT_EQ(5, _erModel->addNode(5, "A", "A5"));
	EXPECT_EQ(4, _erModel->addNode(4, "A", "A4"));
	EXPECT_EQ(2, _erModel->addNode(2, "R", "R2"));

	for (int i = 0; i < _erModel->_components.size(); i++)
		EXPECT_EQ(testNotSortComponentsTextSet[i], _erModel->_components[i]->getText());
	
	_erModel->sortCompoentsAndConnection();
	
	for (int i = 0; i < _erModel->_components.size(); i++)
		EXPECT_EQ(testSortComponentsTextSet[i], _erModel->_components[i]->getText());
}