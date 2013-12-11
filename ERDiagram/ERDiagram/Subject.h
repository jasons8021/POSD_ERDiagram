#ifndef SUBJECT_H_
#define SUBJECT_H_

#include <vector>
#include <string>
#include "Observer.h"

using namespace std;

class Subject
{
public:
	Subject();
	virtual ~Subject();
	void attachObserver(Observer* observer);
	void detachObserver(Observer* observer);
	void notifyTextChanged(int, string);
	void notifyPrimaryKeyChanged(int, bool);
	void notifyAddNewNode(int, string, string, int, int);
	void notifyNewConnection(int, int, int, string);
	void notifyDeleteComplete(string);
	void notifyReBuildConnection(string);
private:
	vector<Observer*> _observerList;
};

#endif
