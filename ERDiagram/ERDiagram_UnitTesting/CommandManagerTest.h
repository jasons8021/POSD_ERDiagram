#ifndef COMMANDMANAGERTEST_H_
#define COMMANDMANAGERTEST_H_

#define PARAMETER_NULLSTRING ""
#define PARAMETER_NOVALUE -1
#define PARAMETER_PRECOMPONENTID -1
#define PARAMETER_NEXTCOMPONENTID 1
#define PARAMETER_HAVEPRIMARYKEY 0
#define PARAMETER_ATTRIBUTE "A"
#define PARAMETER_ENTITY "E"
#define PARAMETER_RELATIONSHIP "R"
#define PARAMETER_CONNECTOR "C"

#include <gtest/gtest.h>
#include "CommandManager.h"
#include "AddComponentCmd.h"
#include "ConnectComponentsCmd.h"
#include "DeleteComponentCmd.h"
#include "ERModel.h"

class CommandManagerTest : public :: testing :: Test
{
protected:
	ERModel* _erModel;
	CommandManager* _cmdManager;
	void SetUp();
	void TearDown();
};

#endif

