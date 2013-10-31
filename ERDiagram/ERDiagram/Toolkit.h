#ifndef TOOLKIT_H_
#define TOOLKIT_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

class Toolkit
{
public:
	Toolkit();
	virtual ~Toolkit();
	static string integerToString(int);
	static vector<string> splitFunction(string, string);
};

#endif

