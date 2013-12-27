#include "MoveItemCmd.h"

MoveItemCmd::MoveItemCmd( ERModel* erModel, int componentID, int newSx, int newSy )
{
	_erModel = erModel;
	_componentID = componentID;
	_newSx = newSx;
	_newSy = newSy;

	Component* movedComponent = _erModel->searchComponent(_componentID);
	_originalSx = movedComponent->getSx();
	_originalSy = movedComponent->getSy();
}

MoveItemCmd::~MoveItemCmd()
{
}

void MoveItemCmd::execute()
{
	_erModel->movedComponentPosition(_componentID, _newSx, _newSy);
	_erModel->notifyNewItemPosition(_componentID, _newSx, _newSy);
}

void MoveItemCmd::unexecute()
{
	_erModel->movedComponentPosition(_componentID, _originalSx, _originalSy);
	_erModel->notifyNewItemPosition(_componentID, _originalSx, _originalSy);
}
