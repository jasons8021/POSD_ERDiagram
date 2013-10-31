#include "IntegrationTest.h"

void IntegrationTest::SetUp()
{
	_mkdir("testdata");
	ofstream outputFile("testdata/test_file1.erd");
	outputFile << "E, Engineer" << endl;
	outputFile << "A, Emp_ID" << endl;
	outputFile << "R, Has" << endl;
	outputFile << "A, Name" << endl;
	outputFile << "E, PC" << endl;
	outputFile << "A, PC_ID" << endl;
	outputFile << "A, Purchase_Date" << endl;
	outputFile << "C" << endl;
	outputFile << "C" << endl;
	outputFile << "C" << endl;
	outputFile << "C" << endl;
	outputFile << "C, 1" << endl;
	outputFile << "C, 1" << endl;
	outputFile << "A, Department" << endl;
	outputFile << "C" << endl;
	outputFile << endl;
	outputFile << "7 0,1" << endl;
	outputFile << "8 0,3" << endl;
	outputFile << "9 4,5" << endl;
	outputFile << "10 4,6" << endl;
	outputFile << "11 0,2" << endl;
	outputFile << "12 2,4" << endl;
	outputFile << "14 0,13" << endl;
	outputFile << endl;
	outputFile << "0 1,3" << endl;
	outputFile << "4 5" << endl;
	outputFile.close();

	_erModel = new ERModel();
}

void IntegrationTest::TearDown()
{
	delete _erModel;
	remove("testdata/test_file1.erd");
	_rmdir("testdata");
}

TEST_F(IntegrationTest, testLoadFileNotExist)
{
	string result;
	result = _erModel->loadERDiagram("testdata\\file_not_exist.erd");
	EXPECT_EQ("File not found!!\n", result);
}

TEST_F(IntegrationTest, testIsPrimaryExist)
{
	string result;
	string inputFileTextSet;
	string componentTableString;
	string connectionTableString;
	// Load the “test_file1.erd” under the “testdata” directory
	inputFileTextSet = _erModel->loadERDiagram("testdata\\test_file1.erd");

	// Assert the diagram is loaded correctly
	EXPECT_EQ("The ER diagram is displayed as follows:\n", inputFileTextSet);

	// Display the table
	componentTableString = _erModel->getComponentsTable(PARAMETER_ALL);
	EXPECT_EQ("  E   |  0   |  Engineer\n  A   |  1   |  Emp_ID\n  R   |  2   |  Has\n  A   |  3   |  Name\n  E   |  4   |  PC\n  A   |  5   |  PC_ID\n  A   |  6   |  Purchase_Date\n  C   |  7   |  \n  C   |  8   |  \n  C   |  9   |  \n  C   |  10   |  \n  C   |  11   |  1\n  C   |  12   |  1\n  A   |  13   |  Department\n  C   |  14   |  \n", componentTableString);
	connectionTableString = _erModel->getConnectionTable();
	EXPECT_EQ("     7     |  0   |  1   |\n     8     |  0   |  3   |\n     9     |  4   |  5   |\n     10     |  4   |  6   |\n     11     |  0   |  2   |\n     12     |  2   |  4   |\n     14     |  0   |  13   |\n", connectionTableString);
	
	// Assert Engineer’s primary key is “Name” and “Emp_ID”
	// Engineer只有兩個PK
	EXPECT_EQ(2, static_cast<NodeEntity*>(_erModel->searchComponent(0))->getPrimaryKey().size());
	EXPECT_EQ(1, static_cast<NodeEntity*>(_erModel->searchComponent(0))->getPrimaryKey()[0]);
	EXPECT_EQ(3, static_cast<NodeEntity*>(_erModel->searchComponent(0))->getPrimaryKey()[1]);

	// Assert PC’s primary key is “PC_ID”
	// PC只有一個PK : PC_ID
	EXPECT_EQ(1, static_cast<NodeEntity*>(_erModel->searchComponent(4))->getPrimaryKey().size());
	EXPECT_EQ(5, static_cast<NodeEntity*>(_erModel->searchComponent(4))->getPrimaryKey()[0]);
}

