#ifndef NODEATTRIBUTETEST_H_
#define NODEATTRIBUTETEST_H_

#include <gtest/gtest.h>
#include "NodeAttribute.h"
#include "NodeEntity.h"

class NodeAttributeTest : public :: testing :: Test
{
protected:
	NodeAttribute* _attributeComponent;
	virtual void SetUp();
	virtual void TearDown();
};

#endif