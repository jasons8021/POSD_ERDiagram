#include "ConnectComponentsCmd.h"
#include <iostream>

ConnectComponentsCmd::ConnectComponentsCmd(ERModel* erModel, int sourceNodeID, int destinationNodeID, string text)
{
	_erModel = erModel;
	_sourceNodeID = sourceNodeID;
	_destinationNodeID = destinationNodeID;
	_text = text;
	_componentID = PARAMETER_NOVALUE;
}

ConnectComponentsCmd::~ConnectComponentsCmd()
{
}

void ConnectComponentsCmd::execute()
{
	//	執行connect後，取得加入進去的componentID
	if (_componentID == PARAMETER_NOVALUE)						// 不是Redo的情況
		_componentID = _erModel->addConnection(_sourceNodeID, _destinationNodeID, _text);
		
	else														// Redo的情況
	{
		_erModel->setComponentID(_erModel->getComponentID() + PARAMETER_NEXTCOMPONENTID);
		_componentID = _erModel->addConnection(_componentID, _sourceNodeID, _destinationNodeID, _text);
	}
}

void ConnectComponentsCmd::unexecute()
{
	_erModel->deleteFunction(_componentID);
	_erModel->setIsModify(false);
	_erModel->setComponentID(_erModel->getComponentID() + PARAMETER_PRECOMPONENTID);
}
