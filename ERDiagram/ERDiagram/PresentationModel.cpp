#include "PresentationModel.h"

PresentationModel::PresentationModel( ERModel* erModel )
{
	_erModel = erModel;
}

PresentationModel::~PresentationModel()
{
}

// 提供給TextUI的addNodeCmd，會回傳加入是否成功
bool PresentationModel::addNodeCmd_TextUI( string type, string text, int sx, int sy )
{
	int beforeAddNodeSize;
	beforeAddNodeSize = _erModel->getComponentTableSize();

	addNodeCmd(type, text, sx, sy);
	
	if (_erModel->getComponentTableSize() > beforeAddNodeSize)
		return true;
	else
		return false;
}

// 提供給TextUI的addConnectionCmd，會回傳目前ConnectionTable的大小
string PresentationModel::addConnectionCmd_TextUI( int firstComponentID, int secondComponentID, string cardinality )
{
	// 如果要connection的兩個Component有問題，則回傳問題字串
	if(_erModel->getCheckConnectionStateMessage(firstComponentID,secondComponentID) != TEXT_CONNECTION_CANCONNECT)
		return _erModel->getCheckConnectionStateMessage(firstComponentID,secondComponentID);
	else
		_cmdManager.execute(new ConnectComponentsCmd(_erModel, firstComponentID, secondComponentID, cardinality));
		//_erModel->addConnectionCmd(firstComponentID, secondComponentID, cardinality);

 	return Toolkit::integerToString(_erModel->getConnectionTableSize());
}

// 回傳為ComponentTable的字串
string PresentationModel::displayComponentTable_TextUI()
{
	string result;

	if (_erModel->getComponentTableSize() > PARAMETER_ZEROCOMPONENT)
	{
		// ComponentTable format
		result += TEXT_ADDNEWNODE_TITLE;
		result += TEXT_ENDLINE;
		result += TEXT_DEMARCATION_COMPONENTTABLE;
		result += TEXT_ENDLINE;
		result += TEXT_COMPONENT_TABLEFORMAT;
		result += TEXT_ENDLINE;
		result += TEXT_DEMARCATIONTWO_CONPONENTTABLE;
		result += TEXT_ENDLINE;

		// ComponentTable contents string
		result += _erModel->getComponentsTable(PARAMETER_ALL);

 		// ComponentTable End
		result += TEXT_DEMARCATION_COMPONENTTABLE;
		result += TEXT_ENDLINE;
	}

	return result;
}

// 回傳為ConnectionTable的字串
string PresentationModel::displayConnectionTable_TextUI()
{
	string result;

	if (_erModel->getConnectionTableSize() > PARAMETER_ZEROCOMPONENT)
	{
 		// ConnectiontTable format
		result += TEXT_CONNECTION_TITLE;
		result += TEXT_ENDLINE;
		result += TEXT_DEMARCATION_CONNECTIONTABLE;
		result += TEXT_ENDLINE;
		result += TEXT_CONNECTION_TABLEFORMAT;
		result += TEXT_ENDLINE;
		result += TEXT_DEMARCATIONTWO_CONNECTIONTABLE;
		result += TEXT_ENDLINE;

		// ConnectiontTable contents string
		result += _erModel->getConnectionTable();

		// ConnectiontTable End
		result += TEXT_DEMARCATION_CONNECTIONTABLE;
		result += TEXT_ENDLINE;
	}

	return result;
}

// 回傳為顯示全部Entity的字串
string PresentationModel::displayEntityTable_TextUI()
{
	string result;
	result += TEXT_SETPRIMARYKEY_ENTITYTITLE;
	result += TEXT_ENDLINE;
	result += TEXT_DEMARCATION_COMPONENTTABLE;
	result += TEXT_ENDLINE;
	result += TEXT_COMPONENT_TABLEFORMAT;
	result += TEXT_ENDLINE;
	result += TEXT_DEMARCATIONTWO_CONPONENTTABLE;
	result += TEXT_ENDLINE;
	result += _erModel->getComponentsTable(PARAMETER_ENTITY);
	result += TEXT_DEMARCATION_COMPONENTTABLE;
	result += TEXT_ENDLINE;

	return result;
}

//////////////////////////////////////////////////////////////////////////
//					檢查輸入的節點ID是否正確的Function					//
//////////////////////////////////////////////////////////////////////////
int PresentationModel::getComponentTableSize()
{
	return _erModel->getComponentTableSize();
}

int PresentationModel::getConnectionTableSize()
{
	return _erModel->getConnectionTableSize();
}

// 判斷是否需要設定Cardinality
bool PresentationModel::checkSetCardinality(int firstComponentID, int secondComponentID)
{
	return _erModel->checkSetCardinality(firstComponentID, secondComponentID);
}

// 提供給TextUI的判斷該Component是否存在
string PresentationModel::searchComponent_TextUI( string searchID, string searchType )
{
	if(!(_erModel->searchComponentExist(searchID, searchType)))
		return TEXT_CONNECTION_ERRORNODE;
	else
		return searchID;
}

