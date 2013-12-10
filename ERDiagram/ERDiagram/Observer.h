#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <string>

using namespace std;

class Observer
{
public:
	Observer();
	virtual ~Observer();
	virtual void updateInfo() = 0;
	virtual void updateTextChanged(int, string) = 0;
	virtual void updatePrimaryKeyChanged(int, bool) = 0;
	virtual void updateAddNewNode(string, string, int, int) = 0;
	virtual void updateConnection(int, int, string) = 0;
};

#endif
