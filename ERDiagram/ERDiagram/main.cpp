#include <iostream>
#include <QApplication>
#include <qdebug>

#include "ERModel.h"
#include "TextUI.h"
#include "PresentationModel.h"
#include "GUI.h"

#define PARAMETER_WINDOW_WIDTH 800
#define PARAMETER_WINDOW_HEIGHT 600

using namespace std;

int main(int argc, char *argv[])
{
	ERModel* erModel = new ERModel();
	PresentationModel* presentationModel = new PresentationModel(erModel);

	if (true)
	{
		QApplication app(argc, argv);
		GUI* graphicalUI = new GUI(presentationModel);

		graphicalUI->setMaximumSize(PARAMETER_WINDOW_WIDTH, PARAMETER_WINDOW_HEIGHT);
		graphicalUI->setMinimumSize(PARAMETER_WINDOW_WIDTH, PARAMETER_WINDOW_HEIGHT);
		graphicalUI->setWindowTitle("Entity Relation Diagramming Tool");
		graphicalUI->show();
		return app.exec();
	}
	else
	{
		TextUI* textUI= new TextUI(presentationModel);

		textUI->displayMenu();

		delete textUI;
		delete presentationModel;
		delete erModel;

		return 0;
	}
}
