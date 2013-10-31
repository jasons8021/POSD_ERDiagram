#include "PresentationModel.h"

PresentationModel::PresentationModel( ERModel* erModel )
{
	_erModel = erModel;
}

PresentationModel::~PresentationModel()
{
}

bool PresentationModel::addNodeCmd( string type, string text )
{
	int beforeAddNodeSize;
	beforeAddNodeSize = _erModel->getComponentTableSize();

	_erModel->addNodeCmd(type, text);

	if (_erModel->getComponentTableSize() > beforeAddNodeSize)
		return true;
	else
		return false;
}

string PresentationModel::addConnectionCmd( int firstComponentID, int secondComponentID, string cardinality )
{
	// 如果要connection的兩個Component有問題，則回傳問題字串
	if(_erModel->getCheckConnectionStateMessage(firstComponentID,secondComponentID) != TEXT_CONNECTION_CANCONNECT)
		return _erModel->getCheckConnectionStateMessage(firstComponentID,secondComponentID);
	else
		_erModel->addConnectionCmd(firstComponentID, secondComponentID, cardinality);

 	return Toolkit::integerToString(_erModel->getConnectionTableSize());
}

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

bool PresentationModel::checkSetCardinality(int firstComponentID, int secondComponentID)
{
	return _erModel->checkSetCardinality(firstComponentID, secondComponentID);
}

string PresentationModel::searchComponent_TextUI( string searchID, string searchType )
{
	if(!(_erModel->searchComponentExist(searchID, searchType)))
		return TEXT_CONNECTION_ERRORNODE;
	else
		return searchID;
}

string PresentationModel::searchEntity_TextUI( string searchID )
{
	if (!_erModel->searchComponentExist(searchID, PARAMETER_ALL))					// 輸入ID不存在
	 	return TEXT_CONNECTION_ERRORNODE;
	else if (_erModel->searchComponentExist(searchID, PARAMETER_ENTITY))			// 找到輸入的Entity
	 	return searchID;
	else																			// 輸入的ID不是Entity
	 	return TEXT_NODENUMBEGIN + searchID + TEXT_SETPRIMARYKEY_ERRORMESSAGE;
}

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

vector<int> PresentationModel::splitPrimaryKey( string primaryKeys )
{
	vector<string> splitText;
	vector<int> primaryKeySet;

	splitText = Toolkit::splitFunction(primaryKeys, COMMA);

	for (int i = 0; i < splitText.size(); i++)
		primaryKeySet.push_back(atoi(splitText[i].c_str()));

	return primaryKeySet;
}

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

string PresentationModel::deleteComponent_TextUI( string delComponentID )
{
	string result;

	_erModel->deleteCmd(atoi(delComponentID.c_str()));

	result += TEXT_DELETE_DELETEFINISH_ONE;
	result += delComponentID;
	result += TEXT_DELETE_DELETEFINISH_TWO;
	result += TEXT_ENDLINE;

	result += displayComponentTable_TextUI();
	result += TEXT_ENDLINE;
	result += displayConnectionTable_TextUI();

	return result;
}

string PresentationModel::undo_TextUI()
{
	string result;

	if (_erModel->undoCmd())
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

string PresentationModel::redo_TextUI()
{
	string result;

	if(_erModel->redoCmd())
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
