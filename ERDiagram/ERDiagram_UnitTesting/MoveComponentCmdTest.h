#ifndef MOVECOMPONENTCMDTEST_H_
#define MOVECOMPONENTCMDTEST_H_

#include "gtest/gtest.h"
#include "MoveComponentCmd.h"
#include "ERModel.h"

class MoveComponentCmdTest : public :: testing :: Test
{
protected:
	ERModel* _erModel;
	MoveComponentCmd* _moveComponentCmd;
	virtual void SetUp();
	virtual void TearDown();
};

#endif