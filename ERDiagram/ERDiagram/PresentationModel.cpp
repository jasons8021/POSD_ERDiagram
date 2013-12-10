#include "PresentationModel.h"

PresentationModel::PresentationModel( ERModel* erModel )
{
	_erModel = erModel;
}

PresentationModel::~PresentationModel()
{
}

// ���ѵ�TextUI��addNodeCmd�A�|�^�ǥ[�J�O�_���\
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

// ���ѵ�TextUI��addConnectionCmd�A�|�^�ǥثeConnectionTable���j�p
string PresentationModel::addConnectionCmd_TextUI( int firstComponentID, int secondComponentID, string cardinality )
{
	// �p�G�nconnection�����Component�����D�A�h�^�ǰ��D�r��
	if(_erModel->getCheckConnectionStateMessage(firstComponentID,secondComponentID) != TEXT_CONNECTION_CANCONNECT)
		return _erModel->getCheckConnectionStateMessage(firstComponentID,secondComponentID);
	else
		_cmdManager.execute(new ConnectComponentsCmd(_erModel, firstComponentID, secondComponentID, cardinality));
		//_erModel->addConnectionCmd(firstComponentID, secondComponentID, cardinality);

 	return Toolkit::integerToString(_erModel->getConnectionTableSize());
}

// �^�Ǭ�ComponentTable���r��
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

// �^�Ǭ�ConnectionTable���r��
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

// �^�Ǭ���ܥ���Entity���r��
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
//					�ˬd��J���`�IID�O�_���T��Function					//
//////////////////////////////////////////////////////////////////////////
int PresentationModel::getComponentTableSize()
{
	return _erModel->getComponentTableSize();
}

int PresentationModel::getConnectionTableSize()
{
	return _erModel->getConnectionTableSize();
}

// �P�_�O�_�ݭn�]�wCardinality
bool PresentationModel::checkSetCardinality(int firstComponentID, int secondComponentID)
{
	return _erModel->checkSetCardinality(firstComponentID, secondComponentID);
}

// ���ѵ�TextUI���P�_��Component�O�_�s�b
string PresentationModel::searchComponent_TextUI( string searchID, string searchType )
{
	if(!(_erModel->searchComponentExist(searchID, searchType)))
		return TEXT_CONNECTION_ERRORNODE;
	else
		return searchID;
}

// ���ѵ�TextUI���P�_��Entity�O�_�s�b�ο�J�O�_��Entity
string PresentationModel::searchEntity_TextUI( string searchID )
{
	if (!_erModel->searchComponentExist(searchID, PARAMETER_ALL))					// ��JID���s�b
	 	return TEXT_CONNECTION_ERRORNODE;
	else if (_erModel->searchComponentExist(searchID, PARAMETER_ENTITY))			// ����J��Entity
	 	return searchID;
	else																			// ��J��ID���OEntity
	 	return TEXT_NODENUMBEGIN + searchID + TEXT_SETPRIMARYKEY_ERRORMESSAGE;
}

// ��ܬY��Entity�Ҿ֦���Attribute�A�^�ǧ�Attribute�Ҳզ����r��
string PresentationModel::displayAttributeTable_TextUI( int entityID )
{
	string result;

	result += TEXT_SETPRIMARYKEY_ATTRIBUTEOFENTITY;
	result += Toolkit::integerToString(entityID);
	result += TEXT_SETPRIMARYKEY_ENDTEXT;
	result += TEXT_ENDLINE;

	// AttributeTable���˦�
	result += TEXT_DEMARCATION_COMPONENTTABLE;
	result += TEXT_ENDLINE;
	result += TEXT_COMPONENT_TABLEFORMAT;
	result += TEXT_ENDLINE;
	result += TEXT_DEMARCATIONTWO_CONPONENTTABLE;
	result += TEXT_ENDLINE;

	// EntityTable���e
	result += _erModel->searchAttributeOfEntity(entityID);

	// ComponentTable����
	result += TEXT_DEMARCATION_COMPONENTTABLE;
	result += TEXT_ENDLINE;

	return result;
}

