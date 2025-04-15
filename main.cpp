#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "ApplicationManager/DemoApplication/DemoApplication.h"

int WIN32 WinMain(
	HINSTANCE hInstance,
	HINSTANCE prevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	DemoApplication app{};
	APPLICATION_INIT_DESC desc{};
	desc.Height = 720;
	desc.Width = 1280;
	desc.WindowName = L"DemoApplication";

	if (!app.Init(&desc))
	{
		return E_FAIL;
	}
	return app.Execute();
}
