#ifndef MOVEITEMCMD_H_
#define MOVEITEMCMD_H_

#define MINUS -1

#include "Command.h"
#include "ERModel.h"
#include "Component.h"

class MoveComponentCmd : public Command
{
	friend class MoveComponentCmdTest;
	FRIEND_TEST(MoveComponentCmdTest, execute);
	FRIEND_TEST(MoveComponentCmdTest, unexecute);
public:
	MoveComponentCmd(ERModel*, vector<int>, int, int);
	virtual ~MoveComponentCmd();
	virtual void execute();
	virtual void unexecute();
private:
	ERModel* _erModel;
	vector<int> _componentID;
	int _moveDistance_x;
	int _moveDistance_y;
	int _isRedo;
};

#endif
