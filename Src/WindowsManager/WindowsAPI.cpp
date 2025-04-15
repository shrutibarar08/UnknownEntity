#include "WindowsAPI.h"


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WindowsAPI* pWindow = nullptr;

    if (uMsg == WM_NCCREATE) {
        // Extract 'this' pointer passed via CreateWindowEx
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pWindow = static_cast<WindowsAPI*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
    }
    else {
        // Retrieve stored pointer
        pWindow = reinterpret_cast<WindowsAPI*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pWindow) {
        return pWindow->HandleWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool WindowsAPI::Init(const WINDOWS_API_INI_DESC* desc)
{
    return InitWindowClass(desc);
}

bool WindowsAPI::ProcessMessage()
{
    MSG msg{};

    while (PeekMessage(&msg, nullptr,
        0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT) return false;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}

HWND const& WindowsAPI::GetHWnd() const
{
    return mhWnd;
}

HINSTANCE const& WindowsAPI::GetHInstance() const
{
    return mhInstance;
}

LRESULT WindowsAPI::HandleWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

bool WindowsAPI::InitWindowClass(const WINDOWS_API_INI_DESC* desc)
{
    mhInstance = GetModuleHandle(nullptr);
    mWindowName = desc->WindowName;

    constexpr LPCWSTR CLASS_NAME{ L"WindowsAPI" };

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;  // Set the static WindowProc
    wc.hInstance = mhInstance;
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClass(&wc))
    {
        return false;
    }

    mhWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        mWindowName.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr,
        nullptr,
        mhInstance,
        this
    );

    if (!mhWnd)
    {
        return false;
    }

    ShowWindow(mhWnd, SW_SHOW);
    UpdateWindow(mhWnd);

    return true;
}
