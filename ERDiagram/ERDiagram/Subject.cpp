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
void Subject::notifyObserver()
{
	for (int i = 0; i < _observerList.size(); i++)
	{
		_observerList[i]->updateInfo();
	}
}