// 測試 testUndoDeleteComponent 
TEST_F(IntegrationTest, testUndoDeleteComponent)
{
	string inputFileTextSet;
	// Load the “test_file1.erd” under the “testdata” directory
	inputFileTextSet = _erModel->loadERDiagram("testdata\\test_file1.erd");

	// Assert the diagram is loaded correctly
	EXPECT_EQ("The ER diagram is displayed as follows:\n", inputFileTextSet);

	// Add an entity with text “Test”
	_erModel->addNodeCmd(PARAMETER_ENTITY, "Test");
	// 新增的Test，其ComponentID為15
	EXPECT_EQ("Test", _erModel->searchComponent(15)->getText());

	// Delete the entity added above
	_erModel->deleteCmd(15);

	// Assert the entity has been deleted
	EXPECT_EQ(NULL, _erModel->searchComponent(15));

	// Undo
	_erModel->undoCmd();
	// Assert there is an entity with text “Test” 
	EXPECT_EQ("Test", _erModel->searchComponent(15)->getText());
}

TEST_F(IntegrationTest, testRedoConnectComponent)
{
	string inputFileTextSet;
	// Load the “test_file1.erd” under the “testdata” directory
	inputFileTextSet = _erModel->loadERDiagram("testdata\\test_file1.erd");

	// Assert the diagram is loaded correctly
	EXPECT_EQ("The ER diagram is displayed as follows:\n", inputFileTextSet);

	// Add an entity with text “Test”
	_erModel->addNodeCmd(PARAMETER_ENTITY, "Test");
	// 新增的Test，其ComponentID為15
	EXPECT_EQ("Test", _erModel->searchComponent(15)->getText());

	// Add an entity with text “Test Attr”
	_erModel->addNodeCmd(PARAMETER_ATTRIBUTE, "Test Attr");
	// 新增的Test，其ComponentID為16
	EXPECT_EQ("Test Attr", _erModel->searchComponent(16)->getText());

	// Connect node 15 and 16
	// Assert there is a connection between node 15 and 16 
	_erModel->addConnectionCmd(15, 16, PARAMETER_NULL);
	EXPECT_EQ("C", _erModel->searchComponent(17)->getType());

	// Undo
	_erModel->undoCmd();

	// Assert there is no connection between node 15 and 16
	// 15跟16的connection已經空了，表示已經刪除連結
	EXPECT_EQ(0, _erModel->searchRelatedComponent(15).size());
	EXPECT_EQ(0, _erModel->searchRelatedComponent(16).size());

	// Redo
	_erModel->redoCmd();

	// Assert node 15 and 16 are connected
	EXPECT_EQ("C", _erModel->searchComponent(17)->getType());
}

