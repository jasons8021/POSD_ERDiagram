#include "DeleteGroupCmd.h"

DeleteGroupCmd::DeleteGroupCmd( ERModel* erModel, vector<int> componentIDSet )
{
	_erModel = erModel;
	_componentIDSet = componentIDSet;

	for (int i = 0; i < _componentIDSet.size(); i++)
		_deleteCommandSet.push_back(new DeleteComponentCmd(_erModel, _componentIDSet[i]));
}

DeleteGroupCmd::~DeleteGroupCmd()
{

}

void DeleteGroupCmd::execute()
{
	for (int i = 0; i < _deleteCommandSet.size(); i++)
		_deleteCommandSet[i]->execute();
}

void DeleteGroupCmd::unexecute()
{
	for (int i = _deleteCommandSet.size() - 1; i >= 0; i--)
		_deleteCommandSet[i]->unexecute();
}
