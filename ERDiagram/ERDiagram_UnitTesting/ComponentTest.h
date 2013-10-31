#ifndef COMPONENTTEST_H_
#define COMPONENTTEST_H_

#define PARAMETER_ATTRIBUTE "A"
#define PARAMETER_ENTITY "E"
#define PARAMETER_RELATIONSHIP "R"
#define PARAMETER_CONNECTOR "C"
#define PARAMETER_ALL "ALLTYPE"
#define PARAMETER_ISERROR -1
#define PARAMETER_NULL ""

#include <gtest/gtest.h>
#include "Component.h"
#include "NodeAttribute.h"
#include "NodeEntity.h"
#include "NodeRelationship.h"

class ComponentTest : public :: testing :: Test
{
protected:
	Component* _component;
	virtual void SetUp();
	virtual void TearDown();
};

#endif