// �N��J��PK����
vector<int> PresentationModel::splitPrimaryKey( string primaryKeys )
{
	vector<string> splitText;
	vector<int> primaryKeySet;

	splitText = Toolkit::splitFunction(primaryKeys, COMMA);

	for (int i = 0; i < splitText.size(); i++)
		primaryKeySet.push_back(atoi(splitText[i].c_str()));

	return primaryKeySet;
}

// �ˬd��J��Attribute�O�_�b�Y��Entity��
string PresentationModel::checkAttributeInEntity_TextUI( string entityID, vector<int> primaryKeys )
{
	string errorMessage;

	for (int i = 0; i < primaryKeys.size(); i++)
	{
		if (!_erModel->searchComponentExist(Toolkit::integerToString(primaryKeys[i]), PARAMETER_ALL))					//	��J��ID���s�b.
			errorMessage += TEXT_CONNECTION_ERRORNODE;
		else if (!_erModel->searchEntityConnection(atoi(entityID.c_str()), primaryKeys[i], PARAMETER_ATTRIBUTE))		//	��J��ID�ëD�b��EntityNode��
			errorMessage += TEXT_NODENUMBEGIN + Toolkit::integerToString(primaryKeys[i]) + TEXT_SETPRIMARYKEY_ERRORATTRIBUTEID_ONE + entityID + TEXT_SETPRIMARYKEY_ERRORATTRIBUTEID_TWO;
	}

	return errorMessage;
}

// �]PK
string PresentationModel::setPrimaryKeys_TextUI( int entityID, vector<int> primaryKeys )
{
	string result;
	string attributeNodeIDSet;

	_erModel->setPrimaryKey(entityID, primaryKeys);

	// ��ܿ�J��PK�w�g�[�J����
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

// �^��ER Table���r��
string PresentationModel::displayERDiagramTable_TextUI()
{
	string result;

	if(_erModel->checkOneToOne())
	{
	 	// ERDiagramTable ���˦�
	 	result += TEXT_DEMARCATION_ERDIAGRAMTABLE;
		result += TEXT_ENDLINE;
	 	result += TEXT_ERDIAGRAM_TITLE;
		result += TEXT_ENDLINE;
	 	result += TEXT_DEMARCATIONTWO_ERDIAGRAMTABLE;
		result += TEXT_ENDLINE;

	 	//ERDiagramTable���e
	 	result += _erModel->getERDiagramTable();
	 
	 	// ComponentTable ����
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

	// ���T�}�ҡGThe ER diagram is displayed as follows:\n�F���~�}�ҡGFile not found!!\n
	result += _erModel->loadERDiagram(fileName);
	if (result != TEXT_LOADSAVE_OPENFAILED)
	{
		result += displayComponentTable_TextUI();
		result += TEXT_ENDLINE;
		result += displayConnectionTable_TextUI();
	}
	return result;
}

// TextUI���R��Component�æ^�ǹ������r��
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

// TextUI��undo�æ^�ǹ����r��
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

// TextUI��redo�æ^�ǹ������r��
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

// �^�ǵ�GUI�һݭn��ComponentSet���r��
string PresentationModel::getComponent_GUI()
{
	return _erModel->getComponentForGUI();
}

// �^�ǵ�GUI�һݭn��ConnectionSet���r��
string PresentationModel::getConnection_GUI()
{
	return _erModel->getConnectionForGUI();
}

// �^�ǵ�GUI�һݭn��PK Set���r��
string PresentationModel::getPrimaryKey_GUI()
{
	return _erModel->getPrimaryKeyForGUI();
}

// �^�ǬO�_GUI�W����Item�O�_�i�H�s�u
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

// �W�[�[���
void PresentationModel::attachObserver( Observer* observer )
{
	_erModel->attachObserver(observer);
}

// �h���[���
void PresentationModel::detachObserver( Observer* observer )
{
	_erModel->detachObserver(observer);
}

// GUI����Text
void PresentationModel::changeText( int targetNodeID, string editedText )
{
	_erModel->changeText(targetNodeID, editedText);
}

// GUI����PK
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
