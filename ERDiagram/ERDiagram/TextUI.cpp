#include "TextUI.h"

TextUI::TextUI(PresentationModel* presentationModel)
{
	_presentationModel = presentationModel;
}

TextUI::~TextUI()
{
}

void TextUI::displayMenu()
{
	cout << TEXT_MENU;
	processCommand();
}

void TextUI::processCommand()
{
	string choice;

	cin >> choice;
	switch(atoi(choice.c_str()))
	{
	case Load:
		loadERDiagram();
		displayMenu();
		break;
	case Save:
		saveERDiagram();
		displayMenu();
		break;
	case Add:
		addNewNode();
		displayMenu();
		break;
	case Connect:
		addConnection();
		displayMenu();
		break;
	case GetTable:
		displayComponentTable();
		displayConnectionTable();
		displayMenu();
		break;
	case SetPK:
		setPrimaryKey();
		displayMenu();
		break;
	case GetERTable:
		displayERDiagramTable();
		displayMenu();
		break;
	case Delete:
		deleteComponent();
		displayMenu();
		break;
	case Undo:
		undoCmd();
		displayMenu();
		break;
	case Redo:
		redoCmd();
		displayMenu();
		break;
	case Exit:
		exitERDiagram();
		break;
	default:
		cout << TEXT_MENU_ERRORCHOICE;
		displayMenu();
		break;
	}
}

void TextUI::addNewNode()
{
	string type;
	string text;

	cout << TEXT_ADDNEWNODE_TYPE;
	cin >> type;

	while ( !(type == PARAMETER_ATTRIBUTE || type == PARAMETER_ENTITY || type == PARAMETER_RELATIONSHIP) )
	{
		cout << TEXT_ADDNEWNODE_ERRORTYPE;
		cin >> type;
	}

	cout << TEXT_ADDNEWNODE_NAME;
	cin >> text;

	if (_presentationModel->addNodeCmd(type, text))
		cout << _presentationModel->displayComponentTable_TextUI() << endl;
}

void TextUI::addConnection()
{
	int firstComponentID;
	int secondComponentID;
	bool checkCardinality;
	string cardinality = PARAMETER_NULL;

	if (_presentationModel->getComponentTableSize() > PARAMETER_ZEROCOMPONENT)
	{
		// 第一個Component
		cout << TEXT_CONNECTION_FIRSTNODE;
		firstComponentID = atoi(searchComponent(PARAMETER_ALL).c_str());

		// 第二個Component
		cout << TEXT_CONNECTION_SECONDNODE;
		secondComponentID = atoi(searchComponent(PARAMETER_ALL).c_str());

		// 如果是Relationship跟Entity互相連接，輸入Cardinality
		if (_presentationModel->checkSetCardinality(firstComponentID, secondComponentID))
			cardinality = chooseCardinality();

		// 如果要connection的兩個Component有問題，則回傳問題字串
		cout << _presentationModel->addConnectionCmd(firstComponentID, secondComponentID, cardinality) << endl;

		cout << _presentationModel->displayConnectionTable_TextUI();
	}
	else
		cout << TEXT_CONNECTION_NOCOMPONENT << endl;
	
}

string TextUI::chooseCardinality()
{
	string cardinalityOption;

	cout << TEXT_CONNECTION_CARDINALITY << endl;
	cout << TEXT_CONNECTION_CARDINALITYOPTION;
	cin >> cardinalityOption;

	switch(atoi(cardinalityOption.c_str()))
	{
	case CardinalityOptionOne:
		return PARAMETER_CARDINALITYOPTION_ONE;
	case CardinalityOptionTwo:
		return PARAMETER_CARDINALITYOPTION_TWO;
	default:
		break;
	}
	return PARAMETER_NULL;
}

void TextUI::displayComponentTable()
{
	cout << _presentationModel->displayComponentTable_TextUI() << endl;
}

void TextUI::displayConnectionTable()
{
	cout << _presentationModel->displayConnectionTable_TextUI() << endl;
}

