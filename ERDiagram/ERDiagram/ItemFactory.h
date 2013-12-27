#ifndef ITEMFACTORY_H_
#define ITEMFACTORY_H_

#define PARAMETER_ATTRIBUTE "A"
#define PARAMETER_ENTITY "E"
#define PARAMETER_RELATIONSHIP "R"
#define PARAMETER_CONNECTOR "C"

#include <QString>
#include "ItemAttribute.h"
#include "ItemComponent.h"
#include "ItemConnection.h"
#include "ItemEntity.h"
#include "ItemRelationship.h"

class ItemFactory
{
public:
	ItemFactory();
	virtual ~ItemFactory();
	ItemComponent* creatItem(int, int, int, QString, QString);
	ItemComponent* creatItemConnection(int, ItemComponent*, ItemComponent*, QString, bool);
};

#endif
