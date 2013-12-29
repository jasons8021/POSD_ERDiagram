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
	friend class DeleteGroupCmdTest;
	FRIEND_TEST(DeleteGroupCmdTest, execute);
	FRIEND_TEST(DeleteGroupCmdTest, unexecute);
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