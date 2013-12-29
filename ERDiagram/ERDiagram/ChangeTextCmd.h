#ifndef CHANGETEXTCMD_H_
#define CHANGETEXTCMD_H_

#include <gtest/gtest_prod.h> 
#include "Command.h"
#include "ERModel.h"
#include "Component.h"

class ChangeTextCmd : public Command
{
	friend class ChangeTextCmdTest;
	FRIEND_TEST(ChangeTextCmdTest, execute);
	FRIEND_TEST(ChangeTextCmdTest, unexecute);
public:
	ChangeTextCmd(ERModel*, int, string);
	virtual ~ChangeTextCmd();
	virtual void execute();
	virtual void unexecute();
private:
	ERModel* _erModel;
	int _componentID;
	string _textBeforeChanged;
	string _textAfterChanged;
};

#endif
