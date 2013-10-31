#ifndef CONNECTCOMPONENTSCMDTEST_H_
#define CONNECTCOMPONENTSCMDTEST_H_

#define PARAMETER_ATTRIBUTE "A"
#define PARAMETER_ENTITY "E"
#define PARAMETER_RELATIONSHIP "R"
#define PARAMETER_CONNECTOR "C"
#define PARAMETER_ALL "ALLTYPE"
#define PARAMETER_ISERROR -1
#define PARAMETER_NULL ""
#define PARAMETER_NOVALUE -1

#include "gtest/gtest.h"
#include "ConnectComponentsCmd.h"
#include "ERModel.h"

class ConnectComponentsCmdTest : public :: testing :: Test
{
protected:
	
	ERModel* _erModel;
	ConnectComponentsCmd* _connectComponentsCmd;
	ConnectComponentsCmd* _connectComponentsRedoCmd;
	virtual void SetUp();
	virtual void TearDown();
};

#endif
