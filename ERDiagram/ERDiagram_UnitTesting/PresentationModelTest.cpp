#include "PresentationModelTest.h"

void PresentationModelTest::SetUp()
{
	_erModel = new ERModel();
	_presentationModel = new PresentationModel(_erModel);

	const int testNum = 10;
	const string testType[testNum] = {"A", "E", "R", "A", "A", "A", "E", "R", "E", "E"};
	const string testText[testNum] = {"A0", "E1", "R2", "A3", "A4", "A5", "E6", "R7", "E8", "E9"};

	// �s�Wcomponent�ivector��
	for (int i = 0; i < testNum; i++)
	{
		if (testType[i] == PARAMETER_ATTRIBUTE)
			_presentationModel->_erModel->_components.push_back(new NodeAttribute(i, testText[i], 0, 0));
		else if (testType[i] == PARAMETER_ENTITY)
			_presentationModel->_erModel->_components.push_back(new NodeEntity(i, testText[i], 0, 0));
		else
			_presentationModel->_erModel->_components.push_back(new NodeRelationship(i, testText[i], 0, 0));
		_presentationModel->_erModel->_componentID++;
	}
}

void PresentationModelTest::TearDown()
{
	delete _presentationModel;
	delete _erModel;
}

// ����addNodeCmd_TextUI
TEST_F(PresentationModelTest, addNodeCmd_TextUI)
{
	EXPECT_TRUE(_presentationModel->addNodeCmd_TextUI("A", "CmdA10", 0, 0));

	// ���~���A����
	EXPECT_FALSE(_presentationModel->addNodeCmd_TextUI("Y", "ErrorType0", 0, 0));
	EXPECT_FALSE(_presentationModel->addNodeCmd_TextUI("A C", "ErrorType1", 0, 0));
}

// ���յ�TextUI�Ϊ�addConnectionCmd
TEST_F(PresentationModelTest, addConnectionCmd_TextUI)
{
	EXPECT_EQ("1", _presentationModel->addConnectionCmd_TextUI(0, 1, ""));
	EXPECT_EQ("2", _presentationModel->addConnectionCmd_TextUI(1, 2, "1"));

	// ���~���A����
	EXPECT_EQ("The node '1' cannot be connected to itself.", _presentationModel->addConnectionCmd_TextUI(1, 1, ""));
	EXPECT_EQ("The node '3' cannot be connected by the node '0'.", _presentationModel->addConnectionCmd_TextUI(0, 3, ""));
	EXPECT_EQ("The node '2' cannot be connected to itself.", _presentationModel->addConnectionCmd_TextUI(2, 2, ""));
}

//  ���յ�TextUI�Ϊ��j�MComponent
TEST_F(PresentationModelTest, searchComponent_TextUI)
{
	EXPECT_EQ("0", _presentationModel->searchComponent_TextUI("0", PARAMETER_ALL));

	// ���~���A����
	EXPECT_EQ("The node ID you entered does not exist. Please enter a valid one again.\n> ", _presentationModel->searchComponent_TextUI("11", PARAMETER_ALL));
}

// ���ը��oComponentTable�j�p
TEST_F(PresentationModelTest, getComponentTableSize)
{
	EXPECT_EQ(10, _presentationModel->getComponentTableSize());
}

// ����ConnectionTable�j�p
TEST_F(PresentationModelTest, getConnectionTableSize)
{
	EXPECT_EQ(0, _presentationModel->getConnectionTableSize());

	// �s�WConnector
	_presentationModel->_erModel->addConnection(10, 1, 2, "1");

	EXPECT_EQ(1, _presentationModel->getConnectionTableSize());
}

// ���յ�TextUI�Ϊ��j�MEntity
TEST_F(PresentationModelTest, searchEntity_TextUI)
{
	EXPECT_EQ("1", _presentationModel->searchEntity_TextUI("1"));

	EXPECT_EQ("The node ID you entered does not exist. Please enter a valid one again.\n> ", _presentationModel->searchEntity_TextUI("11"));
	
	EXPECT_EQ("The node '0' is not an entity. Please enter a valid one again.\n> ", _presentationModel->searchEntity_TextUI("0"));
}

