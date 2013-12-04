#ifndef TABLEVIEWMODEL_H_
#define TABLEVIEWMODEL_H_

#define LETTER_ATTRIBUTE "A"
#define LETTER_ENTITY "E"
#define LETTER_RELATIONSHIP "R"
#define LETTER_CONNECTOR "C"
#define TYPE_ATTRIBUTE "Attribute"
#define TYPE_ENTITY "Entity"
#define TYPE_RELATIONSHIP "Relationship"
#define TYPE_CONNECTOR "Connector"

#include <QStandardItemModel>

class TableViewModel : public QStandardItemModel
{
	Q_OBJECT
public:
	TableViewModel();
	virtual ~TableViewModel();
	void addNodeIntoModel(QString, QString);
};

#endif