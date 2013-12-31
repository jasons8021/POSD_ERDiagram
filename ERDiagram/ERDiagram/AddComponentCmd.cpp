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
	//	����addNode��A���o�[�J�i�h��componentID
	if (_componentID == PARAMETER_NOVALUE)		// ���ORedo�����p
		_componentID = _erModel->addNode(_type, _text, _sx, _sy);
	else										// Redo�����p
	{	
		_componentID = _erModel->addNode(_componentID, _type, _text, _sx, _sy);
		_erModel->setComponentID(_erModel->getComponentID() + PARAMETER_NEXTCOMPONENTID);		//	�]��redo�����Y�A�ҥHcomponentID+1
	}

	// NotifyAddNewNode
	_erModel->notifyAddNewNode(_componentID, _type, _text, _sx, _sy);
}

void AddComponentCmd::unexecute()
{
	_erModel->deleteFunction(_componentID);
	_erModel->setIsModify(false);
	_erModel->setComponentID(_erModel->getComponentID() + PARAMETER_PRECOMPONENTID);			//	undo�����Y�A�|�֤@��component�A�ҥHcomponentID-1
}