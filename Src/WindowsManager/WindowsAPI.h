#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>


typedef struct WINDOWS_API_INI_DESC
{
	std::wstring WindowName;
	int Width;
	int Height;
}WINDOWS_API_INI_DESC;

class WindowsAPI
{
public:
	WindowsAPI() = default;
	~WindowsAPI() = default;

	WindowsAPI(WindowsAPI const&) = default;
	WindowsAPI& operator=(WindowsAPI const&) = default;

	WindowsAPI(WindowsAPI&&) = default;
	WindowsAPI& operator=(WindowsAPI&&) = default;

	bool Init(const WINDOWS_API_INI_DESC* desc);
	bool ProcessMessage();

	HWND const& GetHWnd() const;
	HINSTANCE const& GetHInstance() const;

	LRESULT HandleWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	bool InitWindowClass(const WINDOWS_API_INI_DESC* desc);

private:
	HINSTANCE	 mhInstance{ nullptr };
	HWND		 mhWnd{ nullptr };
	std::wstring mWindowName;
};
