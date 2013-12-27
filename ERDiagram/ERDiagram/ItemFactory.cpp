#include "ItemFactory.h"

ItemFactory::ItemFactory()
{
}

ItemFactory::~ItemFactory()
{
}

ItemComponent* ItemFactory::creatItem( int erModelID, int sx, int sy, QString type, QString text )
{
	if (type == PARAMETER_ATTRIBUTE)
		return new ItemAttribute(erModelID, sx, sy, text);
	else if (type == PARAMETER_ENTITY)
		return new ItemEntity(erModelID, sx, sy, text);
	else/* if (type == PARAMETER_RELATIONSHIP)*/
		return new ItemRelationship(erModelID, sx, sy, text);
}

ItemComponent* ItemFactory::creatItemConnection( int erModelID, ItemComponent* sourceItem, ItemComponent* destinationItem, QString text, bool isSetCardinality )
{
	return new ItemConnection(erModelID, sourceItem, destinationItem, text, isSetCardinality);
}
