#ifndef PRESENTATIONMODELTEST_H_
#define PRESENTATIONMODELTEST_H_

#define PARAMETER_ALL "ALLTYPE"
#define PARAMETER_NULL ""
#define PARAMETER_ATTRIBUTE "A"
#define PARAMETER_ENTITY "E"
#define PARAMETER_RELATIONSHIP "R"

#include <gtest/gtest.h>
#include "ERModel.h"
#include "PresentationModel.h"

class PresentationModelTest : public :: testing :: Test
{
protected:
	ERModel* _erModel;
	PresentationModel* _presentationModel;
	virtual void SetUp();
	virtual void TearDown();
};

#endif