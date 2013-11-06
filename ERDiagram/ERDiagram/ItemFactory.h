#ifndef ITEMFACTORY_H_
#define ITEMFACTORY_H_

class ItemFactory
{
public:
	ItemFactory();
	~ItemFactory();
	ComponentItem* creatComponentItem(int, int, string, string);
};

#endif
