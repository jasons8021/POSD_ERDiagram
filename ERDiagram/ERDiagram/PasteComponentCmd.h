#ifndef PASTECOMPONENTCMD_H_
#define PASTECOMPONENTCMD_H_

#define PARAMETER_NEXTCOMPONENTID 1
#define PARAMETER_ISERROR -1

#include "Command.h"
#include "ERModel.h"
#include "Component.h"

class PasteComponentCmd : public Command
{
public:
	PasteComponentCmd(ERModel*);
	virtual ~PasteComponentCmd();
	virtual void execute();
	virtual void unexecute();
private:
	ERModel* _erModel;
	vector<int> _pasteComponentID;
	bool _isRedo;
};

#endif