void TextUI::setPrimaryKey()
{
	string entityNodeID;
	vector<int> primaryKeys;

	cout << _presentationModel->displayEntityTable_TextUI() << endl;

	// 輸入Entity
	cout << TEXT_SETPRIMARYKEY_ENTERNODEID;
	entityNodeID = searchEntity(PARAMETER_ENTITY);

	// 顯示該Entity擁有的Attribute Component
	cout << _presentationModel->displayAttributeTable_TextUI(atoi(entityNodeID.c_str())) << endl;
	
	// 輸入primary keys 並檢查正確
	primaryKeys = searchAttribute(entityNodeID);

	cout << _presentationModel->setPrimaryKeys_TextUI(atoi(entityNodeID.c_str()), primaryKeys) << endl;
}

void TextUI::displayERDiagramTable()
{
	cout << _presentationModel->displayERDiagramTable_TextUI() << endl;
}

void TextUI::exitERDiagram()
{
	string choice;

	if (_presentationModel->getIsModify())
	{
		cout << TEXT_LOADSVAE_EXITPROGRAM << endl;
		cout << TEXT_LOADSVAE_SAVEORNOT << endl;
		cout << GETINPUT;
		cin >> choice;
		switch(atoi(choice.c_str()))
		{
		case PARAMETER_SAVEEXIT:
			saveERDiagram();
			cout << TEXT_GOODBYE;
			break;
		case PARAMETER_NOSAVEEXIT:
			cout << TEXT_GOODBYE;
			break;
		}
	}
	else
		cout << TEXT_GOODBYE;
}

void TextUI::saveERDiagram()
{
	string fileName;
	cout << TEXT_LOADSAVE_FILENAME;
	cin >> fileName;

	_presentationModel->saveERDiagram_TextUI(fileName);
}

void TextUI::loadERDiagram()
{
	string fileName;
	cout << TEXT_LOADSAVE_FILENAME;
	cin >> fileName;

	cout << _presentationModel->loadERDiagram_TextUI(fileName) << endl;
}

void TextUI::deleteComponent()
{
	string delComponentID;
	cout << TEXT_DELETE_ENTERNODE;
	delComponentID = searchComponent(PARAMETER_ALL);
	
	cout << _presentationModel->deleteComponent_TextUI(delComponentID) << endl;
}

void TextUI::undoCmd()
{
	cout << _presentationModel->undo_TextUI() << endl;
}

void TextUI::redoCmd()
{
	cout << _presentationModel->redo_TextUI() << endl;
}

//////////////////////////////////////////////////////////////////////////
//					檢查輸入的節點ID是否正確的Function					//
//////////////////////////////////////////////////////////////////////////

string TextUI::searchComponent( string searchType )
{
	string searchID;
	string result;

	cin >> searchID;

	// 確認輸入的node是否正確
	result = _presentationModel->searchComponent_TextUI(searchID, searchType);
	while(result == TEXT_CONNECTION_ERRORNODE){
		cout << result;
		cin >> searchID;
		result = _presentationModel->searchComponent_TextUI(searchID, searchType);
	}

	return searchID;
}

string TextUI::searchEntity( string searchType )
{
 	string searchID;
	string result;

 	cin >> searchID;
	result = _presentationModel->searchEntity_TextUI(searchID);
	while(result == (TEXT_NODENUMBEGIN + searchID + TEXT_SETPRIMARYKEY_ERRORMESSAGE) || result == TEXT_CONNECTION_ERRORNODE)
	{
		cout << result;
		cin >> searchID;
		result = _presentationModel->searchEntity_TextUI(searchID);
	}

 	return searchID;
}

vector<int> TextUI::searchAttribute( string entityNodeID )
{
	string attributeNodeID;
	string errorMessage = PARAMETER_NULL;
 	vector<int> primaryKeys;

	cout << TEXT_SETPRIMARYKEY_ENTERTWOATTRIBUTE;
 	cin >> attributeNodeID;
 	primaryKeys = _presentationModel->splitPrimaryKey(attributeNodeID);

	errorMessage = _presentationModel->checkAttributeInEntity_TextUI(entityNodeID, primaryKeys);
	while( errorMessage != PARAMETER_NULL)
	{
		// errorMessage用來回報Primary Key的集合是否有問題
		cout << errorMessage;

		//	重新輸入新的Primary Key
		primaryKeys.clear();
		cin >> attributeNodeID;
		primaryKeys = _presentationModel->splitPrimaryKey(attributeNodeID);
		errorMessage = _presentationModel->checkAttributeInEntity_TextUI(entityNodeID, primaryKeys);
	}

 	return primaryKeys;
}
