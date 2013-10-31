#ifndef DELETECOMPONENTCMDTEST_H_
#define DELETECOMPONENTCMDTEST_H_

#define PARAMETER_ATTRIBUTE "A"
#define PARAMETER_ENTITY "E"
#define PARAMETER_RELATIONSHIP "R"
#define PARAMETER_CONNECTOR "C"
#define PARAMETER_ALL "ALLTYPE"
#define PARAMETER_ISERROR -1
#define PARAMETER_NULL ""
#define PARAMETER_NOVALUE -1

#include "gtest/gtest.h"
#include "DeleteComponentCmd.h"
#include "ERModel.h"

class DeleteComponentCmdTest : public :: testing :: Test
{
protected:
	ERModel* _erModel;
	DeleteComponentCmd* _deleteAttributeCmd;
	DeleteComponentCmd* _deleteEntityCmd;
	DeleteComponentCmd* _deleteConnectorCmd;
	virtual void SetUp();
	virtual void TearDown();
};

#endif

