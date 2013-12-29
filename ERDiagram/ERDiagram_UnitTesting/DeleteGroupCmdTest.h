#ifndef DELETEGROUPCMDTEST_H_
#define DELETEGROUPCMDTEST_H_

#include "gtest/gtest.h"
#include "DeleteGroupCmd.h"
#include "ERModel.h"

class DeleteGroupCmdTest : public :: testing :: Test
{
protected:
	ERModel* _erModel;
	DeleteGroupCmd* _deleteGroupCmd;
	virtual void SetUp();
	virtual void TearDown();
};

#endif

