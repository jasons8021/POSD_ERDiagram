#ifndef ERDIAGRAMMESSAGEBOXMANAGER_H_
#define ERDIAGRAMMESSAGEBOXMANAGER_H_

#include <QMessageBox>

using namespace std;

class ERDiagramMessageBoxManager
{
public:
	static void showMessageBox(string);
	static void showMessageBox(string, string);
};

#endif