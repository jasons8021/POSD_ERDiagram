#ifndef DELETEGROUPCMD_H_
#define DELETEGROUPCMD_H_

#include <vector>
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
private:
	ERModel* _erModel;
	vector<int> _componentIDSet;
	vector<DeleteComponentCmd*> _deleteCommandSet;
};

#endif