#ifndef CUTCOMPONENTCMDTEST_H_
#define CUTCOMPONENTCMDTEST_H_

#include "gtest/gtest.h"
#include "CutComponentCmd.h"
#include "ERModel.h"

class CutComponentCmdTest : public :: testing :: Test
{
protected:
	ERModel* _erModel;
	CutComponentCmd* _cutComponentCmd;
	virtual void SetUp();
	virtual void TearDown();
};

#endif