// ���յ�TextUI�Ϊ�EntityTable�r��
TEST_F(PresentationModelTest, displayEntityTable_TextUI)
{
	EXPECT_EQ("Entities:\n------------------------------------\n TYPE |  ID  |  NAME\n------+------+----------------------\n  E   |  1   |  E1\n  E   |  6   |  E6\n  E   |  8   |  E8\n  E   |  9   |  E9\n------------------------------------\n", _presentationModel->displayEntityTable_TextUI());
}

// ���յ�TextUI�Ϊ�ConnectionTable�r��
TEST_F(PresentationModelTest, displayConnectionTable_TextUI)
{
	EXPECT_EQ(PARAMETER_NULL, _presentationModel->displayConnectionTable_TextUI());

	// �s�WConnector
	_presentationModel->_erModel->addConnection(10, 1, 0, "");
	_presentationModel->_erModel->addConnection(11, 1, 3, "");

	EXPECT_EQ("Connections:\n--------------------------\nConnection | node | node |\n-----------+------+------|\n     10     |  1   |  0   |\n     11     |  1   |  3   |\n--------------------------\n", _presentationModel->displayConnectionTable_TextUI());
}

// ���յ�TextUI�Ϊ�ComponentTable�r��
TEST_F(PresentationModelTest, displayComponentTable_TextUI)
{
	EXPECT_EQ("Components:\n------------------------------------\n TYPE |  ID  |  NAME\n------+------+----------------------\n  A   |  0   |  A0\n  E   |  1   |  E1\n  R   |  2   |  R2\n  A   |  3   |  A3\n  A   |  4   |  A4\n  A   |  5   |  A5\n  E   |  6   |  E6\n  R   |  7   |  R7\n  E   |  8   |  E8\n  E   |  9   |  E9\n------------------------------------\n", _presentationModel->displayComponentTable_TextUI());

	// �s�WConnector
	_presentationModel->_erModel->addConnection(10, 1, 0, "");
	_presentationModel->_erModel->addConnection(11, 1, 3, "");

	EXPECT_EQ("Components:\n------------------------------------\n TYPE |  ID  |  NAME\n------+------+----------------------\n  A   |  0   |  A0\n  E   |  1   |  E1\n  R   |  2   |  R2\n  A   |  3   |  A3\n  A   |  4   |  A4\n  A   |  5   |  A5\n  E   |  6   |  E6\n  R   |  7   |  R7\n  E   |  8   |  E8\n  E   |  9   |  E9\n  C   |  10   |  \n  C   |  11   |  \n------------------------------------\n", _presentationModel->displayComponentTable_TextUI());
}

// ���յ�TextUI�Ϊ�AttributeTable�r��
TEST_F(PresentationModelTest, displayAttributeTable_TextUI)
{
	_presentationModel->_erModel->addConnection(10, 1, 0, "");
	_presentationModel->_erModel->addConnection(11, 1, 3, "");
	EXPECT_EQ("Attributes of the entity '1'\n------------------------------------\n TYPE |  ID  |  NAME\n------+------+----------------------\n  A   |  0   |  A0\n  A   |  3   |  A3\n------------------------------------\n", _presentationModel->displayAttributeTable_TextUI(1));
}

