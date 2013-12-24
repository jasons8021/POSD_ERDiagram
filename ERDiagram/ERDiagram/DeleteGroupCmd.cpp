#include "DeleteGroupCmd.h"

DeleteGroupCmd::DeleteGroupCmd( ERModel* erModel, vector<int> componentIDSet )
{
	_erModel = erModel;

	// ��Connector�Ԩ�e���ANode(Attribute, Entity, Relationship)���᭱
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

// �NcomponentIDSet��Connector��X�ӡA���̫e��
void DeleteGroupCmd::arrangeIDSet( vector<int> componentIDSet )
{
	vector<int> resultSet;
	vector<int> nodeIDSet;

	// �Ϥ�Connector�P��LNode(Attribute, Entity, Relationship)
	for (int i = 0; i < componentIDSet.size(); i++)
	{
		Component* tempComponent = _erModel->searchComponent(componentIDSet[i]);
		if (tempComponent->getType() != PARAMETER_CONNECTOR)	// ��Attribute, Entity, Relationship�PConnector����
			nodeIDSet.push_back(componentIDSet[i]);
		else													// ����JConnector
			resultSet.push_back(componentIDSet[i]);
	}

	// �X��
	for (int i = 0; i < nodeIDSet.size(); i++)
		resultSet.push_back(nodeIDSet[i]);

	_componentIDSet = resultSet;
}
