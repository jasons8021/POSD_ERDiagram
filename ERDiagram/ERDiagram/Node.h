#ifndef NODE_H_
#define NODE_H_

#define PARAMETER_NULL ""

#include <vector>
#include "Component.h"

using namespace std;

class ComponentVisitor;

class Node : public Component{
public:
	Node();
	Node(int, string, string, int, int);
	virtual ~Node();
	virtual void deleteConnectedComponent(int);
	virtual void deleteAllRelatedInfo();
	virtual void connectTo(Component*);
	virtual string canConnectTo(Component*);
	virtual Component* deepClone() = 0;
	virtual void accept(ComponentVisitor*);
};

#endif