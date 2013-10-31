#ifndef ADDCOMPONENTCMDTEST_H_
#define ADDCOMPONENTCMDTEST_H_

#define PARAMETER_ATTRIBUTE "A"
#define PARAMETER_ENTITY "E"
#define PARAMETER_RELATIONSHIP "R"
#define PARAMETER_CONNECTOR "C"
#define PARAMETER_ALL "ALLTYPE"
#define PARAMETER_ISERROR -1
#define PARAMETER_NULL ""
#define PARAMETER_NOVALUE -1

#include "gtest/gtest.h"
#include "AddComponentCmd.h"
#include "ERModel.h"

class AddComponentCmdTest : public :: testing :: Test
{
protected:
	
	ERModel* _erModel;
	AddComponentCmd* _addComponentCmd;
	virtual void SetUp();
	virtual void TearDown();
};

#endif