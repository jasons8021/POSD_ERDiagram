#include "ChangePrimaryKeyCmd.h"

ChangePrimaryKeyCmd::ChangePrimaryKeyCmd(ERModel* erModel, int componentID, bool isPK)
{
	_erModel = erModel;
	_componentID = componentID;
	_isPK = isPK;
}

ChangePrimaryKeyCmd::~ChangePrimaryKeyCmd()
{
}

void ChangePrimaryKeyCmd::execute()
{
	_erModel->changePrimaryKey(_componentID, _isPK);
	_erModel->notifyPrimaryKeyChanged(_componentID, _isPK);
}

void ChangePrimaryKeyCmd::unexecute()
{
	_erModel->changePrimaryKey(_componentID, !_isPK);
	_erModel->notifyPrimaryKeyChanged(_componentID, !_isPK);
}
