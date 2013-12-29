#include "MoveComponentCmd.h"

MoveComponentCmd::MoveComponentCmd( ERModel* erModel, vector<int> componentID, int moveDistance_x, int moveDistance_y )
{
	_erModel = erModel;
	_isRedo = false;
	_componentID = componentID;
	_moveDistance_x = moveDistance_x;
	_moveDistance_y = moveDistance_y;
}

MoveComponentCmd::~MoveComponentCmd()
{
}

void MoveComponentCmd::execute()
{
	for (int i = 0; i < _componentID.size(); i++)
	{
		_erModel->movedComponentPosition(_componentID[i], _moveDistance_x, _moveDistance_y);
		// �Ĥ@�����ʥ�QT���ز��ʡA�ҥH�i�H���γq���A���᪺Redo�A�h�q��
		if (_isRedo)
			_erModel->notifyNewItemPosition(_componentID[i], _moveDistance_x, _moveDistance_y);
	}
}

void MoveComponentCmd::unexecute()
{
	_isRedo = true;
	for (int i = 0; i < _componentID.size(); i++)
	{
		_erModel->movedComponentPosition(_componentID[i], MINUS * _moveDistance_x, MINUS * _moveDistance_y);
		_erModel->notifyNewItemPosition(_componentID[i], MINUS * _moveDistance_x, MINUS * _moveDistance_y);
	}
}
