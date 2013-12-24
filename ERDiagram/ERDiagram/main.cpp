#include <iostream>
#include <QApplication>
#include "ERModel.h"
#include "TextUI.h"
#include "PresentationModel.h"
#include "GUI.h"

#define PARAMETER_WINDOW_WIDTH 1600
#define PARAMETER_WINDOW_HEIGHT 900

using namespace std;

int main(int argc, char *argv[])
{
	ERModel* erModel = new ERModel();
	PresentationModel* presentationModel = new PresentationModel(erModel);

	if (argc == 1 || strcmp(argv[1], "guimode") == 0)
	{
		QApplication app(argc, argv);
		GUI* graphicalUI = new GUI(presentationModel);

		graphicalUI->setWindowTitle("Entity Relation Diagramming Tool");
		graphicalUI->show();
		return app.exec();
	}
	else if (argc > 1 && strcmp(argv[1], "textmode") == 0)
	{
		TextUI* textUI= new TextUI(presentationModel);

		textUI->displayMenu();

		delete textUI;
		delete presentationModel;
		delete erModel;
		return 0;
	}
}
