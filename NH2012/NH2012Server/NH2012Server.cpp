// NH2012Server.cpp : main project file.

#include "stdafx.h"
#include "ServerGUI.h"

using namespace NH2012Server;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew ServerGUI());
	return 0;
}
