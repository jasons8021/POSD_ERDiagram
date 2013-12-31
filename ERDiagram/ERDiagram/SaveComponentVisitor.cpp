#include "SaveComponentVisitor.h"
#include "Toolkit.h"

SaveComponentVisitor::SaveComponentVisitor()
{
	_componentInfoString = TEXT_NULL;
	_connectionInfoString = TEXT_NULL;
	_primaryKeyInfoString = TEXT_NULL;
	_positionInfoString = TEXT_NULL;
}

SaveComponentVisitor::~SaveComponentVisitor()
{

}

// ���X��Attribute
void SaveComponentVisitor::visit( NodeAttribute* visitAttribute)
{
	_componentInfoString += visitAttribute->getType();
	_componentInfoString += COMMASPACE;
	_componentInfoString += visitAttribute->getText();
	_componentInfoString += ENDLINE;

	// �s��m
	_positionInfoString += Toolkit::integerToString(visitAttribute->getSx());
	_positionInfoString += SPACE;
	_positionInfoString += Toolkit::integerToString(visitAttribute->getSy());
	_positionInfoString += ENDLINE;
}

// ���X��Entity�A�ݭn�b�sPrimaryKey
void SaveComponentVisitor::visit( NodeEntity* visitEntity )
{
	// �sComponent
	_componentInfoString += visitEntity->getType();
	_componentInfoString += COMMASPACE;
	_componentInfoString += visitEntity->getText();
	_componentInfoString += ENDLINE;

	// �s��m
	_positionInfoString += Toolkit::integerToString(visitEntity->getSx());
	_positionInfoString += SPACE;
	_positionInfoString += Toolkit::integerToString(visitEntity->getSy());
	_positionInfoString += ENDLINE;

	// �sPK
	if (visitEntity->getPrimaryKey().size() > 0)
	{
		_primaryKeyInfoString += Toolkit::integerToString(visitEntity->getID());
		_primaryKeyInfoString += SPACE;

		for (int i = 0; i < visitEntity->getPrimaryKey().size() - 1; i++)
		{
			_primaryKeyInfoString += Toolkit::integerToString(visitEntity->getPrimaryKey()[i]);
			_primaryKeyInfoString += COMMA;
		}
		// �̫�@��PK
		_primaryKeyInfoString += Toolkit::integerToString(visitEntity->getPrimaryKey()[visitEntity->getPrimaryKey().size() - 1]);
	}

	_primaryKeyInfoString += ENDLINE;
}

// ���X��Relationship
void SaveComponentVisitor::visit( NodeRelationship* visitRelationship )
{
	_componentInfoString += visitRelationship->getType();
	_componentInfoString += COMMASPACE;
	_componentInfoString += visitRelationship->getText();
	_componentInfoString += ENDLINE;

	// �s��m
	_positionInfoString += Toolkit::integerToString(visitRelationship->getSx());
	_positionInfoString += SPACE;
	_positionInfoString += Toolkit::integerToString(visitRelationship->getSy());
	_positionInfoString += ENDLINE;
}

// ���X��Connector�A�ݭn�A�t�~�s�o��Connector�s�쨺���Component
void SaveComponentVisitor::visit( Connector* visitConnector )
{
	_componentInfoString += visitConnector->getType();

	if (visitConnector->getText() != TEXT_NULL)
	{
		_componentInfoString += COMMASPACE;
		_componentInfoString += visitConnector->getText();
	}
	_componentInfoString += ENDLINE;

	_connectionInfoString += Toolkit::integerToString(visitConnector->getID());
	_connectionInfoString += SPACE;
	_connectionInfoString += Toolkit::integerToString(static_cast<Connector*>(visitConnector)->getSourceNodeID());
	_connectionInfoString += COMMA;
	_connectionInfoString += Toolkit::integerToString(static_cast<Connector*>(visitConnector)->getDestinationNodeID());
	_connectionInfoString += ENDLINE;
}

string SaveComponentVisitor::getERDiagramComponent()
{
	return _componentInfoString + ENDLINE + _connectionInfoString + ENDLINE + _primaryKeyInfoString;
}

string SaveComponentVisitor::getPositionInfo()
{
	return _positionInfoString;
}
