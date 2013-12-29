#ifndef CHANGEPRIMARYKEYCMDTEST_H_
#define CHANGEPRIMARYKEYCMDTEST_H_

#include "gtest/gtest.h"
#include "ChangePrimaryKeyCmd.h"
#include "ERModel.h"

class ChangePrimaryKeyCmdTest : public :: testing :: Test
{
protected:
	ERModel* _erModel;
	ChangePrimaryKeyCmd* _changePrimaryKeyCmd;
	virtual void SetUp();
	virtual void TearDown();
};

#endif

