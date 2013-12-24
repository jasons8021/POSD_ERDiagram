#include "DeleteGroupCmd.h"

DeleteGroupCmd::DeleteGroupCmd( ERModel* erModel, vector<int> componentIDSet )
{
	_erModel = erModel;

	// 把Connector拉到前面，Node(Attribute, Entity, Relationship)放到後面
	arrangeIDSet(componentIDSet);

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

// 將componentIDSet的Connector抽出來，放到最前面
void DeleteGroupCmd::arrangeIDSet( vector<int> componentIDSet )
{
	vector<int> resultSet;
	vector<int> nodeIDSet;

	// 區分Connector與其他Node(Attribute, Entity, Relationship)
	for (int i = 0; i < componentIDSet.size(); i++)
	{
		Component* tempComponent = _erModel->searchComponent(componentIDSet[i]);
		if (tempComponent->getType() != PARAMETER_CONNECTOR)	// 把Attribute, Entity, Relationship與Connector分離
			nodeIDSet.push_back(componentIDSet[i]);
		else													// 先放入Connector
			resultSet.push_back(componentIDSet[i]);
	}

	// 合併
	for (int i = 0; i < nodeIDSet.size(); i++)
		resultSet.push_back(nodeIDSet[i]);

	_componentIDSet = resultSet;
}
