#include "PasteComponentCmd.h"

PasteComponentCmd::PasteComponentCmd( ERModel* erModel )
{
	_erModel = erModel;
	_isRedo = false;
}

PasteComponentCmd::~PasteComponentCmd()
{

}

void PasteComponentCmd::execute()
{
	vector<Component*> clipboardComponent = _erModel->getClipboard();
	int adjustedID;
	for (int i = 0; i < clipboardComponent.size(); i++)
	{
		// 從clipboard獲得CloneComponent後再做一次Clone，再丟給Model處理
		adjustedID = _erModel->addClone(clipboardComponent[i]->deepClone());

		if (adjustedID != PARAMETER_ISERROR)
		{
			// 記錄ID的變化
			_erModel->recordNewCloneComponentID(clipboardComponent[i]->getID(), adjustedID);

			// 第一次將貼上ID記錄下來，以便undo
			if (!_isRedo)
				_pasteComponentID.push_back(adjustedID);
		}
	}
	if (!_isRedo)
	{
		_pasteComponentID = _erModel->arrangeConnectorFirst(_pasteComponentID);
		_erModel->notifyPasteComponent(_pasteComponentID);
	}
	
}

void PasteComponentCmd::unexecute()
{
	_isRedo = true;
	for (int i = 0; i < _pasteComponentID.size(); i++)
	{
		_erModel->deleteFunction(_pasteComponentID[i]);
		_erModel->setComponentID(_erModel->getComponentID()-1);
	}
}
