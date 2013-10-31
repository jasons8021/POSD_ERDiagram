#ifndef COMMANDTEST_H_
#define COMMANDTEST_H_

#include <gtest/gtest.h>
#include "Command.h"

class CommandTest : public :: testing :: Test
{
protected:
	Command* command;
};

#endif
