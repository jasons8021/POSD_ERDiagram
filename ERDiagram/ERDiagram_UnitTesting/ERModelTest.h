#ifndef ERMODELTEST_H_
#define ERMODELTEST_H_

#define PARAMETER_ATTRIBUTE "A"
#define PARAMETER_ENTITY "E"
#define PARAMETER_RELATIONSHIP "R"
#define PARAMETER_CONNECTOR "C"
#define PARAMETER_ALL "ALLTYPE"
#define PARAMETER_ISERROR -1
#define PARAMETER_NULL ""
#define PARAMETER_COMPONENTSTABLE 0
#define PARAMETER_CONNECTIONSTABLE 1

#include <gtest/gtest.h>
#include <string>
#include <fstream>
#include "ERModel.h"

class ERModelTest : public :: testing :: Test
{
protected:
	ERModel* _erModel;
	virtual void SetUp();
	virtual void TearDown();
};

#endif