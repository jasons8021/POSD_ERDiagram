#ifndef NODERELATIONSHIPTEST_H_
#define NODERELATIONSHIPTEST_H_

#include <gtest/gtest.h>
#include "NodeRelationship.h"
#include "NodeEntity.h"

class NodeRelationshipTest  : public :: testing :: Test
{
protected:
	NodeRelationship* _relationshipComponent;
	virtual void SetUp();
	virtual void TearDown();
};

#endif