#include "DemoApplication.h"

#include "Utils/Logger.h"


bool DemoApplication::Init(const APPLICATION_INIT_DESC* desc)
{
	if (!InitWindowsAPI(desc))
	{
		Logger::Fail("Failed To Initialize Windows API");
	}

	if (!InitRenderAPI(desc))
	{
		Logger::Fail("Failed To Initialize Render API");
	}

	return true;
}

int DemoApplication::Execute()
{
	while (true)
	{
		if (!mWindowsAPI->ProcessMessage()) return true;
	}
}

bool DemoApplication::InitWindowsAPI(const APPLICATION_INIT_DESC* desc)
{
	mWindowsAPI = std::make_unique<WindowsAPI>();

	WINDOWS_API_INI_DESC winDesc{};
	winDesc.WindowName = desc->WindowName;
	winDesc.Height = desc->Height;
	winDesc.Width = desc->Width;

	return mWindowsAPI->Init(&winDesc);
}

bool DemoApplication::InitRenderAPI(const APPLICATION_INIT_DESC* desc)
{
	mRenderAPI = std::make_unique<RenderAPI>();

	RENDER_API_INIT_DESC renderDesc{};
	renderDesc.Height = desc->Height;
	renderDesc.Width = desc->Width;
	renderDesc.hWnd = mWindowsAPI->GetHWnd();

	return mRenderAPI->Init(&renderDesc);
}
