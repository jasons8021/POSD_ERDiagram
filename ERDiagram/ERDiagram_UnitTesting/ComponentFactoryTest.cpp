#include "ComponentFactoryTest.h"

TEST_F(ComponentFactoryTest, creatComponent){

	EXPECT_TRUE(componentFactory.creatComponent(0, "A", "Attri0") != nullptr);
	EXPECT_TRUE(componentFactory.creatComponent(1, "E", "Entity1") != nullptr);
	EXPECT_TRUE(componentFactory.creatComponent(2, "R", "Relation2") != nullptr);
	EXPECT_TRUE(componentFactory.creatComponent(3, "C", "Connect3") != nullptr);
	EXPECT_TRUE(componentFactory.creatComponent(4, "T", "ErrorType4") == nullptr);
	EXPECT_TRUE(componentFactory.creatComponent(5, "EE", "ErrorType5") == nullptr);
	EXPECT_TRUE(componentFactory.creatComponent(6, "A E", "ErrorType6") == nullptr);
}