#ifndef CONNECTORTEST_H_
#define CONNECTORTEST_H_

#include <gtest/gtest.h>
#include "Connector.h"
#include "NodeAttribute.h"
#include "NodeEntity.h"
#include "NodeRelationship.h"

class ConnectorTest : public :: testing :: Test
{
protected:
	Connector* _connectorComponent;
	virtual void SetUp();
	virtual void TearDown();
};

#endif