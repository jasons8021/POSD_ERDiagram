#include "ComponentFactoryTest.h"

TEST_F(ComponentFactoryTest, creatComponent){

	EXPECT_TRUE(componentFactory.creatComponent(0, "A", "Attri0", 0, 0) != nullptr);
	EXPECT_TRUE(componentFactory.creatComponent(1, "E", "Entity1", 0, 0) != nullptr);
	EXPECT_TRUE(componentFactory.creatComponent(2, "R", "Relation2", 0, 0) != nullptr);
	EXPECT_TRUE(componentFactory.creatComponent(3, "C", "Connect3", 0, 0) != nullptr);
	EXPECT_TRUE(componentFactory.creatComponent(4, "T", "ErrorType4", 0, 0) == nullptr);
	EXPECT_TRUE(componentFactory.creatComponent(5, "EE", "ErrorType5", 0, 0) == nullptr);
	EXPECT_TRUE(componentFactory.creatComponent(6, "A E", "ErrorType6", 0, 0) == nullptr);
}