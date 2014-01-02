#include "Subject.h"

Subject::Subject()
{
}

Subject::~Subject()
{
}

// ·s¼Wobserver
void Subject::attachObserver( Observer* observer )
{
	_observerList.push_back(observer);
}

// §R°£observer
void Subject::detachObserver( Observer* observer )
{
	for(int i = 0; i < _observerList.size(); i++)
	{
		if (observer == _observerList[i])
		{
			_observerList.erase(_observerList.begin()+i);
			break;
		}
	}
}

void Subject::notifyTextChanged( int targetNodeID, string editedText )
{
	for (int i = 0; i < _observerList.size(); i++)
	{
		_observerList[i]->updateTextChanged(targetNodeID, editedText);
	}
}

void Subject::notifyPrimaryKeyChanged( int targetNodeID, bool isPrimaryKey )
{
	for (int i = 0; i < _observerList.size(); i++)
	{
		_observerList[i]->updatePrimaryKeyChanged(targetNodeID, isPrimaryKey);
	}
}

void Subject::notifyAddNewNode( int componentID, string type, string text, int sx, int sy )
{
	for (int i = 0; i < _observerList.size(); i++)
	{
		_observerList[i]->updateAddNewNode(componentID, type, text, sx, sy);
	}
}

void Subject::notifyNewConnection( int componentID, int sourceNodeID, int destinationNodeID, string cardinality, vector<pair<int, bool>> changePK )
{
	for (int i = 0; i < _observerList.size(); i++)
	{
		_observerList[i]->updateConnection(componentID, sourceNodeID, destinationNodeID, cardinality, changePK);
	}
}

void Subject::notifyDeleteComplete( string deleteComponentIDSet )
{
	for (int i = 0; i < _observerList.size(); i++)
	{
		_observerList[i]->updateDeleteComplete(deleteComponentIDSet);
	}
}

void Subject::notifyReBuildConnection( string relatedConnectionSet )
{
	for (int i = 0; i < _observerList.size(); i++)
	{
		_observerList[i]->updateReBuildConnection(relatedConnectionSet);
	}
}

void Subject::notifyPasteComponent( vector<int> pasteComponentIDSet )
{
	for (int i = 0; i < _observerList.size(); i++)
	{
		_observerList[i]->updatePasteComponent(pasteComponentIDSet);
	}
}

void Subject::notifyNewItemPosition( int componentID, int newSx, int newSy )
{
	for (int i = 0; i < _observerList.size(); i++)
	{
		_observerList[i]->updateItemPosition(componentID, newSx, newSy);
	}
}
