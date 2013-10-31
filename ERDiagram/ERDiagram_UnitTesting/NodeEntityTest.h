#ifndef NODEENTITYTEST_H_
#define NODEECTITYTEST_H_

#include <gtest/gtest.h>
#include "NodeAttribute.h"
#include "NodeEntity.h"
#include "NodeRelationship.h"

class NodeEntityTest : public :: testing :: Test
{
protected:
	NodeEntity* _entityComponent;
	virtual void SetUp();
	virtual void TearDown();
};

#endif

