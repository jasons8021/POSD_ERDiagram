#ifndef INTEGRATIONTEST_H_
#define INTEGRATIONTEST_H_

#define PARAMETER_ATTRIBUTE "A"
#define PARAMETER_ENTITY "E"
#define PARAMETER_RELATIONSHIP "R"
#define PARAMETER_CONNECTOR "C"
#define PARAMETER_ALL "ALLTYPE"
#define PARAMETER_ISERROR -1
#define PARAMETER_NULL ""
#define PARAMETER_COMPONENTSTABLE 0
#define PARAMETER_CONNECTIONSTABLE 1

#include <fstream>
#include <gtest/gtest.h>
#include "AddComponentCmd.h"
#include "Command.h"
#include "CommandManager.h"
#include "Component.h"
#include "ComponentFactory.h"
#include "ConnectComponentsCmd.h"
#include "Connector.h"
#include "DeleteComponentCmd.h"
#include "ERModel.h"
#include "Node.h"
#include "NodeAttribute.h"
#include "NodeEntity.h"
#include "NodeRelationship.h"
#include "Toolkit.h"

class IntegrationTest : public :: testing :: Test
{
protected:
	ERModel* _erModel;
	virtual void SetUp();
	virtual void TearDown();
};

#endif

