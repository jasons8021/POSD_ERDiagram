#include "ComponentFactory.h"

ComponentFactory::ComponentFactory()
{
}

ComponentFactory::~ComponentFactory()
{
}

Component* ComponentFactory::creatComponent( int id, string type, string text )
{
	if (type == PARAMETER_ATTRIBUTE)
		return new NodeAttribute(id, text);
	else if (type == PARAMETER_ENTITY)
		return new NodeEntity(id, text);
	else if (type == PARAMETER_RELATIONSHIP)
		return new NodeRelationship(id, text);
	else if (type == PARAMETER_CONNECTOR)
		return new Connector(id, text);
	else
		return nullptr;
}
