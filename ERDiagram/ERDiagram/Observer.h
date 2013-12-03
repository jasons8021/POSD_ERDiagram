#ifndef OBSERVER_H_
#define OBSERVER_H_

using namespace std;

class Observer
{
public:
	Observer();
	virtual ~Observer();
	virtual void updateInfo() = 0;
};

#endif
