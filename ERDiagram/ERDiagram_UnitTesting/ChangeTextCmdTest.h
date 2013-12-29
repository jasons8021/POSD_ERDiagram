#ifndef CHANGETEXTCMDTEST_H_
#define CHANGETEXTCMDTEST_H_

#include "gtest/gtest.h"
#include "ChangeTextCmd.h"
#include "ERModel.h"

class ChangeTextCmdTest : public :: testing :: Test
{
protected:
	ERModel* _erModel;
	ChangeTextCmd* _changeTextCmd;
	virtual void SetUp();
	virtual void TearDown();
};

#endif

