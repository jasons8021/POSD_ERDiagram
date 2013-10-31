#include "Toolkit.h"


Toolkit::Toolkit()
{
}


Toolkit::~Toolkit()
{
}

string Toolkit::integerToString( int targetNum )
{
	stringstream intNum;
	string intToStringNum;

	// int to string
	intNum << targetNum; // int to stringstream
	intNum >> intToStringNum; // stringstream to string

	return intToStringNum;
}

vector<string> Toolkit::splitFunction( string unSplitText, string splitChar )
{
	string::size_type stringPosition = 0;
	vector<string> splitTextSet;
	string token;

	unSplitText += splitChar;
	while ((stringPosition = unSplitText.find(splitChar)) != std::string::npos) {
		token = unSplitText.substr(0, stringPosition);
		splitTextSet.push_back(token);
		unSplitText.erase(0, stringPosition + splitChar.length());
	}

	return splitTextSet;
}
