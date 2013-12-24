#ifndef CHANGEPRIMARYKEYCMD_H_
#define CHANGEPRIMARYKEYCMD_H_

#include <gtest/gtest_prod.h> 
#include "Command.h"
#include "ERModel.h"

class ChangePrimaryKeyCmd : public Command
{
public:
	ChangePrimaryKeyCmd(ERModel*, int, bool);
	virtual ~ChangePrimaryKeyCmd();
	virtual void execute();
	virtual void unexecute();
private:
	ERModel* _erModel;
	int _componentID;
	bool _isPK;
};

#endif