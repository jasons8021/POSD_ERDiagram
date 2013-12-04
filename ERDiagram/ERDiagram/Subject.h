#ifndef SUBJECT_H_
#define SUBJECT_H_

#include <vector>
#include "Observer.h"

using namespace std;

class Subject
{
public:
	Subject();
	virtual ~Subject();
	void attachObserver(Observer* observer);
	void detachObserver(Observer* observer);
	void notifyObserver();
	void notifyTextChanged();
private:
	vector<Observer*> _observerList;
};

#endif
