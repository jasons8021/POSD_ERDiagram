#include "SaveXmlComponentVisitor.h"

SaveXmlComponentVisitor::SaveXmlComponentVisitor()
{
	_xmlInfoString = TEXT_NULL;
	_xmlInfoString += XMLTITLE;
	_xmlInfoString += ENDLINE;
	_xmlInfoString += START_ERDIAGRAM;
	_xmlInfoString += ENDLINE;
}

SaveXmlComponentVisitor::~SaveXmlComponentVisitor()
{

}

// 拜訪到Attribute
/*
<Attribute>
	<Id>1</d>
	<Text>Emp_ID</Text>
	<positionX>100</positionX>
	<positionY>200</positionY>
</Attribute>
*/
void SaveXmlComponentVisitor::visit( NodeAttribute* visitAttribute)
{
	_xmlInfoString += TAG;
	_xmlInfoString += START_ATTRIBUTE;
	_xmlInfoString += ENDLINE;

	// <Id></Id>
	_xmlInfoString += DOUBLETAG;
	_xmlInfoString += START_ID;
	_xmlInfoString += visitAttribute->getIDString();
	_xmlInfoString += END_ID;
	_xmlInfoString += ENDLINE;

	// <Text>Emp_ID</Text>
	_xmlInfoString += DOUBLETAG;
	_xmlInfoString += START_TEXT;
	_xmlInfoString += visitAttribute->getText();
	_xmlInfoString += END_TEXT;
	_xmlInfoString += ENDLINE;

	// <positionX>100</positionX>
	_xmlInfoString += DOUBLETAG;
	_xmlInfoString += START_POSITIONX;
	_xmlInfoString += Toolkit::integerToString(visitAttribute->getSx());
	_xmlInfoString += END_POSITIONX;
	_xmlInfoString += ENDLINE;

	// <positionY>200</positionY>
	_xmlInfoString += DOUBLETAG;
	_xmlInfoString += START_POSITIONY;
	_xmlInfoString += Toolkit::integerToString(visitAttribute->getSy());
	_xmlInfoString += END_POSITIONY;
	_xmlInfoString += ENDLINE;

	_xmlInfoString += TAG;
	_xmlInfoString += END_ATTRIBUTE;
	_xmlInfoString += ENDLINE;
	
}

/*
<Entity>
	<Id>0</d>
	<Text>Engineer</Text>
	<PrimaryKey>1</PrimaryKey>
	<positionX>250</positionX>
	<positionY>300</positionY>
</Entity>
*/
// 拜訪到Entity，需要在存PrimaryKey
void SaveXmlComponentVisitor::visit( NodeEntity* visitEntity )
{
	_xmlInfoString += TAG;
	_xmlInfoString += START_ENTITY;
	_xmlInfoString += ENDLINE;

	// <Id></Id>
	_xmlInfoString += DOUBLETAG;
	_xmlInfoString += START_ID;
	_xmlInfoString += visitEntity->getIDString();
	_xmlInfoString += END_ID;
	_xmlInfoString += ENDLINE;

	// <Text>Emp_ID</Text>
	_xmlInfoString += DOUBLETAG;
	_xmlInfoString += START_TEXT;
	_xmlInfoString += visitEntity->getText();
	_xmlInfoString += END_TEXT;
	_xmlInfoString += ENDLINE;
	
	// <PrimaryKey>1,2,3,...,10</PrimaryKey>
	_xmlInfoString += DOUBLETAG;
	_xmlInfoString += START_PRIMARYKEY;
	if (visitEntity->getPrimaryKey().size() > 0)
	{
		for (int i = 0; i < visitEntity->getPrimaryKey().size() - 1; i++)
		{
			_xmlInfoString += Toolkit::integerToString(visitEntity->getPrimaryKey()[i]);
			_xmlInfoString += COMMA;
		}
		// 最後一筆
		_xmlInfoString += Toolkit::integerToString(visitEntity->getPrimaryKey()[visitEntity->getPrimaryKey().size() - 1]);
	}
	_xmlInfoString += END_PRIMARYKEY;
	_xmlInfoString += ENDLINE;

	// <positionX></positionX>
	_xmlInfoString += DOUBLETAG;
	_xmlInfoString += START_POSITIONX;
	_xmlInfoString += Toolkit::integerToString(visitEntity->getSy());
	_xmlInfoString += END_POSITIONX;
	_xmlInfoString += ENDLINE;

	// <positionY></positionY>
	_xmlInfoString += DOUBLETAG;
	_xmlInfoString += START_POSITIONY;
	_xmlInfoString += Toolkit::integerToString(visitEntity->getSy());
	_xmlInfoString += END_POSITIONY;
	_xmlInfoString += ENDLINE;

	_xmlInfoString += TAG;
	_xmlInfoString += END_ENTITY;
	_xmlInfoString += ENDLINE;
}

