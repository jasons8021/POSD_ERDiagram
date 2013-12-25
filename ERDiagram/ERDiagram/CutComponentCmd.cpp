#include "CutComponentCmd.h"

CutComponentCmd::CutComponentCmd( ERModel* erModel, vector<int> cutComponentIDSet )
{
	_erModel = erModel;
	_cutComponentIDSet = cutComponentIDSet;
	_deleteGroupCmd = new DeleteGroupCmd(_erModel, _cutComponentIDSet);
	_isRedo = false;
}

CutComponentCmd::~CutComponentCmd()
{
}

void CutComponentCmd::execute()
{
	if (!_isRedo)
	{
		_erModel->resetClipboard();
		_erModel->cloneItemIntoClipboard(_cutComponentIDSet);
	}

	_deleteGroupCmd->execute();
}

void CutComponentCmd::unexecute()
{
	_isRedo = true;
	_deleteGroupCmd->unexecute();
}