// 提供給TextUI的判斷該Entity是否存在或輸入是否為Entity
string PresentationModel::searchEntity_TextUI( string searchID )
{
	if (!_erModel->searchComponentExist(searchID, PARAMETER_ALL))					// 輸入ID不存在
	 	return TEXT_CONNECTION_ERRORNODE;
	else if (_erModel->searchComponentExist(searchID, PARAMETER_ENTITY))			// 找到輸入的Entity
	 	return searchID;
	else																			// 輸入的ID不是Entity
	 	return TEXT_NODENUMBEGIN + searchID + TEXT_SETPRIMARYKEY_ERRORMESSAGE;
}

// 顯示某個Entity所擁有的Attribute，回傳完Attribute所組成的字串
string PresentationModel::displayAttributeTable_TextUI( int entityID )
{
	string result;

	result += TEXT_SETPRIMARYKEY_ATTRIBUTEOFENTITY;
	result += Toolkit::integerToString(entityID);
	result += TEXT_SETPRIMARYKEY_ENDTEXT;
	result += TEXT_ENDLINE;

	// AttributeTable表格樣式
	result += TEXT_DEMARCATION_COMPONENTTABLE;
	result += TEXT_ENDLINE;
	result += TEXT_COMPONENT_TABLEFORMAT;
	result += TEXT_ENDLINE;
	result += TEXT_DEMARCATIONTWO_CONPONENTTABLE;
	result += TEXT_ENDLINE;

	// EntityTable內容
	result += _erModel->searchAttributeOfEntity(entityID);

	// ComponentTable結尾
	result += TEXT_DEMARCATION_COMPONENTTABLE;
	result += TEXT_ENDLINE;

	return result;
}

// 將輸入的PK切割
vector<int> PresentationModel::splitPrimaryKey( string primaryKeys )
{
	vector<string> splitText;
	vector<int> primaryKeySet;

	splitText = Toolkit::splitFunction(primaryKeys, COMMA);

	for (int i = 0; i < splitText.size(); i++)
		primaryKeySet.push_back(atoi(splitText[i].c_str()));

	return primaryKeySet;
}

// 檢查輸入的Attribute是否在某個Entity中
string PresentationModel::checkAttributeInEntity_TextUI( string entityID, vector<int> primaryKeys )
{
	string errorMessage;

	for (int i = 0; i < primaryKeys.size(); i++)
	{
		if (!_erModel->searchComponentExist(Toolkit::integerToString(primaryKeys[i]), PARAMETER_ALL))					//	輸入的ID不存在.
			errorMessage += TEXT_CONNECTION_ERRORNODE;
		else if (!_erModel->searchEntityConnection(atoi(entityID.c_str()), primaryKeys[i], PARAMETER_ATTRIBUTE))		//	輸入的ID並非在該EntityNode中
			errorMessage += TEXT_NODENUMBEGIN + Toolkit::integerToString(primaryKeys[i]) + TEXT_SETPRIMARYKEY_ERRORATTRIBUTEID_ONE + entityID + TEXT_SETPRIMARYKEY_ERRORATTRIBUTEID_TWO;
	}

	return errorMessage;
}

// 設PK
string PresentationModel::setPrimaryKeys_TextUI( int entityID, vector<int> primaryKeys )
{
	string result;
	string attributeNodeIDSet;

	_erModel->setPrimaryKey(entityID, primaryKeys);

	// 顯示輸入的PK已經加入完成
	for (int i = 0; i < primaryKeys.size(); i++)
		attributeNodeIDSet += Toolkit::integerToString(primaryKeys[i]) + COMMA;
	attributeNodeIDSet = attributeNodeIDSet.substr(0, PARAMETER_ADJUSTPKSHOWSTRINGDOUBLESIZE * primaryKeys.size() + PARAMETER_ADJUSTPKSHOWSTRINGSUBLAST);
	
	result += TEXT_NODENUMBEGIN;
	result += Toolkit::integerToString(entityID);
	result += TEXT_SETPRIMARYKEY_SETPKFINISH_ONE;
	result += attributeNodeIDSet;
	result += TEXT_SETPRIMARYKEY_SETPKFINISH_TWO;

	return result;
}

// 回傳ER Table的字串
string PresentationModel::displayERDiagramTable_TextUI()
{
	string result;

	if(_erModel->checkOneToOne())
	{
	 	// ERDiagramTable 表格樣式
	 	result += TEXT_DEMARCATION_ERDIAGRAMTABLE;
		result += TEXT_ENDLINE;
	 	result += TEXT_ERDIAGRAM_TITLE;
		result += TEXT_ENDLINE;
	 	result += TEXT_DEMARCATIONTWO_ERDIAGRAMTABLE;
		result += TEXT_ENDLINE;

	 	//ERDiagramTable內容
	 	result += _erModel->getERDiagramTable();
	 
	 	// ComponentTable 結尾
	 	result += TEXT_DEMARCATION_ERDIAGRAMTABLE;
		result += TEXT_ENDLINE;
	}
	else
	{
		result += TEXT_ERDIAGRAM_NOTABLE;
		result += TEXT_ENDLINE;
	}

	return result;
}

