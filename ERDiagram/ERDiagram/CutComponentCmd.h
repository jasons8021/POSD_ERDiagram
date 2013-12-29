#ifndef CUTCOMPONENTCMD_H_
#define CUTCOMPONENTCMD_H_

#include "Command.h"
#include "ERModel.h"
#include "DeleteGroupCmd.h"

class CutComponentCmd : public Command
{
	friend class CutComponentCmdTest;
	FRIEND_TEST(CutComponentCmdTest, execute);
	FRIEND_TEST(CutComponentCmdTest, unexecute);
public:
	CutComponentCmd(ERModel*, vector<int>);
	virtual ~CutComponentCmd();
	virtual void execute();
	virtual void unexecute();
private:
	bool _isRedo;
	ERModel* _erModel;
	vector<int> _cutComponentIDSet;
	DeleteGroupCmd* _deleteGroupCmd;
};

#endif
