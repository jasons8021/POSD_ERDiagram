#include "Subject.h"

Subject::Subject()
{
}

Subject::~Subject()
{
}

// 新增observer
void Subject::attachObserver( Observer* observer )
{
	_observerList.push_back(observer);
}

// 刪除observer
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

// 通知每個observer
void Subject::notifyObserver()
{
	for (int i = 0; i < _observerList.size(); i++)
	{
		_observerList[i]->updateInfo();
	}
}
