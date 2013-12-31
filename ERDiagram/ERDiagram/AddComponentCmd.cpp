#include "AddComponentCmd.h"

AddComponentCmd::AddComponentCmd(ERModel* erModel, string type, string text, int sx, int sy)
{
	_erModel = erModel;
	_type = type;
	_text = text;
	_sx = sx;
	_sy = sy;
	_componentID = PARAMETER_NOVALUE;
}

AddComponentCmd::~AddComponentCmd()
{
}

void AddComponentCmd::execute()
{
	//	執行addNode後，取得加入進去的componentID
	if (_componentID == PARAMETER_NOVALUE)		// 不是Redo的情況
		_componentID = _erModel->addNode(_type, _text, _sx, _sy);
	else										// Redo的情況
	{	
		_componentID = _erModel->addNode(_componentID, _type, _text, _sx, _sy);
		_erModel->setComponentID(_erModel->getComponentID() + PARAMETER_NEXTCOMPONENTID);		//	因為redo的關係，所以componentID+1
	}

	// NotifyAddNewNode
	_erModel->notifyAddNewNode(_componentID, _type, _text, _sx, _sy);
}

void AddComponentCmd::unexecute()
{
	_erModel->deleteFunction(_componentID);
	_erModel->setIsModify(false);
	_erModel->setComponentID(_erModel->getComponentID() + PARAMETER_PRECOMPONENTID);			//	undo的關係，會少一個component，所以componentID-1
}