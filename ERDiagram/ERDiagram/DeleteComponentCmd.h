#ifndef DELETECOMPONENTCMD_H_
#define DELETECOMPONENTCMD_H_

#define PARAMETER_NULLSTRING ""
#define PARAMETER_NOVALUE -1
#define PARAMETER_PRECOMPONENTID -1
#define PARAMETER_NEXTCOMPONENTID 1
#define PARAMETER_HAVEPRIMARYKEY 0
#define PARAMETER_ATTRIBUTE "A"
#define PARAMETER_ENTITY "E"
#define PARAMETER_RELATIONSHIP "R"
#define PARAMETER_CONNECTOR "C"

#define PARAMETER_TEXTUI_COORDINATES 0

#include <gtest/gtest_prod.h> 
#include "Command.h"
#include "ERModel.h"
#include "NodeEntity.h"
#include "Toolkit.h"

class DeleteComponentCmd : public Command
{
	friend class DeleteComponentCmdTest;
	FRIEND_TEST(DeleteComponentCmdTest, execute);
	FRIEND_TEST(DeleteComponentCmdTest, unexecute);
public:
	DeleteComponentCmd(ERModel* erModel, int);
	virtual ~DeleteComponentCmd();
	virtual void execute();
	virtual void unexecute();
	void storeInformation();
	void relatedComponentInformation();
	string reBuildRelatedConnection();
private:
	ERModel* _erModel;
	int _componentID;
	int _sourceNodeID;
	int _destinationNodeID;
	string _type;
	string _text;
	int _sx;
	int _sy;
	vector<vector<int>> _relatedConnections;
	vector<string> _relatedConnectionText;
	vector<int> _primaryKeys;
};

#endif