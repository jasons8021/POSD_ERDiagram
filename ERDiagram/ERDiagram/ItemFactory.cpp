#include "ItemFactory.h"

ItemFactory::ItemFactory()
{
}

ItemFactory::~ItemFactory()
{
}

ItemComponent* ItemFactory::creatItem( int sx, int sy, QString type, QString text )
{
	if (type == PARAMETER_ATTRIBUTE)
		return new ItemAttribute(sx, sy, text);
	else if (type == PARAMETER_ENTITY)
		return new ItemEntity(sx, sy, text);
	else/* if (type == PARAMETER_RELATIONSHIP)*/
		return new ItemRelationship(sx, sy, text);
}

ItemComponent* ItemFactory::creatItemConnection( ItemComponent* sourceItem, ItemComponent* destinationItem, QString text )
{
	return new ItemConnection(sourceItem, destinationItem, text);
}
