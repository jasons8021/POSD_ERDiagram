#include "ChangeTextCmd.h"


ChangeTextCmd::ChangeTextCmd( ERModel* erModel, int componentID, string editedText )
{
	_erModel = erModel;
	_componentID = componentID;
	_textAfterChanged = editedText;

	Component* editedTextComponent = _erModel->searchComponent(_componentID);
	_textBeforeChanged = editedTextComponent->getText();
}


ChangeTextCmd::~ChangeTextCmd(void)
{
}

void ChangeTextCmd::execute()
{
	_erModel->changeText(_componentID, _textAfterChanged);
	_erModel->notifyTextChanged(_componentID, _textAfterChanged);
}

void ChangeTextCmd::unexecute()
{
	_erModel->changeText(_componentID, _textBeforeChanged);
	_erModel->notifyTextChanged(_componentID, _textBeforeChanged);
}