bool PresentationModel::getIsModify()
{
	return _erModel->getIsModify();
}

void PresentationModel::saveERDiagram_TextUI( string fileName )
{
	_erModel->saveERDiagram(fileName);
}

string PresentationModel::loadERDiagram_TextUI( string fileName )
{
	string result;

	// 正確開啟：The ER diagram is displayed as follows:\n；錯誤開啟：File not found!!\n
	result += _erModel->loadERDiagram(fileName);
	if (result != TEXT_LOADSAVE_OPENFAILED)
	{
		result += displayComponentTable_TextUI();
		result += TEXT_ENDLINE;
		result += displayConnectionTable_TextUI();
	}
	return result;
}

// TextUI的刪除Component並回傳對應的字串
string PresentationModel::deleteComponent_TextUI( string delComponentID )
{
	string result;

	_cmdManager.execute(new DeleteComponentCmd(_erModel, atoi(delComponentID.c_str())));
	//_erModel->deleteCmd();

	result += TEXT_DELETE_DELETEFINISH_ONE;
	result += delComponentID;
	result += TEXT_DELETE_DELETEFINISH_TWO;
	result += TEXT_ENDLINE;

	result += displayComponentTable_TextUI();
	result += TEXT_ENDLINE;
	result += displayConnectionTable_TextUI();

	return result;
}

// TextUI的undo並回傳對應字串
string PresentationModel::undo_TextUI()
{
	string result;

	if (undoCmd())
	{
		result += TEXT_UNDO_SUCCESS;
		result += TEXT_ENDLINE;
		result += displayComponentTable_TextUI();
		result += TEXT_ENDLINE;
		result += displayConnectionTable_TextUI();
	}
	else
	{
		result += TEXT_UNDO_FAILED;
		result += TEXT_ENDLINE;
	}

	return result;
}

// TextUI的redo並回傳對應的字串
string PresentationModel::redo_TextUI()
{
	string result;

	if(redoCmd())
	{
		result += TEXT_REDO_SUCCESS;
		result += TEXT_ENDLINE;
		result += displayComponentTable_TextUI();
		result += TEXT_ENDLINE;
		result += displayConnectionTable_TextUI();
	}
	else
	{
		result += TEXT_REDO_FAILED;
		result += TEXT_ENDLINE;
	}

	return result;
}

// 回傳給GUI所需要的ComponentSet的字串
string PresentationModel::getComponent_GUI()
{
	return _erModel->getComponentForGUI();
}

// 回傳給GUI所需要的ConnectionSet的字串
string PresentationModel::getConnection_GUI()
{
	return _erModel->getConnectionForGUI();
}

// 回傳給GUI所需要的PK Set的字串
string PresentationModel::getPrimaryKey_GUI()
{
	return _erModel->getPrimaryKeyForGUI();
}

// 回傳是否GUI上面的Item是否可以連線
bool PresentationModel::addConnectionCmd_GUI( int firstComponentID, int secondComponentID, string cardinality )
{
	int beforeAddNodeSize;
	beforeAddNodeSize = _erModel->getConnectionTableSize();

	if(_erModel->getCheckConnectionStateMessage(firstComponentID,secondComponentID) != TEXT_CONNECTION_CANCONNECT)
		return false;
	else
		addConnectionCmd(firstComponentID, secondComponentID, cardinality);

	if (_erModel->getConnectionTableSize() > beforeAddNodeSize)
		return true;
	else
		return false;
}

// 增加觀察者
void PresentationModel::attachObserver( Observer* observer )
{
	_erModel->attachObserver(observer);
}

// 去除觀察者
void PresentationModel::detachObserver( Observer* observer )
{
	_erModel->detachObserver(observer);
}

// GUI改變Text
void PresentationModel::changeText( int targetNodeID, string editedText )
{
	_erModel->changeText(targetNodeID, editedText);
}

// GUI改變PK
void PresentationModel::changePrimaryKey( int targetNodeID, bool isPrimaryKey )
{
	_erModel->changePrimaryKey(targetNodeID, isPrimaryKey);
}

//////////////////////////////////////////////////////////////////////////
//							Command Pattern								//
//////////////////////////////////////////////////////////////////////////

void PresentationModel::addNodeCmd( string type, string text, int sx, int sy )
{
	_cmdManager.execute(new AddComponentCmd(_erModel, type, text, sx, sy));
}

void PresentationModel::addConnectionCmd( int sourceNodeID, int destinationNodeID, string text )
{
	_cmdManager.execute(new ConnectComponentsCmd(_erModel, sourceNodeID, destinationNodeID, text));
}

void PresentationModel::deleteCmd( int delComponentID )
{
	_cmdManager.execute(new DeleteComponentCmd(_erModel, delComponentID));
}
bool PresentationModel::undoCmd()
{
	return _cmdManager.undo();
}

bool PresentationModel::redoCmd()
{
	return _cmdManager.redo();
}
