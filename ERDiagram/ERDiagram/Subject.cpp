#include "Subject.h"

Subject::Subject()
{
}

Subject::~Subject()
{
}

// �s�Wobserver
void Subject::attachObserver( Observer* observer )
{
	_observerList.push_back(observer);
}

// �R��observer
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

// �q���C��observer
// void Subject::notifyObserver()
// {
// 	for (int i = 0; i < _observerList.size(); i++)
// 	{
// 		_observerList[i]->updateInfo();
// 	}
// }

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

void Subject::notifyAddNewNode( string type, string text, int sx, int sy )
{
	for (int i = 0; i < _observerList.size(); i++)
	{
		_observerList[i]->updateAddNewNode(type, text, sx, sy);
	}
}
