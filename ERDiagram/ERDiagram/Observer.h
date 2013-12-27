#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <string>
#include <vector>

using namespace std;

class Observer
{
public:
	Observer();
	virtual ~Observer();
	virtual void updateTextChanged(int, string) = 0;
	virtual void updatePrimaryKeyChanged(int, bool) = 0;
	virtual void updateAddNewNode(int, string, string, int, int) = 0;
	virtual void updateConnection(int, int, int, string) = 0;
	virtual void updateDeleteComplete(string) = 0;
	virtual void updateReBuildConnection(string) = 0;
	virtual void updatePasteComponent(vector<int>) = 0;
	virtual void updateItemPosition(int, int, int) = 0;
};

#endif
