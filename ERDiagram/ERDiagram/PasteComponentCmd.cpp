#include "PasteComponentCmd.h"

PasteComponentCmd::PasteComponentCmd( ERModel* erModel )
{
	_erModel = erModel;
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
		adjustedID = _erModel->addCloneComponent(clipboardComponent[i]->deepClone());
		_pasteComponentID.push_back(adjustedID);
	}
	_pasteComponentID = _erModel->arrangeConnectorFirst(_pasteComponentID);
	_erModel->notifyPasteComponent(_pasteComponentID);
}

void PasteComponentCmd::unexecute()
{
	for (int i = 0; i < _pasteComponentID.size(); i++)
		_erModel->deleteFunction(_pasteComponentID[i]);
}
