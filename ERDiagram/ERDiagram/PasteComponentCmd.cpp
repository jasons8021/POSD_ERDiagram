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
		// �qclipboard��oCloneComponent��A���@��Clone�A�A�ᵹModel�B�z
		adjustedID = _erModel->addClone(clipboardComponent[i]->deepClone());

		if (adjustedID != PARAMETER_ISERROR)
		{
			// �O��ID���ܤ�
			_erModel->recordNewCloneComponentID(clipboardComponent[i]->getID(), adjustedID);

			// �Ĥ@���N�K�WID�O���U�ӡA�H�Kundo
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
