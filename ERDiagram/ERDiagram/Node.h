#ifndef NODE_H_
#define NODE_H_

#define PARAMETER_NULL ""

#include <vector>
#include "Component.h"

using namespace std;

class Node : public Component{
public:
	Node();
	Node(int, string, string);
	virtual ~Node();
	virtual void deleteConnectedComponent(int);
	virtual void connectTo(Component*);
	virtual string canConnectTo(Component*);
};

#endif