// ���յ�TextUI�Ϊ��ˬd��Attribute�O���O�Y��Entity��Attribute
TEST_F(PresentationModelTest, checkAttributeInEntity_TextUI)
{
	vector<int> testPrimaryKeys;

	_presentationModel->_erModel->addConnection(10, 1, 0, "");
	_presentationModel->_erModel->addConnection(11, 1, 3, "");

	// �bEntity�����o��Attribute
	testPrimaryKeys.push_back(0);
	testPrimaryKeys.push_back(3);
	EXPECT_EQ(PARAMETER_NULL, _presentationModel->checkAttributeInEntity_TextUI("1", testPrimaryKeys));
	while(testPrimaryKeys.size() > 0)
		testPrimaryKeys.pop_back();

	// ���s�b��component
	testPrimaryKeys.push_back(15);
	EXPECT_EQ("The node ID you entered does not exist. Please enter a valid one again.\n> ", _presentationModel->checkAttributeInEntity_TextUI("1", testPrimaryKeys));
	while(testPrimaryKeys.size() > 0)
		testPrimaryKeys.pop_back();

	// Attribute���bEntity��
	testPrimaryKeys.push_back(0);
	testPrimaryKeys.push_back(5);
	EXPECT_EQ("The node '5' does not belong to Entity '1'. Please enter a valid one again.\n> ", _presentationModel->checkAttributeInEntity_TextUI("1", testPrimaryKeys));
	while(testPrimaryKeys.size() > 0)
		testPrimaryKeys.pop_back();
}

// ���յ�TextUI�Ϊ��]PK
TEST_F(PresentationModelTest, setPrimaryKeys_TextUI)
{
	vector<int> testPrimaryKeys;

	_presentationModel->_erModel->addConnection(10, 1, 0, "");
	_presentationModel->_erModel->addConnection(11, 1, 3, "");

	// �bEntity�����o��Attribute
	testPrimaryKeys.push_back(0);
	testPrimaryKeys.push_back(3);
	EXPECT_EQ("The node '1' has the primary key (0,3).", _presentationModel->setPrimaryKeys_TextUI(1, testPrimaryKeys));	
}