/*
<Relation>
	<Id>3</d>
	<Text>Has</Text>
	<positionX>400</positionX>
	<positionY>300</positionY>
</Relation>
*/
// 拜訪到Relationship
void SaveXmlComponentVisitor::visit( NodeRelationship* visitRelationship )
{
	_xmlInfoString += TAG;
	_xmlInfoString += START_RELATIONSHIP;
	_xmlInfoString += ENDLINE;

	// <Id></Id>
	_xmlInfoString += DOUBLETAG;
	_xmlInfoString += START_ID;
	_xmlInfoString += visitRelationship->getIDString();
	_xmlInfoString += END_ID;
	_xmlInfoString += ENDLINE;

	// <Text></Text>
	_xmlInfoString += DOUBLETAG;
	_xmlInfoString += START_TEXT;
	_xmlInfoString += visitRelationship->getText();
	_xmlInfoString += END_TEXT;
	_xmlInfoString += ENDLINE;

	// <positionX></positionX>
	_xmlInfoString += DOUBLETAG;
	_xmlInfoString += START_POSITIONX;
	_xmlInfoString += Toolkit::integerToString(visitRelationship->getSx());
	_xmlInfoString += END_POSITIONX;
	_xmlInfoString += ENDLINE;

	// <positionY></positionY>
	_xmlInfoString += DOUBLETAG;
	_xmlInfoString += START_POSITIONY;
	_xmlInfoString += Toolkit::integerToString(visitRelationship->getSy());
	_xmlInfoString += END_POSITIONY;
	_xmlInfoString += ENDLINE;

	_xmlInfoString += TAG;
	_xmlInfoString += END_RELATIONSHIP;
	_xmlInfoString += ENDLINE;
}

/*
<Connector>
	<Id>7</d>
	<Text></Text>
	<Source>0</Source>
	<Target>1</Target>
</Connector>
*/
// 拜訪到Connector，需要再另外存這個Connector連到那兩個Component
void SaveXmlComponentVisitor::visit( Connector* visitConnector )
{
	_xmlInfoString += TAG;
	_xmlInfoString += START_CONNECTOR;
	_xmlInfoString += ENDLINE;

	// <Id></Id>
	_xmlInfoString += DOUBLETAG;
	_xmlInfoString += START_ID;
	_xmlInfoString += visitConnector->getIDString();
	_xmlInfoString += END_ID;
	_xmlInfoString += ENDLINE;

	// <Text></Text>
	_xmlInfoString += DOUBLETAG;
	_xmlInfoString += START_TEXT;
	_xmlInfoString += visitConnector->getText();
	_xmlInfoString += END_TEXT;
	_xmlInfoString += ENDLINE;

	// <Source></Source>
	_xmlInfoString += DOUBLETAG;
	_xmlInfoString += START_SOURCE;
	_xmlInfoString += Toolkit::integerToString(static_cast<Connector*>(visitConnector)->getSourceNodeID());
	_xmlInfoString += END_SOURCE;
	_xmlInfoString += ENDLINE;

	// <Target></Target>
	_xmlInfoString += DOUBLETAG;
	_xmlInfoString += START_DESTINATION;
	_xmlInfoString += Toolkit::integerToString(static_cast<Connector*>(visitConnector)->getDestinationNodeID());
	_xmlInfoString += END_DESTINATION;
	_xmlInfoString += ENDLINE;

	_xmlInfoString += TAG;
	_xmlInfoString += END_CONNECTOR;
	_xmlInfoString += ENDLINE;
}

string SaveXmlComponentVisitor::getXmlInfo()
{
	_xmlInfoString += END_ERDIAGRAM;
	return _xmlInfoString;
}

