#ifndef DELETEGROUPCMD_H_
#define DELETEGROUPCMD_H_

#define PARAMETER_CONNECTOR "C"

#include <vector>
#include <gtest/gtest_prod.h>
#include "ERModel.h"
#include "Command.h"
#include "DeleteComponentCmd.h"

using namespace std;

class DeleteGroupCmd : public Command
{
public:
	DeleteGroupCmd(ERModel*, vector<int>);
	virtual ~DeleteGroupCmd();
	virtual void execute();
	virtual void unexecute();
	void arrangeIDSet(vector<int>);
private:
	ERModel* _erModel;
	vector<int> _componentIDSet;
	vector<DeleteComponentCmd*> _deleteCommandSet;
};

#endif