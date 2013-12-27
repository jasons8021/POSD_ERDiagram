#ifndef MOVEITEMCMD_H_
#define MOVEITEMCMD_H_

#include "Command.h"
#include "ERModel.h"
#include "Component.h"

class MoveItemCmd : public Command
{
public:
	MoveItemCmd(ERModel*, int, int, int);
	virtual ~MoveItemCmd();
	virtual void execute();
	virtual void unexecute();
private:
	ERModel* _erModel;
	int _componentID;
	int _newSx;
	int _newSy;
	int _originalSx;
	int _originalSy;
};

#endif