// ���յ�TextUI�Ϊ�ER Table�r��
TEST_F(PresentationModelTest, displayERDiagramTable_TextUI)
{
	vector<int> testPrimaryKeys;

	EXPECT_EQ("It has no table to display.\n", _presentationModel->displayERDiagramTable_TextUI());

	_presentationModel->_erModel->addConnection(10, 1, 0, "");
	_presentationModel->_erModel->addConnection(11, 1, 3, "");

	// �bEntity�����o��Attribute
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

// ���յ�TextUI�Ϊ��x�s
TEST_F(PresentationModelTest, saveERDiagram_TextUI)
{
	ifstream inputERDiagramFile;

	// �T�{�L�ɮ�
	inputERDiagramFile.open(".\\unitTest\\UnitTestSaveERDiagram.erd");
	EXPECT_FALSE(inputERDiagramFile.is_open());
	inputERDiagramFile.close();

	_presentationModel->saveERDiagram_TextUI(".\\unitTest\\UnitTestSaveERDiagram.erd");

	// �T�{�s�ɬO�_���\�s�W
	inputERDiagramFile.open(".\\unitTest\\UnitTestSaveERDiagram.erd");
	EXPECT_TRUE(inputERDiagramFile.is_open());
	inputERDiagramFile.close();

	remove("unitTest/UnitTestSaveERDiagram.erd");
	_rmdir("unitTest");
}

// ���յ�TextUI�Ϊ�Ū��
TEST_F(PresentationModelTest, loadERDiagram_TextUI)
{
	// ���~���ɦW
	EXPECT_EQ("File not found!!\n", _presentationModel->loadERDiagram_TextUI("File.erd"));
	// ���T���ɦW
	EXPECT_EQ("The ER diagram is displayed as follows:\nComponents:\n------------------------------------\n TYPE |  ID  |  NAME\n------+------+----------------------\n  E   |  0   |  Engineer\n  A   |  1   |  Emp_ID\n  R   |  2   |  Has\n  A   |  3   |  Name\n  E   |  4   |  PC\n  A   |  5   |  PC_ID\n  A   |  6   |  Purchase_Date\n  C   |  7   |  \n  C   |  8   |  \n  C   |  9   |  \n  C   |  10   |  \n  C   |  11   |  1\n  C   |  12   |  1\n  A   |  13   |  Department\n  C   |  14   |  \n------------------------------------\n\nConnections:\n--------------------------\nConnection | node | node |\n-----------+------+------|\n     7     |  0   |  1   |\n     8     |  0   |  3   |\n     9     |  4   |  5   |\n     10     |  4   |  6   |\n     11     |  0   |  2   |\n     12     |  2   |  4   |\n     14     |  0   |  13   |\n--------------------------\n", _presentationModel->loadERDiagram_TextUI("UnitTestFile.erd"));
}

// �����ˬd�O�_�n�]Cardinality
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

// ���դ���PK
TEST_F(PresentationModelTest, splitPrimaryKey)
{
	vector<int> testPrimaryKeySet;

	testPrimaryKeySet.push_back(1);
	testPrimaryKeySet.push_back(3);
	EXPECT_EQ(testPrimaryKeySet, _presentationModel->splitPrimaryKey("1,3"));

	testPrimaryKeySet.push_back(5);
	EXPECT_EQ(testPrimaryKeySet, _presentationModel->splitPrimaryKey("1,3,5"));

}

// ���յ�TextUI�Ϊ�deleteComponent
TEST_F(PresentationModelTest, deleteComponent_TextUI)
{
	EXPECT_EQ("The component '0' has been deleted.\nComponents:\n------------------------------------\n TYPE |  ID  |  NAME\n------+------+----------------------\n  E   |  1   |  E1\n  R   |  2   |  R2\n  A   |  3   |  A3\n  A   |  4   |  A4\n  A   |  5   |  A5\n  E   |  6   |  E6\n  R   |  7   |  R7\n  E   |  8   |  E8\n  E   |  9   |  E9\n------------------------------------\n\n", _presentationModel->deleteComponent_TextUI("0"));
	EXPECT_EQ("The component '9' has been deleted.\nComponents:\n------------------------------------\n TYPE |  ID  |  NAME\n------+------+----------------------\n  E   |  1   |  E1\n  R   |  2   |  R2\n  A   |  3   |  A3\n  A   |  4   |  A4\n  A   |  5   |  A5\n  E   |  6   |  E6\n  R   |  7   |  R7\n  E   |  8   |  E8\n------------------------------------\n\n", _presentationModel->deleteComponent_TextUI("9"));
}

// ���յ�TextUI�Ϊ�undo
TEST_F(PresentationModelTest, undo_TextUI)
{
	EXPECT_EQ("Undo failed\n", _presentationModel->undo_TextUI());
	EXPECT_TRUE(_presentationModel->addNodeCmd_TextUI("A", "CmdA10", 0, 0));
	EXPECT_EQ("Undo succeed\nComponents:\n------------------------------------\n TYPE |  ID  |  NAME\n------+------+----------------------\n  A   |  0   |  A0\n  E   |  1   |  E1\n  R   |  2   |  R2\n  A   |  3   |  A3\n  A   |  4   |  A4\n  A   |  5   |  A5\n  E   |  6   |  E6\n  R   |  7   |  R7\n  E   |  8   |  E8\n  E   |  9   |  E9\n------------------------------------\n\n", _presentationModel->undo_TextUI());}

// ���յ�TextUI�Ϊ�redo
TEST_F(PresentationModelTest, redo_TextUI)
{
	EXPECT_EQ("Redo failed\n", _presentationModel->redo_TextUI());
	EXPECT_TRUE(_presentationModel->addNodeCmd_TextUI("A", "CmdA10", 0, 0));
	EXPECT_EQ("Undo succeed\nComponents:\n------------------------------------\n TYPE |  ID  |  NAME\n------+------+----------------------\n  A   |  0   |  A0\n  E   |  1   |  E1\n  R   |  2   |  R2\n  A   |  3   |  A3\n  A   |  4   |  A4\n  A   |  5   |  A5\n  E   |  6   |  E6\n  R   |  7   |  R7\n  E   |  8   |  E8\n  E   |  9   |  E9\n------------------------------------\n\n", _presentationModel->undo_TextUI());
	EXPECT_EQ("Redo succeed\nComponents:\n------------------------------------\n TYPE |  ID  |  NAME\n------+------+----------------------\n  A   |  0   |  A0\n  E   |  1   |  E1\n  R   |  2   |  R2\n  A   |  3   |  A3\n  A   |  4   |  A4\n  A   |  5   |  A5\n  E   |  6   |  E6\n  R   |  7   |  R7\n  E   |  8   |  E8\n  E   |  9   |  E9\n  A   |  10   |  CmdA10\n------------------------------------\n\n", _presentationModel->redo_TextUI());
}

// ���յ�GUI�Ϊ�Component
TEST_F(PresentationModelTest, getComponent_GUI)
{
	string allTypeResult = "A,A0;";
	allTypeResult += "E,E1;";
	allTypeResult += "R,R2;";
	allTypeResult += "A,A3;";
	allTypeResult += "A,A4;";
	allTypeResult += "A,A5;";
	allTypeResult += "E,E6;";
	allTypeResult += "R,R7;";
	allTypeResult += "E,E8;";
	allTypeResult += "E,E9";

	EXPECT_EQ(allTypeResult, _presentationModel->getComponent_GUI());

	while(_erModel->_components.size() > 0 )
		_erModel->_components.pop_back();
	EXPECT_EQ(PARAMETER_NULL, _presentationModel->getComponent_GUI());
}

// ���յ�GUI�Ϊ�Connection
TEST_F(PresentationModelTest, getConnection_GUI)
{
	// �@�}�lconnections�O�Ū��S�F��A�ΨӧP�_�j�p��0�����p
	EXPECT_EQ(PARAMETER_NULL, _presentationModel->getConnection_GUI());

	string resultTwoConnection = "0,1,;";
	resultTwoConnection += "5,6,";

	string resultFourConnection = resultTwoConnection;
	resultFourConnection += ";1,2,1;";
	resultFourConnection += "7,8,N";

	// �[�J���Connection��A����ӳs��
	EXPECT_EQ(10, _erModel->addConnection(10, 0, 1, ""));
	EXPECT_EQ(11, _erModel->addConnection(11, 5, 6, ""));
	EXPECT_EQ(resultTwoConnection, _presentationModel->getConnection_GUI());

	// �A�[�J���Connection��A���|�ӳs��
	EXPECT_EQ(12, _erModel->addConnection(12, 1, 2, "1"));
	EXPECT_EQ(13, _erModel->addConnection(13, 7, 8, "N"));
	EXPECT_EQ(resultFourConnection, _presentationModel->getConnection_GUI());
}

// ���յ�GUI�Ϊ�PrimaryKey
TEST_F(PresentationModelTest, getPrimaryKeySet_GUI)
{
	EXPECT_EQ(PARAMETER_NULL, _presentationModel->getPrimaryKeySet_GUI());

	vector<int> primaryKeys;

	// �[�JPK
	primaryKeys.push_back(0);
	primaryKeys.push_back(3);
	primaryKeys.push_back(4);
	_erModel->setPrimaryKey(1, primaryKeys);
	EXPECT_EQ("0,3,4", _presentationModel->getPrimaryKeySet_GUI());
}

// ���յ�GUI�Ϊ�addConnectionCmd
TEST_F(PresentationModelTest, addConnectionCmd_GUI)
{
	EXPECT_TRUE(_presentationModel->addConnectionCmd_GUI(0, 1, ""));
	EXPECT_TRUE(_presentationModel->addConnectionCmd_GUI(1, 2, "1"));

	// ���~���A����
	EXPECT_FALSE(_presentationModel->addConnectionCmd_GUI(1, 1, ""));
	EXPECT_FALSE(_presentationModel->addConnectionCmd_GUI(0, 3, ""));
	EXPECT_FALSE( _presentationModel->addConnectionCmd_GUI(2, 2, ""));
	EXPECT_FALSE( _presentationModel->addConnectionCmd_GUI(6, 0, ""));
}


// ����addNodeCmd
TEST_F(PresentationModelTest, addNodeCmd)
{
	int beforeAddNodeSize;
	beforeAddNodeSize = _erModel->getComponentTableSize();

	_presentationModel->addNodeCmd("A", "CmdA10", 0, 0);
	EXPECT_TRUE(_erModel->getComponentTableSize() > beforeAddNodeSize);
	EXPECT_EQ("CmdA10", _erModel->_components[10]->getText());
	EXPECT_EQ("A", _erModel->_components[10]->getType());
}

// ����addConnectionCmd
TEST_F(PresentationModelTest, addConnectionCmd)
{
	bool checkResult = false;

	_presentationModel->addConnectionCmd(0, 1, PARAMETER_NULL);

	if (_erModel->_connections[0]->getID() == 10 && _erModel->_connections[0]->getText() == PARAMETER_NULL)
		checkResult = true;
	else
		checkResult = false;

	EXPECT_TRUE(checkResult);
	checkResult = false;

	_presentationModel->addConnectionCmd(1, 2, "N");

	if (_erModel->_connections[1]->getID() == 11 && _erModel->_connections[1]->getText() == "N")
		checkResult = true;
	else
		checkResult = false;
	EXPECT_TRUE(checkResult);
}

// ����deleteCmd
TEST_F(PresentationModelTest, deleteCmd)
{
	bool checkDeleteResult = false;

	EXPECT_EQ(10, _erModel->addConnection(10, 0, 1, PARAMETER_NULL));
	EXPECT_EQ(11, _erModel->addConnection(11, 1, 3, PARAMETER_NULL));
	EXPECT_EQ(12, _erModel->addConnection(12, 1, 4, PARAMETER_NULL));
	EXPECT_EQ(13, _erModel->addConnection(13, 5, 8, PARAMETER_NULL));

	_presentationModel->deleteCmd(3);
	// �ˬd�P�R������component���s�����Y�O�_�Q����
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

	// �ˬd�PA3�����Y��Connector�O�_����
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

	// �ˬdComponent����3�O�_����
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

// ����undo
TEST_F(PresentationModelTest, undoCmd)
{
	bool checkResult = false;

	// �٨S����Command�A�ҥH����undo
	EXPECT_FALSE(_presentationModel->undoCmd());

	//
	// ����AddNode��undo
	//
	_presentationModel->addNodeCmd("A", "CmdA10", 0, 0);
	EXPECT_TRUE(_presentationModel->undoCmd());

	// �ˬdComponent����10�O�_�Q�٭�A�ݤj�p�N�i�H���D
	checkResult = false;
	if (_erModel->getComponentTableSize() == 11)
		checkResult = false;
	else
		checkResult = true;
	EXPECT_TRUE(checkResult);
	checkResult = false;

	//
	// ����Connection��undo
	//
	_presentationModel->addConnectionCmd(0, 1, PARAMETER_NULL);
	EXPECT_TRUE(_presentationModel->undoCmd());

	// �ˬd�PA3�����Y��Connector�O�_����
	checkResult = false;

	if (_erModel->getConnectionTableSize() == 1)
		checkResult = false;
	else
		checkResult = true;

	EXPECT_TRUE(checkResult);
	checkResult = false;

	// �ˬd�P�R������Connector�s�������Component�A�������Y�O�_�Q����
	// SourceNode�ˬdConnection�j�p�A�]�����S���s���A�ҥH�O0
	if (_erModel->_components[0]->getConnections().size() == 0)
		checkResult = true;
	else
		checkResult = false;

	// destinationNode�ˬdConnection�j�p�A�]�����S���s���A�ҥH�O0
	if (_erModel->_components[1]->getConnections().size() == 0)
		checkResult = true;
	else
		checkResult = false;

	EXPECT_TRUE(checkResult);
	checkResult = false;

	//
	// ����Delete��undo
	//
	EXPECT_EQ(10, _erModel->addConnection(10, 0, 1, PARAMETER_NULL));
	EXPECT_EQ(11, _erModel->addConnection(11, 1, 3, PARAMETER_NULL));
	EXPECT_EQ(12, _erModel->addConnection(12, 1, 4, PARAMETER_NULL));
	EXPECT_EQ(13, _erModel->addConnection(13, 5, 8, PARAMETER_NULL));

	_presentationModel->deleteCmd(3);
	EXPECT_TRUE(_presentationModel->undoCmd());

	// �ˬd�j�p�O�_���^��12(�屼C11��A3)->14
	if (_erModel->_components.size() == 14)
		checkResult = true;
	else
		checkResult = false;

	EXPECT_TRUE(checkResult);
	checkResult = false;

	// �ˬd�P�R������component���s�����Y�O�_�Q�٭�
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

	// �ˬd�PA3�����Y��Connector�O�_�٭�
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

	// �ˬdComponent����3�O�_�٭�
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

// ����redo
TEST_F(PresentationModelTest, redoCmd)
{
	bool checkResult = false;

	//
	// �ˬdAdd��redo
	//
	_presentationModel->addNodeCmd("A", "CmdA10", 0 , 0);
	EXPECT_TRUE(_presentationModel->undoCmd());
	EXPECT_TRUE(_presentationModel->redoCmd());

	if (_erModel->_components[10]->getID() == 10 && _erModel->_components[10]->getText() == "CmdA10")
		checkResult = true;
	else
		checkResult = false;

	EXPECT_TRUE(checkResult);
	checkResult = false;

	//
	// �ˬdConnection��redo
	//
	_presentationModel->addConnectionCmd(1, 2, "N");
	EXPECT_TRUE(_presentationModel->undoCmd());
	EXPECT_TRUE(_presentationModel->redoCmd());

	if (_erModel->_connections[0]->getID() == 11 && _erModel->_connections[0]->getText() == "N")
		checkResult = true;
	else
		checkResult = false;
	EXPECT_TRUE(checkResult);
	checkResult = false;

	//
	// �ˬdDelete��redo
	//
	EXPECT_EQ(12, _erModel->addConnection(12, 0, 1, PARAMETER_NULL));
	EXPECT_EQ(13, _erModel->addConnection(13, 1, 3, PARAMETER_NULL));
	EXPECT_EQ(14, _erModel->addConnection(14, 1, 4, PARAMETER_NULL));
	EXPECT_EQ(15, _erModel->addConnection(15, 5, 8, PARAMETER_NULL));

	_presentationModel->deleteCmd(3);
	EXPECT_TRUE(_presentationModel->undoCmd());
	EXPECT_TRUE(_presentationModel->redoCmd());

	// �ˬd�P�R������component���s�����Y�O�_�Q����
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

	// �ˬd�PA3�����Y��Connector�O�_����
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

	// �ˬdComponent����3�O�_����
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

// ����GUI���ק�PK
TEST_F(PresentationModelTest, changePrimaryKeyCmd)
{
	_presentationModel->changePrimaryKeyCmd(0,true);
	EXPECT_FALSE(static_cast<NodeAttribute*>(_erModel->_components[0])->getIsPrimaryKey());

	_erModel->addConnection(10, 0, 1, "");
	_presentationModel->changePrimaryKeyCmd(0,true);
	EXPECT_TRUE(static_cast<NodeAttribute*>(_erModel->_components[0])->getIsPrimaryKey());

	_presentationModel->changePrimaryKeyCmd(0,false);
	EXPECT_FALSE(static_cast<NodeAttribute*>(_erModel->_components[0])->getIsPrimaryKey());
}

// ����GUI���ק�Text
TEST_F(PresentationModelTest, changeTextCmd)
{
	_presentationModel->changeTextCmd(0,"Edited A0");
	EXPECT_EQ("Edited A0", _erModel->_components[0]->getText());

	_presentationModel->changeTextCmd(1,"Edited E1");
	EXPECT_EQ("Edited E1", _erModel->_components[1]->getText());

	_presentationModel->changeTextCmd(2,"Edited R2");
	EXPECT_EQ("Edited R2", _erModel->_components[2]->getText());
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// ���էR���h��
TEST_F(PresentationModelTest, deleteGroupCmd)
{
	vector<int> testIDSet;
	testIDSet.push_back(1);
	testIDSet.push_back(2);
	testIDSet.push_back(3);

	_presentationModel->deleteGroupCmd(testIDSet);
	EXPECT_EQ(7, _erModel->_components.size());
}

// ���հŤU
TEST_F(PresentationModelTest, cutComponentCmd)
{
	vector<int> testIDSet;
	testIDSet.push_back(1);
	testIDSet.push_back(2);
	testIDSet.push_back(3);

	_presentationModel->cutComponentCmd(testIDSet);
	EXPECT_EQ(7, _erModel->_components.size());
	EXPECT_EQ(3, _erModel->_clipboard.size());
	EXPECT_EQ(1, _erModel->_clipboard[0]->getID());
	EXPECT_EQ(2, _erModel->_clipboard[1]->getID());
	EXPECT_EQ(3, _erModel->_clipboard[2]->getID());
}

// ���սƻs
TEST_F(PresentationModelTest, copyComponent)
{
	vector<int> testIDSet;
	testIDSet.push_back(1);
	testIDSet.push_back(2);
	testIDSet.push_back(3);

	_presentationModel->copyComponent(testIDSet);
	EXPECT_EQ(3, _erModel->_clipboard.size());
	EXPECT_EQ(1, _erModel->_clipboard[0]->getID());
	EXPECT_EQ(2, _erModel->_clipboard[1]->getID());
	EXPECT_EQ(3, _erModel->_clipboard[2]->getID());
}

// ���նK�W
TEST_F(PresentationModelTest, pasteComponentCmd)
{
	_erModel->addConnection(10, 0, 1, PARAMETER_NULL);
	_erModel->_componentID++;

	// ����
	_erModel->_clipboard.push_back(_erModel->_components[0]->deepClone());
	_erModel->_clipboard.push_back(_erModel->_components[9]->deepClone());
	_erModel->_clipboard.push_back(_erModel->_components[1]->deepClone());
	_erModel->_clipboard.push_back(_erModel->_components[10]->deepClone());

	_presentationModel->pasteComponentCmd();

	EXPECT_EQ(15, _erModel->_components.size());
	EXPECT_EQ(11, static_cast<Connector*>(_erModel->_components[14])->getSourceNodeID());
	EXPECT_EQ(13, static_cast<Connector*>(_erModel->_components[14])->getDestinationNodeID());
}

// ���ղ���
TEST_F(PresentationModelTest, moveItemCmd)
{
	vector<int> testIDSet;
	testIDSet.push_back(1);
	testIDSet.push_back(2);
	testIDSet.push_back(3);

	_presentationModel->moveItemCmd(testIDSet, 50, 50);

	EXPECT_EQ(50, _erModel->_components[1]->getSx());
	EXPECT_EQ(50, _erModel->_components[1]->getSy());
	EXPECT_EQ(50, _erModel->_components[2]->getSx());
	EXPECT_EQ(50, _erModel->_components[2]->getSy());
	EXPECT_EQ(50, _erModel->_components[3]->getSx());
	EXPECT_EQ(50, _erModel->_components[3]->getSy());
}