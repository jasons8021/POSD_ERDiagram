#ifndef PASTECOMPONENTCMDTEST_H_
#define PASTECOMPONENTCMDTEST_H_

#include "gtest/gtest.h"
#include "PasteComponentCmd.h"
#include "ERModel.h"

class PasteComponentCmdTest : public :: testing :: Test
{
protected:
	ERModel* _erModel;
	PasteComponentCmd* _pasteComponentCmd;
	virtual void SetUp();
	virtual void TearDown();
};

#endif