TEST_F(IntegrationTest, testCommonUsage)
{
	bool checkPK = false;
	string inputFileTextSet;
	vector<int> primaryKeys;
	// Load the “test_file1.erd” under the “testdata” directory
	inputFileTextSet = _erModel->loadERDiagram("testdata\\test_file1.erd");

	// Assert the diagram is loaded correctly
	EXPECT_EQ("The ER diagram is displayed as follows:\n", inputFileTextSet);

	// Add an Entity with text "Work Diary"
	_erModel->addNodeCmd(PARAMETER_ENTITY, "Work Diary");
	// 新增的Work Diary，其ComponentID為15
	EXPECT_EQ("Work Diary", _erModel->searchComponent(15)->getText());

	// Add a Relationship with text "Write" 
	_erModel->addNodeCmd(PARAMETER_RELATIONSHIP, "Write");
	// 新增的Write，其ComponentID為16
	EXPECT_EQ("Write", _erModel->searchComponent(16)->getText());

	// Connect node 0 and 16, and set its cardinality as "1" 
	_erModel->addConnectionCmd(0, 16, "1");
	// Assert node 0 and 16 are connected
	EXPECT_EQ("C", _erModel->searchComponent(17)->getType());
	EXPECT_EQ("1", _erModel->searchComponent(17)->getText());

	// Connect node 15 and 16 
	_erModel->addConnectionCmd(15, 16, PARAMETER_NULL);
	// Assert node 15 and 16 are connected
	EXPECT_EQ("C", _erModel->searchComponent(18)->getType());

	// Add an Attribute with text "Content"
	_erModel->addNodeCmd(PARAMETER_ATTRIBUTE, "Content");
	// 新增的Content，其ComponentID為19
	EXPECT_EQ("Content", _erModel->searchComponent(19)->getText());

	// Add an Attribute with text "WD_ID"
	_erModel->addNodeCmd(PARAMETER_ATTRIBUTE, "WD_ID");
	// 新增的WD_ID，其ComponentID為20
	EXPECT_EQ("WD_ID", _erModel->searchComponent(20)->getText());

	// Add an Attribute with text "WD_date"
	_erModel->addNodeCmd(PARAMETER_ATTRIBUTE, "WD_date");
	// 新增的WD_ID，其ComponentID為21
	EXPECT_EQ("WD_date", _erModel->searchComponent(21)->getText());

	// Connect node 15 and 19
	_erModel->addConnectionCmd(15, 19, PARAMETER_NULL);
	// Assert node 15 and 19 are connected
	EXPECT_EQ("C", _erModel->searchComponent(22)->getType());

	// Connect node 15 and 20
	_erModel->addConnectionCmd(15, 20, PARAMETER_NULL);
	// Assert node 15 and 20 are connected
	EXPECT_EQ("C", _erModel->searchComponent(23)->getType());

	// Connect node 15 and 21
	_erModel->addConnectionCmd(15, 21, PARAMETER_NULL);
	// Assert node 15 and 21 are connected
	EXPECT_EQ("C", _erModel->searchComponent(24)->getType());

	// Set "Work Diary" primary key "WD_ID" 
	primaryKeys.push_back(20);
	_erModel->setPrimaryKey(15, primaryKeys);
	primaryKeys.pop_back();
	// Assert "Work Diary" has the primary key "WD_ID" 
	for (int i = 0; i < _erModel->searchRelatedComponent(15).size(); i++)
	{
		if (_erModel->searchRelatedComponent(15)[i]->getID() == 20)
		{
			if (static_cast<NodeAttribute*>(_erModel->searchRelatedComponent(15)[i])->getIsPrimaryKey())
			{	
				checkPK = true;
				break;
			}
		}
	}
	EXPECT_TRUE(checkPK);
	checkPK = false;

	// Display the table
	EXPECT_EQ("  E   |  0   |  Engineer\n  A   |  1   |  Emp_ID\n  R   |  2   |  Has\n  A   |  3   |  Name\n  E   |  4   |  PC\n  A   |  5   |  PC_ID\n  A   |  6   |  Purchase_Date\n  C   |  7   |  \n  C   |  8   |  \n  C   |  9   |  \n  C   |  10   |  \n  C   |  11   |  1\n  C   |  12   |  1\n  A   |  13   |  Department\n  C   |  14   |  \n  E   |  15   |  Work Diary\n  R   |  16   |  Write\n  C   |  17   |  1\n  C   |  18   |  \n  A   |  19   |  Content\n  A   |  20   |  WD_ID\n  A   |  21   |  WD_date\n  C   |  22   |  \n  C   |  23   |  \n  C   |  24   |  \n", _erModel->getComponentsTable(PARAMETER_ALL));
	EXPECT_EQ("     7     |  0   |  1   |\n     8     |  0   |  3   |\n     9     |  4   |  5   |\n     10     |  4   |  6   |\n     11     |  0   |  2   |\n     12     |  2   |  4   |\n     14     |  0   |  13   |\n     17     |  0   |  16   |\n     18     |  15   |  16   |\n     22     |  15   |  19   |\n     23     |  15   |  20   |\n     24     |  15   |  21   |\n", _erModel->getConnectionTable());
	EXPECT_EQ(" Engineer |  PK(Emp_ID, Name), Department, FK(PC_ID)\n PC       |  PK(PC_ID), Purchase_Date\n Work Diary|  PK(WD_ID), Content, WD_date\n", _erModel->getERDiagramTable());

	// Assert the Entity "Work Diary" exists
	EXPECT_TRUE(_erModel->searchComponentExist("15", PARAMETER_ENTITY));

	// Assert "Work Diary" has the primary key "WD_ID" 
	for (int i = 0; i < _erModel->searchRelatedComponent(15).size(); i++)
	{
		if (_erModel->searchRelatedComponent(15)[i]->getID() == 20)
		{
			if (static_cast<NodeAttribute*>(_erModel->searchRelatedComponent(15)[i])->getIsPrimaryKey())
			{	
				checkPK = true;
				break;
			}
		}
	}
	EXPECT_TRUE(checkPK);
	checkPK = false;

	// Delete the Entity he/she added above 
	_erModel->deleteCmd(15);

	// Assert the Entity has been deleted
	// 18: 15-16 | 22: 15-19 | 23 : 15-20 | 24 : 15-21 
	EXPECT_EQ(NULL, _erModel->searchComponent(18));
	EXPECT_EQ(NULL, _erModel->searchComponent(22));
	EXPECT_EQ(NULL, _erModel->searchComponent(23));
	EXPECT_EQ(NULL, _erModel->searchComponent(24));

	// Display the table
	EXPECT_EQ("  E   |  0   |  Engineer\n  A   |  1   |  Emp_ID\n  R   |  2   |  Has\n  A   |  3   |  Name\n  E   |  4   |  PC\n  A   |  5   |  PC_ID\n  A   |  6   |  Purchase_Date\n  C   |  7   |  \n  C   |  8   |  \n  C   |  9   |  \n  C   |  10   |  \n  C   |  11   |  1\n  C   |  12   |  1\n  A   |  13   |  Department\n  C   |  14   |  \n  R   |  16   |  Write\n  C   |  17   |  1\n  A   |  19   |  Content\n  A   |  20   |  WD_ID\n  A   |  21   |  WD_date\n", _erModel->getComponentsTable(PARAMETER_ALL));
	EXPECT_EQ("     7     |  0   |  1   |\n     8     |  0   |  3   |\n     9     |  4   |  5   |\n     10     |  4   |  6   |\n     11     |  0   |  2   |\n     12     |  2   |  4   |\n     14     |  0   |  13   |\n     17     |  0   |  16   |\n", _erModel->getConnectionTable());
	EXPECT_EQ(" Engineer |  PK(Emp_ID, Name), Department, FK(PC_ID)\n PC       |  PK(PC_ID), Purchase_Date\n", _erModel->getERDiagramTable());

	// Assert the Entity "Work Diary" does not exist
	EXPECT_EQ(NULL, _erModel->searchComponent(15));

	// Assert Engineer's primary key is "Name" and "Emp_ID"
	EXPECT_EQ(2, static_cast<NodeEntity*>(_erModel->searchComponent(0))->getPrimaryKey().size());
	EXPECT_EQ(1, static_cast<NodeEntity*>(_erModel->searchComponent(0))->getPrimaryKey()[0]);
	EXPECT_EQ(3, static_cast<NodeEntity*>(_erModel->searchComponent(0))->getPrimaryKey()[1]);

	// Undo
	_erModel->undoCmd();

	// Display the table
	EXPECT_EQ("  E   |  0   |  Engineer\n  A   |  1   |  Emp_ID\n  R   |  2   |  Has\n  A   |  3   |  Name\n  E   |  4   |  PC\n  A   |  5   |  PC_ID\n  A   |  6   |  Purchase_Date\n  C   |  7   |  \n  C   |  8   |  \n  C   |  9   |  \n  C   |  10   |  \n  C   |  11   |  1\n  C   |  12   |  1\n  A   |  13   |  Department\n  C   |  14   |  \n  E   |  15   |  Work Diary\n  R   |  16   |  Write\n  C   |  17   |  1\n  C   |  18   |  \n  A   |  19   |  Content\n  A   |  20   |  WD_ID\n  A   |  21   |  WD_date\n  C   |  22   |  \n  C   |  23   |  \n  C   |  24   |  \n", _erModel->getComponentsTable(PARAMETER_ALL));
	EXPECT_EQ("     7     |  0   |  1   |\n     8     |  0   |  3   |\n     9     |  4   |  5   |\n     10     |  4   |  6   |\n     11     |  0   |  2   |\n     12     |  2   |  4   |\n     14     |  0   |  13   |\n     17     |  0   |  16   |\n     18     |  15   |  16   |\n     22     |  15   |  19   |\n     23     |  15   |  20   |\n     24     |  15   |  21   |\n", _erModel->getConnectionTable());
	EXPECT_EQ(" Engineer |  PK(Emp_ID, Name), Department, FK(PC_ID)\n PC       |  PK(PC_ID), Purchase_Date\n Work Diary|  PK(WD_ID), Content, WD_date\n", _erModel->getERDiagramTable());

	// Assert "Work Diary" has the primary key "WD_ID" 
	for (int i = 0; i < _erModel->searchRelatedComponent(15).size(); i++)
	{
		if (_erModel->searchRelatedComponent(15)[i]->getID() == 20)
		{
			if (static_cast<NodeAttribute*>(_erModel->searchRelatedComponent(15)[i])->getIsPrimaryKey())
			{	
				checkPK = true;
				break;
			}
		}
	}
	EXPECT_TRUE(checkPK);
	checkPK = false;

	// Redo
	_erModel->redoCmd();

	// Assert the Entity "Work Diary" does not exist
	EXPECT_EQ(NULL, _erModel->searchComponent(15));

	// Assert Engineer's primary key is "Name" and "Emp_ID"
	EXPECT_EQ(2, static_cast<NodeEntity*>(_erModel->searchComponent(0))->getPrimaryKey().size());
	EXPECT_EQ(1, static_cast<NodeEntity*>(_erModel->searchComponent(0))->getPrimaryKey()[0]);
	EXPECT_EQ(3, static_cast<NodeEntity*>(_erModel->searchComponent(0))->getPrimaryKey()[1]);
}
