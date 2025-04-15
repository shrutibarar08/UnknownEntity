#pragma once

#define WIN32_LEAN_AND_MEAN

#include <memory>
#include <windows.h>

#include "ApplicationManager/IApplication.h"
#include "RenderManager/RenderAPI.h"
#include "WindowsManager/WindowsAPI.h"


class DemoApplication : public IApplication
{
public:
	DemoApplication() = default;
	~DemoApplication() override = default;

	DemoApplication(DemoApplication const&) = default;
	DemoApplication& operator=(DemoApplication const&) = default;

	DemoApplication(DemoApplication&&) = default;
	DemoApplication& operator=(DemoApplication&&) = default;

	bool Init(const APPLICATION_INIT_DESC* desc) override;
	int Execute() override;

private:
	bool InitWindowsAPI(const APPLICATION_INIT_DESC* desc);
	bool InitRenderAPI(const APPLICATION_INIT_DESC* desc);

private:
	std::unique_ptr<WindowsAPI> mWindowsAPI;
	std::unique_ptr<RenderAPI> mRenderAPI;

	HINSTANCE mhInstance{ nullptr };
};
